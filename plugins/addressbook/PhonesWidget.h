#ifndef PHONESWIDGET_H
#define PHONESWIDGET_H

#include <QWidget>

class QLineEdit;
class QListWidget;
class QListWidgetItem;

struct Phone
{
    Phone()
    {}
    Phone(const QString& num)
        : number(num)
    {}
    QString number;
};
typedef QVector<Phone> Phones;

class PhonesWidget
    : public QWidget
{
    Q_OBJECT
public:
    PhonesWidget();
    virtual ~PhonesWidget();

    void setPhones(const Phones& phones);
    Phones phones() const;
private slots:
    void onAddPhone();
    void onDeletePhone();
    void onDoubleClick(QListWidgetItem*);
private:
    QLineEdit* m_lePhone;
    QListWidget* m_lwPhones;
};


#endif /* PHONESWIDGET_H */

