#ifndef CONTACTDLG_H
#define CONTACTDLG_H

#include <QDialog>

class QLineEdit;
class QTextEdit;

class ContactDlg
    : public QDialog
{
    Q_OBJECT
public:
    ContactDlg(QWidget* parent, int id);
    ContactDlg(QWidget* parent);
    virtual ~ContactDlg();

    QString firstName() const;
    QString secondName() const;
    QString familyName() const;
    QString mail() const;
    QString address() const;
    QString notes() const;

    void setFirstName(const QString& txt);
    void setSecondName(const QString& txt);
    void setFamilyName(const QString& txt);
    void setMail(const QString& txt);
    void setAddress(const QString& txt);
    void setNotes(const QString& txt);

private slots:
    void onOk();
private:
    void createWidgets();
    
    QLineEdit* m_leFirstName;
    QLineEdit* m_leSecondName;
    QLineEdit* m_leFamilyName;
    QLineEdit* m_leMail;
    QTextEdit* m_teAddress;
    QTextEdit* m_teNotes;
};

#endif /* CONTACTDLG_H */

