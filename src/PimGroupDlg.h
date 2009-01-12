#ifndef PIMGROUPDLG_H
#define PIMGROUPDLG_H

#include <QDialog>

class QLineEdit;

class PimGroupDlg
    : public QDialog
{
    Q_OBJECT
public:
    PimGroupDlg(QWidget*);
    PimGroupDlg(QWidget*, const QString& name, const QString& notes);
    virtual ~PimGroupDlg()
    {}
    QString groupName() const;
    QString groupNotes() const;
private slots:
    void onOk();
private:
    void createWidgets();

    QLineEdit* m_leGroupName;
    QLineEdit* m_leGroupNotes;
};


#endif /* PIMGROUPDLG_H */

