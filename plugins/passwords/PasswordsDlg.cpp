#include <QtGui>

#include "PasswordsDlg.h"


PasswordsDlg::PasswordsDlg(QWidget* parent)
    : QDialog(parent)
{
    createWidgets();
    setWindowTitle(tr("Add password"));
}

PasswordsDlg::PasswordsDlg(QWidget* parent, int /*id*/)
    : QDialog(parent)
{
    createWidgets();
    setWindowTitle(tr("Edit password"));
}

PasswordsDlg::~PasswordsDlg()
{
}

void PasswordsDlg::onOk()
{
    if (!m_leService->text().length())
    {
        QMessageBox::warning(this, tr("Error"), tr("Service empty"), QMessageBox::Ok);
        return;
    }
    if (!m_leLogin->text().length())
    {
        QMessageBox::warning(this, tr("Error"), tr("Login empty"), QMessageBox::Ok);
        return;
    }
    if (!m_lePassword->text().length())
    {
        QMessageBox::warning(this, tr("Error"), tr("Password empty"), QMessageBox::Ok);
        return;
    }
    
    accept();
}

void PasswordsDlg::showHidePassword()
{
    static bool visible = false;
    if (visible)
        m_lePassword->setEchoMode(QLineEdit::Normal);
    else
        m_lePassword->setEchoMode(QLineEdit::Password);
    visible = !visible;
}

void PasswordsDlg::setService(const QString& txt)
{
    m_leService->setText(txt);
}

void PasswordsDlg::setLogin(const QString& txt)
{
    m_leLogin->setText(txt);
}

void PasswordsDlg::setPassword(const QString& txt)
{
    m_lePassword->setText(txt);
}

void PasswordsDlg::setMail(const QString& txt)
{
    m_leMail->setText(txt);
}

void PasswordsDlg::setUrl(const QString& txt)
{
    m_leUrl->setText(txt);
}

void PasswordsDlg::setNotes(const QString& txt)
{
    m_teNotes->document()->setPlainText(txt);
}

QString PasswordsDlg::service() const
{
    return m_leService->text();
}

QString PasswordsDlg::login() const
{
    return m_leLogin->text();
}

QString PasswordsDlg::password() const
{
    return m_lePassword->text();
}

QString PasswordsDlg::mail() const
{
    return m_leMail->text();
}

QString PasswordsDlg::url() const
{
    return m_leUrl->text();
}

QString PasswordsDlg::notes() const
{
    return m_teNotes->toPlainText();
}

void PasswordsDlg::createWidgets()
{
    QVBoxLayout* mainLayout = new QVBoxLayout;
    QTabWidget* tab = new QTabWidget;
    
    QWidget* generalTab = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout;
    QGridLayout* grid = new QGridLayout;
    QHBoxLayout* hbox = NULL;

    m_leService = new QLineEdit;
    grid->addWidget(new QLabel(tr("Service:")), 0, 0);
    grid->addWidget(m_leService, 0, 1);
    
    m_leLogin = new QLineEdit;
    grid->addWidget(new QLabel(tr("Login:")), 1, 0);
    grid->addWidget(m_leLogin, 1, 1);

    m_lePassword = new QLineEdit;
    QPushButton* btn = new QPushButton;
    connect(btn, SIGNAL(clicked()), this, SLOT(showHidePassword()));
    btn->setIcon(QIcon(":showHide"));
    btn->setCheckable(true);
    hbox = new QHBoxLayout;
    hbox->addWidget(m_lePassword, 1);
    hbox->addWidget(btn);
    grid->addWidget(new QLabel(tr("Password:")), 2, 0);
    grid->addLayout(hbox, 2, 1);

    m_leMail = new QLineEdit;
    grid->addWidget(new QLabel(tr("Mail:")), 3, 0);
    grid->addWidget(m_leMail, 3, 1);

    m_leUrl = new QLineEdit;
    grid->addWidget(new QLabel(tr("Url:")), 4, 0);
    grid->addWidget(m_leUrl, 4, 1);

    layout->addLayout(grid);
    layout->addStretch(1);

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

    showHidePassword();
    setSizeGripEnabled(true);
}

