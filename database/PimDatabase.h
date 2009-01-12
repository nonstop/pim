#ifndef PIMDATABASE_H
#define PIMDATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QVector>

struct Record
{
    int id;
    int type;
    int group;
    operator QString() const
    {
        QString res("[");
        res += "id: " + QString::number(id);
        res += ",type: " + QString::number(type);
        res += ",group: " + QString::number(group);
        res += "]";
        return res;
    }
};
typedef QVector<Record> Records;
typedef Records::const_iterator RecordsConstIter;

struct Group
{
    int id;
    QString name;
    QString notes;
    int group;
    operator QString() const
    {
        QString res("[");
        res += "id: " + QString::number(id);
        res += ",name: " + name;
        res += ",group: " + QString::number(group);
        res += "]";
        return res;
    }
};
typedef QVector<Group> Groups;
typedef Groups::const_iterator GroupsConstIter;

class PimDatabase
    : public QObject
{
    Q_OBJECT
public:
    static PimDatabase* instance();
    
    QSqlDatabase& db()
    {   return m_db;    }

    void open(const QString& path);
    void close();
    void create(const QString& path); 

    void commit();
    void rollback();
    void transaction();

    int nextId(const QString& table);
    
    void loadGroups(Groups& groups, int group);
    void newGroup(Group& group);
    void loadGroup(int id, Group& group);
    void saveGroup(const Group& group);
    void deleteGroup(const Group& group);

    void newRecord(Record& record);
    void loadRecords(int idGroup, Records& recs);
    void loadRecord(int id, Record& rec);
    void saveRecord(const Record& rec);
    void deleteRecord(int id);
private:
    PimDatabase();

    QSqlDatabase m_db;
};

#endif /* PIMDATABASE_H */

