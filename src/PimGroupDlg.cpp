#include <QtGui>
#include "PimGroupDlg.h"

PimGroupDlg::PimGroupDlg(QWidget* parent)
    : QDialog(parent)
{
    createWidgets();
    setWindowTitle(tr("Add group"));
}

PimGroupDlg::PimGroupDlg(QWidget*, const QString& name, const QString& notes)
{
    createWidgets();
    m_leGroupName->setText(name);
    m_leGroupNotes->setText(notes);

    setWindowTitle(tr("Edit group"));
}

void PimGroupDlg::createWidgets()
{
    QVBoxLayout* layout = new QVBoxLayout;
    QHBoxLayout* hbox = new QHBoxLayout;
    QLabel* lGroupName = new QLabel(tr("Group name:"));
    m_leGroupName = new QLineEdit;
    hbox->addWidget(lGroupName);
    hbox->addWidget(m_leGroupName, 1);
    layout->addLayout(hbox);
    QLabel* lGroupNotes = new QLabel(tr("Notes"));
    layout->addWidget(lGroupNotes);
    m_leGroupNotes = new QLineEdit;
    layout->addWidget(m_leGroupNotes);

    QHBoxLayout* btns = new QHBoxLayout;
    QPushButton *bOk = new QPushButton(tr("OK"));
    QPushButton *bCancel = new QPushButton(tr("Cancel"));
    btns->addStretch(1);
    btns->addWidget(bOk);
    btns->addWidget(bCancel);
    connect(bOk, SIGNAL(clicked()), this, SLOT(onOk()));
    connect(bCancel, SIGNAL(clicked()), this, SLOT(reject()));
    layout->addStretch(1);
    layout->addLayout(btns);
    setLayout(layout);
}

void PimGroupDlg::onOk()
{
    if (!m_leGroupName->text().length())
    {
        QMessageBox::warning(this, tr("Error"), tr("Group name empty"), QMessageBox::Ok);
        return;
    }
    accept();

}

QString PimGroupDlg::groupName() const
{
    return m_leGroupName->text();
}

QString PimGroupDlg::groupNotes() const
{
    return m_leGroupNotes->text();
}


