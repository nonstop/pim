#include <QtPlugin>
#include <QDebug>
#include <QtSql>
#include <QApplication>

#include "config.h"

#include "PimDatabase.h"
#include "PasswordsPlugin.h"
#include "PasswordsDlg.h"

Q_EXPORT_PLUGIN2(passwords, PasswordsPlugin)

void PasswordsPlugin::getInfo(PluginInfo& info)
{
    info.uid = 1;
}

void PasswordsPlugin::init()
{
    Group g;
    g.name = tr("Passwords");
    g.group = 0;
    PimDatabase::instance()->newGroup(g);
    qDebug() << "added password group: "<< g.id;

    QSqlQuery q(PimDatabase::instance()->db());
    q.exec("CREATE TABLE passwords ( "
        "id INT PRIMARY KEY, "
        "service VARCHAR, "
        "login VARCHAR, "
        "password VARCHAR, "
        "mail VARCHAR, "
        "url VARCHAR, "
        "notes TEXT)");
}

void PasswordsPlugin::installTranslator(const QString& locale)
{
    QString trFile(PIM_TRANSLATIONS_DIR);
    trFile += "/PimPasswords_" + locale;
    if (!m_tr.load(trFile))
        qDebug() << "failed loading translations from " << trFile;
    else
        qDebug() << "translations loaded from " << trFile;
    QApplication::installTranslator(&m_tr);
}

QIcon PasswordsPlugin::icon()
{
    return QIcon(":password");
}

QString PasswordsPlugin::name()
{
    return tr("Password");
}

bool PasswordsPlugin::newRecord(QWidget* parent, int id)
{
    qDebug() << "new password: " << id;
    PasswordsDlg dlg(parent);
    if (dlg.exec() != QDialog::Accepted)
        return false;

    QSqlQuery q(PimDatabase::instance()->db());
    q.prepare(
        "INSERT INTO passwords (id, service, login, password, mail, url, notes) "
        "VALUES (:id, :service, :login, :password, :mail, :url, :notes)");
    q.bindValue(":id", id);
    q.bindValue(":service", dlg.service());
    q.bindValue(":login", dlg.login());
    q.bindValue(":password", dlg.password());
    q.bindValue(":mail", dlg.mail());
    q.bindValue(":url", dlg.url());
    q.bindValue(":notes", dlg.notes());
    q.exec();

    qDebug() << q.lastError();

    return true;
}

bool PasswordsPlugin::editRecord(QWidget* parent, int id)
{
    qDebug() << "edit password: " << id;
    PasswordsDlg dlg(parent, id);
    
    QSqlQuery q(PimDatabase::instance()->db());
    q.prepare("SELECT service, login, password, mail, url, notes FROM passwords WHERE id = :id");
    q.bindValue(":id", id);
    q.exec();

    while (q.next())
    {
        dlg.setService(q.value(0).toString());
        dlg.setLogin(q.value(1).toString());
        dlg.setPassword(q.value(2).toString());
        dlg.setMail(q.value(3).toString());
        dlg.setUrl(q.value(4).toString());
        dlg.setNotes(q.value(5).toString());
    }

    if (dlg.exec() != QDialog::Accepted)
        return false;
    
    q.prepare(
        "UPDATE passwords SET service=:service, login=:login, password=:password, mail=:mail, url=:url, notes=:notes "
        "WHERE id = :id");
    q.bindValue(":id", id);
    q.bindValue(":service", dlg.service());
    q.bindValue(":login", dlg.login());
    q.bindValue(":password", dlg.password());
    q.bindValue(":mail", dlg.mail());
    q.bindValue(":url", dlg.url());
    q.bindValue(":notes", dlg.notes());
    q.exec();

    qDebug() << q.lastError();

    return true;
}

bool PasswordsPlugin::deleteRecord(QWidget* parent, int id)
{
    QSqlQuery q(PimDatabase::instance()->db());
    q.prepare("delete from passwords where id = :id");
    q.bindValue(":id", id);
    return q.exec();
}

QString PasswordsPlugin::recordDisplayName(int id)
{
    QSqlQuery q(PimDatabase::instance()->db());
    q.prepare("SELECT service||' - '||login FROM passwords WHERE id = :id");
    q.bindValue(":id", id);
    q.exec();
    while (q.next())
        return q.value(0).toString();
    qDebug() << q.lastError();
    return "---";
}

QString PasswordsPlugin::recordSummary(int id)
{
    QSqlQuery q(PimDatabase::instance()->db());
    q.prepare("SELECT service, login, mail, url FROM passwords WHERE id = :id");
    q.bindValue(":id", id);
    q.exec();
    QString res;
    while (q.next())
    {
        res += tr("Service:") + q.value(0).toString() + "\n";
        res += tr("Login:") + q.value(1).toString() + "\n";
        res += tr("Mail:") + q.value(2).toString() + "\n";
        res += tr("Url:") + q.value(3).toString();
    }
    return res;
}

