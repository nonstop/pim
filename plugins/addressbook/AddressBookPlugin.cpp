#include <QtPlugin>
#include <QtSql>
#include <QDebug>
#include <QApplication>

#include "config.h"

#include "AddressBookDlg.h"
#include "PimDatabase.h"
#include "AddressBookPlugin.h"

Q_EXPORT_PLUGIN2(addressbook, AddressBookPlugin)

void AddressBookPlugin::getInfo(PluginInfo& info)
{
    info.uid = 2;
}

void AddressBookPlugin::init()
{
    Group g;
    g.name = tr("Address book");
    g.group = 0;
    PimDatabase::instance()->newGroup(g);
    qDebug() << "added address book group: " << g.id;

    QSqlQuery q(PimDatabase::instance()->db());
    q.exec("CREATE TABLE addresses ( "
        "id INT PRIMARY KEY, "
        "name VARCHAR, "
        "address TEXT, "
        "notes TEXT)");
    q.exec("CREATE TABLE addr_phones ( "
        "id_addr INT, "
        "phone VARCHAR)");
}

void AddressBookPlugin::installTranslator(const QString& locale)
{
    QString trFile(PIM_TRANSLATIONS_DIR);
    trFile += "/PimAddressBook_" + locale;
    if (!m_tr.load(trFile))
        qDebug() << "failed loading translations from " << trFile;
    else
        qDebug() << "translations loaded from " << trFile;
    QApplication::installTranslator(&m_tr);
}

QIcon AddressBookPlugin::icon()
{
    return QIcon(":address");
}

QString AddressBookPlugin::name()
{
    return tr("Address book");
}

namespace
{
bool updatePhones(int id, const Phones& phones)
{
    qDebug() << "updating phones for " << id;
    QSqlQuery q(PimDatabase::instance()->db());
    q.prepare("DELETE FROM addr_phones WHERE id_addr = :id");
    q.bindValue(":id", id);
    if (!q.exec())
    {
        qDebug() << __FILE__ << ":" << __LINE__ << " "  << q.lastError();
        return false;
    }

    q.prepare(
        "INSERT INTO addr_phones (id_addr, phone) "
        "VALUES (:id_addr, :phone)");
    for (int i=0; i<phones.size(); ++i)
    {
        q.bindValue(":id_addr", id);
        q.bindValue(":phone", phones[i].number);
        if (!q.exec())
        {
            qDebug() << __FILE__ << ":" << __LINE__ << " " << q.lastError();
            return false;
        }
        qDebug() << i << " inserted " << phones[i].number;
    }
    return true;
}
}

bool AddressBookPlugin::newRecord(QWidget* parent, int id)
{
    qDebug() << "new address: " << id;
    AddressBookDlg dlg(parent);
    if (dlg.exec() != QDialog::Accepted)
        return false;

    QSqlQuery q(PimDatabase::instance()->db());
    q.prepare(
        "INSERT INTO addresses (id, name, address, notes) "
        "VALUES (:id, :name, :address, :notes)");
    q.bindValue(":id", id);
    q.bindValue(":name", dlg.name());
    q.bindValue(":address", dlg.address());
    q.bindValue(":notes", dlg.notes());

    if (!q.exec())
    {
        qDebug() << __FILE__ << ":" << __LINE__ << " "  << q.lastError();
        return false;
    }
    return updatePhones(id, dlg.phones());
}

bool AddressBookPlugin::editRecord(QWidget* parent, int id)
{
    qDebug() << "edit address: " << id;
    AddressBookDlg dlg(parent, id);
    
    QSqlQuery q(PimDatabase::instance()->db());
    q.prepare("SELECT name, address, notes FROM addresses WHERE id = :id");
    q.bindValue(":id", id);
    q.exec();
    while (q.next())
    {
        dlg.setName(q.value(0).toString());
        dlg.setAddress(q.value(1).toString());
        dlg.setNotes(q.value(2).toString());
    }
    Phones phs;
    q.prepare("SELECT phone FROM addr_phones WHERE id_addr = :id");
    q.bindValue(":id", id);
    q.exec();
    while (q.next())
    {
        qDebug() << q.value(0);
        phs.push_back(q.value(0).toString());
    }
    dlg.setPhones(phs);

    if (dlg.exec() != QDialog::Accepted)
        return false;

    q.prepare(
        "UPDATE addresses SET name=:name, address=:address, notes=:notes "
        "WHERE id = :id");
    q.bindValue(":id", id);
    q.bindValue(":name", dlg.name());
    q.bindValue(":address", dlg.address());
    q.bindValue(":notes", dlg.notes());
    if (!q.exec())
    {
        qDebug() << __FILE__ << ":" << __LINE__ << " "  << q.lastError();
        return false;
    }
    
    return updatePhones(id, dlg.phones());
}

bool AddressBookPlugin::deleteRecord(QWidget* /*parent*/, int id)
{
    QSqlQuery q(PimDatabase::instance()->db());
    q.prepare("delete from addresses where id = :id");
    q.bindValue(":id", id);
    if (!q.exec())
    {
        qDebug() << __FILE__ << ":" << __LINE__ << " "  << q.lastError();
        return false;
    }

    q.prepare("delete from addr_phones where id_addr = :id");
    q.bindValue(":id", id);
    if (!q.exec())
    {
        qDebug() << __FILE__ << ":" << __LINE__ << " "  << q.lastError();
        return false;
    }

    return true;
}

QString AddressBookPlugin::recordDisplayName(int id)
{
    QSqlQuery q(PimDatabase::instance()->db());
    q.prepare("SELECT name FROM addresses WHERE id = :id");
    q.bindValue(":id", id);
    q.exec();
    while (q.next())
        return q.value(0).toString();
    qDebug() << __FILE__ << ":" << __LINE__ << " "  << q.lastError();
    return "---";
}

QString AddressBookPlugin::recordSummary(int id)
{
    QSqlQuery q(PimDatabase::instance()->db());
    q.prepare("SELECT name, address FROM addresses WHERE id = :id");
    q.bindValue(":id", id);
    q.exec();
    QString res;
    while (q.next())
    {
        res += tr("Name:") + q.value(0).toString() + "\n";
        res += tr("Address:") + q.value(1).toString();
    }
    return res;
}

