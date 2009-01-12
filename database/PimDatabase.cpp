#include <QtSql>

#include "PimException.h"
#include "PimDatabase.h"

#define DBG qDebug() << __FILE__ << ":" << __LINE__ << " "

PimDatabase* PimDatabase::instance()
{
    static PimDatabase* pdb = NULL;
    if (!pdb)
        pdb = new PimDatabase;
    return pdb;
}

PimDatabase::PimDatabase()
{}

void PimDatabase::commit()
{
    m_db.commit();
}

void PimDatabase::rollback()
{
    m_db.rollback();
}

void PimDatabase::transaction()
{
    m_db.transaction();
}

void PimDatabase::open(const QString& path)
{
    if (m_db.isOpen())
        m_db.close();
    if (!m_db.databaseName().isEmpty())
        QSqlDatabase::removeDatabase(m_db.databaseName());

    m_db = QSqlDatabase::addDatabase("QSQLITE", path);
    m_db.setDatabaseName(path);

    if (!m_db.open())
        throw PIMEXCEPTION("failed to open database");    

}

void PimDatabase::close()
{
    DBG << "closing db";
    if (m_db.isOpen())
        m_db.close();
    if (!m_db.databaseName().isEmpty())
        QSqlDatabase::removeDatabase(m_db.databaseName());
}

void PimDatabase::create(const QString& path)
{
    open(path);

    QSqlQuery q(m_db);
    q.exec("CREATE TABLE groups (id INT PRIMARY KEY, name VARCHAR NOT NULL, notes VARCHAR, id_group INT NOT NULL)");
    q.exec("CREATE TABLE records (id INT PRIMARY KEY, id_group INT NOT NULL, record_type INT NOT NULL)");
}

int PimDatabase::nextId(const QString& table)
{
    QString sql = "SELECT max(id) + 1 FROM " + table;
    QSqlQuery q(sql, m_db);
    if (!q.exec())
        DBG << q.lastError();
    while (q.next())
        return q.value(0).toInt();
    return 0;
}

void PimDatabase::loadGroups(Groups& groups, int group)
{
    QSqlQuery q(m_db);
    q.prepare("SELECT id, name, id_group FROM groups WHERE id_group = :group ORDER BY name");
    q.bindValue(":group", group);
    if (!q.exec())
        DBG << q.lastError();
    while (q.next())
    {
        Group group;
        group.id = q.value(0).toInt();
        group.name = q.value(1).toString();
        group.group = q.value(2).toInt();
        groups.push_back(group);
    }
    if (!q.size())
        DBG << q.lastError();
}

void PimDatabase::newGroup(Group& group)
{
    group.id = nextId("groups");
    QSqlQuery q("INSERT INTO groups (id, name, notes, id_group) VALUES (:id, :name, :notes, :group)", m_db);
    q.bindValue(":id", group.id);
    q.bindValue(":name", group.name);
    q.bindValue(":notes", group.notes);
    q.bindValue(":group", group.group);
    if (!q.exec())
        DBG << q.lastError();
}

void PimDatabase::saveGroup(const Group& group)
{
    QSqlQuery q(m_db);
    q.prepare("UPDATE groups SET name = :name, id_group = :group, notes = :notes  WHERE id = :id");
    q.bindValue(":id", group.id);
    q.bindValue(":name", group.name);
    q.bindValue(":notes", group.notes);
    q.bindValue(":group", group.group);
    if (!q.exec())
        DBG << q.lastError();
    if (!q.numRowsAffected())
        DBG << "no rows affected, group: " << group;
}

void PimDatabase::loadGroup(int id, Group& group)
{
    QSqlQuery q("SELECT id, name, notes, id_group FROM groups WHERE id = :id", m_db);
    q.bindValue(":id", id);
    if (!q.exec())
        DBG << q.lastError();
    while (q.next())
    {
        group.id = q.value(0).toInt();
        group.name = q.value(1).toString();
        group.notes = q.value(2).toString();
        group.group = q.value(3).toInt();
    }
    if (!q.size())
        DBG << "Group not found";
}

void PimDatabase::deleteGroup(const Group& group)
{
    QSqlQuery q(m_db);
    
    q.prepare("UPDATE groups SET id_group = :group WHERE id_group = :id");
    q.bindValue(":group", group.group);
    q.bindValue(":id", group.id);
    if (!q.exec())
        DBG << q.lastError();

    q.prepare("UPDATE records SET id_group = :group WHERE id_group = :id");
    q.bindValue(":group", group.group);
    q.bindValue(":id", group.id);
    if (!q.exec())
        DBG << q.lastError();

    q.prepare("DELETE FROM groups WHERE id = :id");
    q.bindValue(":id", group.id);
    if (!q.exec())
        DBG << q.lastError();
    if (!q.numRowsAffected())
        DBG << "no rows affected";
}

void PimDatabase::newRecord(Record& record)
{
    record.id = nextId("records");
    QSqlQuery q("INSERT INTO records (id, id_group, record_type) VALUES (:id, :group, :type)", m_db);
    q.bindValue(":id", record.id);
    q.bindValue(":group", record.group);
    q.bindValue(":type", record.type);
    if (!q.exec())
        DBG << q.lastError();
}

void PimDatabase::loadRecords(int idGroup, Records& recs)
{
    qDebug("loading records for group %d", idGroup);
    QSqlQuery q(m_db);
    q.prepare("SELECT id, record_type FROM records WHERE id_group = :idGroup");
    q.bindValue(":idGroup", idGroup);
    if (!q.exec())
        DBG << q.lastError();
    while (q.next())
    {
        Record rec;
        rec.id = q.value(0).toInt();
        rec.type = q.value(1).toInt();
        rec.group = idGroup;
        recs.push_back(rec);
    }
}

void PimDatabase::loadRecord(int id, Record& rec)
{
    QSqlQuery q("SELECT id, record_type, id_group FROM records WHERE id = :id", m_db);
    q.bindValue(":id", id);
    if (!q.exec())
        DBG << q.lastError();
    while (q.next())
    {
        rec.id = q.value(0).toInt();
        rec.type = q.value(1).toInt();
        rec.group = q.value(2).toInt();
    }
    if (!q.size())
        DBG << "Record not found";
}

void PimDatabase::saveRecord(const Record& rec)
{
    QSqlQuery q("UPDATE records SET id_group = :group, record_type = :type WHERE id = :id", m_db);
    q.bindValue(":id", rec.id);
    q.bindValue(":type", rec.type);
    q.bindValue(":group", rec.group);
    if (!q.exec())
        DBG << q.lastError();
    if (!q.numRowsAffected())
        DBG << "no rows affected";
}

void PimDatabase::deleteRecord(int id)
{
    QSqlQuery q("DELETE FROM records WHERE id = :id", m_db);
    q.bindValue(":id", id);
    if (!q.exec())
        DBG << q.lastError();
    if (!q.numRowsAffected())
        DBG << "no rows affected";
}

