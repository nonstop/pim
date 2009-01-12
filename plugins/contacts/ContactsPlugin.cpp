#include <QtPlugin>
#include <QDebug>
#include <QtSql>
#include <QApplication>

#include "config.h"

#include "PimDatabase.h"
#include "ContactsPlugin.h"
#include "ContactDlg.h"

Q_EXPORT_PLUGIN2(contacts, ContactsPlugin)

void ContactsPlugin::getInfo(PluginInfo& info)
{
    info.uid = 3;
}

void ContactsPlugin::init()
{
    Group g;
    g.name = tr("Contacts");
    g.group = 0;
    PimDatabase::instance()->newGroup(g);
    qDebug() << "added contact group: "<< g.id;

    QSqlQuery q(PimDatabase::instance()->db());
    q.exec("CREATE TABLE contacts ( "
        "id INT PRIMARY KEY, "
        "firstname VARCHAR, "
        "secname VARCHAR, "
        "famname VARCHAR, "
        "mail VARCHAR, "
        "address TEXT, "
        "notes TEXT)");
    q.exec("CREATE TABLE cont_phones ( "
        "id_cont INT PRIMARY KEY, "
        "phone VARCHAR)");
}

void ContactsPlugin::installTranslator(const QString& locale)
{
    QString trFile(PIM_TRANSLATIONS_DIR);
    trFile += "/PimContacts_" + locale;
    if (!m_tr.load(trFile))
        qDebug() << "failed loading translations from " << trFile;
    else
        qDebug() << "translations loaded from " << trFile;
    QApplication::installTranslator(&m_tr);
}

QIcon ContactsPlugin::icon()
{
    return QIcon(":contact");
}

QString ContactsPlugin::name()
{
    return tr("Contact");
}

bool ContactsPlugin::newRecord(QWidget* parent, int id)
{
    qDebug() << "new contact: " << id;
    ContactDlg dlg(parent);
    if (dlg.exec() != QDialog::Accepted)
        return false;

    QSqlQuery q(PimDatabase::instance()->db());
    q.prepare(
        "INSERT INTO contacts (id, firstname, secname, famname, mail, address, notes) "
        "VALUES (:id, :firstname, :secname, :famname, :mail, :address, :notes)");
    q.bindValue(":id", id);
    q.bindValue(":firstname", dlg.firstName());
    q.bindValue(":secname", dlg.secondName());
    q.bindValue(":famname", dlg.familyName());
    q.bindValue(":mail", dlg.mail());
    q.bindValue(":address", dlg.address());
    q.bindValue(":notes", dlg.notes());
    q.exec();

    qDebug() << q.lastError();

    return true;
}

bool ContactsPlugin::editRecord(QWidget* parent, int id)
{
    qDebug() << "edit contact: " << id;
    ContactDlg dlg(parent, id);
    
    QSqlQuery q(PimDatabase::instance()->db());
    q.prepare("SELECT id, firstname, secname, famname, mail, address, notes FROM contacts WHERE id = :id");
    q.bindValue(":id", id);
    q.exec();

    while (q.next())
    {
        dlg.setFirstName(q.value(0).toString());
        dlg.setSecondName(q.value(1).toString());
        dlg.setFamilyName(q.value(2).toString());
        dlg.setMail(q.value(3).toString());
        dlg.setAddress(q.value(4).toString());
        dlg.setNotes(q.value(5).toString());
    }

    if (dlg.exec() != QDialog::Accepted)
        return false;
    
    q.prepare(
        "UPDATE contacts SET firstname=:firstname, secname=:secname, famname=:famname, "
        "mail=:mail, address=:address, notes=:notes "
        "WHERE id = :id");
    q.bindValue(":id", id);
    q.bindValue(":firstname", dlg.firstName());
    q.bindValue(":secname", dlg.secondName());
    q.bindValue(":famname", dlg.familyName());
    q.bindValue(":mail", dlg.mail());
    q.bindValue(":address", dlg.address());
    q.bindValue(":notes", dlg.notes());
    q.exec();

    qDebug() << q.lastError();

    return true;
}

bool ContactsPlugin::deleteRecord(QWidget* parent, int id)
{
    QSqlQuery q(PimDatabase::instance()->db());
    q.prepare("delete from contacts where id = :id");
    q.bindValue(":id", id);
    q.exec();

    q.prepare("delete from cont_phones where id_cont = :id");
    q.bindValue(":id", id);
    q.exec();

    return true;
}

QString ContactsPlugin::recordDisplayName(int id)
{
    QSqlQuery q(PimDatabase::instance()->db());
    q.prepare("SELECT firstname, secname, famname FROM contacts WHERE id = :id");
    q.bindValue(":id", id);
    q.exec();
    while (q.next())
    {
        QString res = q.value(2).toString() + " ";
        res += q.value(0).toString() + " ";
        res += q.value(1).toString();
        return res;
    }
    qDebug() << q.lastError();
    return "---";
}

QString ContactsPlugin::recordSummary(int id)
{
    QSqlQuery q(PimDatabase::instance()->db());
    q.prepare("SELECT firstname, secname, famname, mail, address FROM contacts WHERE id = :id");
    q.bindValue(":id", id);
    q.exec();
    QString res;
    while (q.next())
    {
        res += q.value(0).toString() + "\n";
        res += q.value(1).toString() + "\n";
        res += q.value(2).toString() + "\n";
        res += tr("Mail:") + q.value(3).toString() + "\n";
        res += tr("Address:") + q.value(4).toString();
    }
    return res;
}

