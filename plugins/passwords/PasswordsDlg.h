#ifndef PASSWORDSDLG_H
#define PASSWORDSDLG_H

#include <QDialog>

class QLineEdit;
class QTextEdit;

class PasswordsDlg
    : public QDialog
{
    Q_OBJECT
public:
    PasswordsDlg(QWidget* parent, int id);
    PasswordsDlg(QWidget* parent);
    virtual ~PasswordsDlg();

    QString service() const;
    QString login() const;
    QString password() const;
    QString mail() const;
    QString url() const;
    QString notes() const;

    void setService(const QString& txt);
    void setLogin(const QString& txt);
    void setPassword(const QString& txt);
    void setMail(const QString& txt);
    void setUrl(const QString& txt);
    void setNotes(const QString& txt);

private slots:
    void onOk();
    void showHidePassword();
private:
    void createWidgets();
    
    QLineEdit* m_leService;
    QLineEdit* m_leLogin;
    QLineEdit* m_lePassword;
    QLineEdit* m_leMail;
    QLineEdit* m_leUrl;
    QTextEdit* m_teNotes;
};

#endif /* PASSWORDSDLG_H */

