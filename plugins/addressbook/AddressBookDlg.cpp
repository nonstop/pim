#include <QtGui>

#include "PhonesWidget.h"
#include "AddressBookDlg.h"

AddressBookDlg::AddressBookDlg(QWidget* parent)
    : QDialog(parent)
{
    createWidgets();
    setWindowTitle(tr("New address"));
}

AddressBookDlg::AddressBookDlg(QWidget* parent, int)
    : QDialog(parent)
{
    createWidgets();
    setWindowTitle(tr("Edit address"));
}

AddressBookDlg::~AddressBookDlg()
{
}

void AddressBookDlg::createWidgets()
{
    QVBoxLayout* mainLayout = new QVBoxLayout;
    QTabWidget* tw = new QTabWidget;

    QWidget* generalTab = new QWidget;
    
    QVBoxLayout* vbox = new QVBoxLayout;
    QHBoxLayout* hbox = new QHBoxLayout;
    hbox->addWidget(new QLabel(tr("Name:")));
    m_leName = new QLineEdit;
    hbox->addWidget(m_leName);
    vbox->addLayout(hbox);

    vbox->addWidget(new QLabel(tr("Address:")));
    m_teAddress = new QTextEdit;
    vbox->addWidget(m_teAddress);
    
    generalTab->setLayout(vbox);
    tw->addTab(generalTab, tr("General"));
    
    m_phones = new PhonesWidget;
    tw->addTab(m_phones, tr("Phones"));

    m_teNotes = new QTextEdit;
    tw->addTab(m_teNotes, tr("Notes"));

    hbox = new QHBoxLayout;
    QPushButton *bOk = new QPushButton(tr("OK"));
    QPushButton *bCancel = new QPushButton(tr("Cancel"));
    hbox->addStretch(1);
    hbox->addWidget(bOk);
    hbox->addWidget(bCancel);
    connect(bOk, SIGNAL(clicked()), this, SLOT(onOk()));
    connect(bCancel, SIGNAL(clicked()), this, SLOT(reject()));

    mainLayout->addWidget(tw);
    mainLayout->addLayout(hbox);

    setLayout(mainLayout);
}

void AddressBookDlg::onOk()
{
    if (!m_leName->text().length())
    {
        QMessageBox::warning(this, tr("Error"), tr("Name empty"), QMessageBox::Ok);
        return;
    }
    if (!m_teAddress->document()->toPlainText().length())
    {
        QMessageBox::warning(this, tr("Error"), tr("Address empty"), QMessageBox::Ok);
        return;
    }
    accept();
}

QString AddressBookDlg::name() const
{
    return m_leName->text();
}

QString AddressBookDlg::address() const
{
    return m_teAddress->document()->toPlainText();
}

QString AddressBookDlg::notes() const
{
    return m_teNotes->document()->toPlainText();
}

Phones AddressBookDlg::phones() const
{
    return m_phones->phones();
}

void AddressBookDlg::setName(const QString& name)
{
    m_leName->setText(name);
}

void AddressBookDlg::setAddress(const QString& address)
{
    m_teAddress->document()->setPlainText(address);
}

void AddressBookDlg::setNotes(const QString& notes)
{
    m_teNotes->document()->setPlainText(notes);
}

void AddressBookDlg::setPhones(const Phones& phones)
{
    m_phones->setPhones(phones);
}

