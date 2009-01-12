#include <QtGui>

#include "ContactDlg.h"


ContactDlg::ContactDlg(QWidget* parent)
    : QDialog(parent)
{
    createWidgets();
    setWindowTitle(tr("Add contact"));
}

ContactDlg::ContactDlg(QWidget* parent, int /*id*/)
    : QDialog(parent)
{
    createWidgets();
    setWindowTitle(tr("Edit contact"));
}

ContactDlg::~ContactDlg()
{
}

void ContactDlg::onOk()
{
    if (!m_leFirstName->text().length() 
        && !m_leSecondName->text().length()
        && !m_leFamilyName->text().length())
    {
        QMessageBox::warning(this, tr("Error"), tr("Not valid data"), QMessageBox::Ok);
        return;
    }
    
    accept();
}

void ContactDlg::setFirstName(const QString& txt)
{
    m_leFirstName->setText(txt);
}

void ContactDlg::setSecondName(const QString& txt)
{
    m_leSecondName->setText(txt);
}

void ContactDlg::setFamilyName(const QString& txt)
{
    m_leFamilyName->setText(txt);
}

void ContactDlg::setMail(const QString& txt)
{
    m_leMail->setText(txt);
}

void ContactDlg::setAddress(const QString& txt)
{
    m_teAddress->document()->setPlainText(txt);
}

void ContactDlg::setNotes(const QString& txt)
{
    m_teNotes->document()->setPlainText(txt);
}

QString ContactDlg::firstName() const
{
    return m_leFirstName->text();
}

QString ContactDlg::secondName() const
{
    return m_leSecondName->text();
}

QString ContactDlg::familyName() const
{
    return m_leFamilyName->text();
}

QString ContactDlg::mail() const
{
    return m_leMail->text();
}

QString ContactDlg::address() const
{
    return m_teAddress->document()->toPlainText();
}

QString ContactDlg::notes() const
{
    return m_teNotes->toPlainText();
}

void ContactDlg::createWidgets()
{
    QVBoxLayout* mainLayout = new QVBoxLayout;
    QTabWidget* tab = new QTabWidget;
    
    QWidget* generalTab = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout;
    QGridLayout* grid = new QGridLayout;
    QHBoxLayout* hbox = NULL;

    m_leFirstName = new QLineEdit;
    grid->addWidget(new QLabel(tr("First name:")), 0, 0);
    grid->addWidget(m_leFirstName, 0, 1);
    
    m_leSecondName = new QLineEdit;
    grid->addWidget(new QLabel(tr("Second name:")), 1, 0);
    grid->addWidget(m_leSecondName, 1, 1);

    m_leFamilyName = new QLineEdit;
    grid->addWidget(new QLabel(tr("Family name:")), 2, 0);
    grid->addWidget(m_leFamilyName, 2, 1);

    m_leMail = new QLineEdit;
    grid->addWidget(new QLabel(tr("Mail:")), 3, 0);
    grid->addWidget(m_leMail, 3, 1);

    layout->addLayout(grid);
    m_teAddress = new QTextEdit;
    layout->addWidget(new QLabel(tr("Address:")));
    layout->addWidget(m_teAddress, 1);

    generalTab->setLayout(layout);
    tab->addTab(generalTab, tr("General"));

    m_teNotes = new QTextEdit;

    tab->addTab(m_teNotes, tr("Notes"));
    
    mainLayout->addWidget(tab, 1);

    hbox = new QHBoxLayout;
    QPushButton *bOk = new QPushButton(tr("OK"));
    QPushButton *bCancel = new QPushButton(tr("Cancel"));
    hbox->addStretch(1);
    hbox->addWidget(bOk);
    hbox->addWidget(bCancel);
    connect(bOk, SIGNAL(clicked()), this, SLOT(onOk()));
    connect(bCancel, SIGNAL(clicked()), this, SLOT(reject()));
    mainLayout->addLayout(hbox);

    setLayout(mainLayout);

    setSizeGripEnabled(true);
}

