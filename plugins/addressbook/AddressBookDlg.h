#ifndef ADDRESSBOOKDLG_H
#define ADDRESSBOOKDLG_H

#include <QDialog>
#include "PhonesWidget.h"

class QLineEdit;
class QTextEdit;

class AddressBookDlg
    : public QDialog
{
    Q_OBJECT
public:
    AddressBookDlg(QWidget*);
    AddressBookDlg(QWidget*, int id);
    virtual ~AddressBookDlg();

    QString name() const;
    QString address() const;
    QString notes() const;
    Phones phones() const;

    void setName(const QString& name);
    void setAddress(const QString& address);
    void setNotes(const QString& notes);
    void setPhones(const Phones& phones);
private slots:
    void onOk();
private:
    void createWidgets();

    QLineEdit* m_leName;
    QTextEdit* m_teAddress;
    QTextEdit* m_teNotes;
    PhonesWidget* m_phones;
};

#endif /* ADDRESSBOOKDLG_H */

