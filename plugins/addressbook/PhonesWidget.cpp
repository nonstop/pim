#include <QtGui>
#include <QDebug>

#include "PhonesWidget.h"

PhonesWidget::PhonesWidget()
{
    QVBoxLayout* mainLayout = new QVBoxLayout;
    
    QHBoxLayout* hbox = new QHBoxLayout;
    m_lwPhones = new QListWidget;
    m_lwPhones->clear();
    connect(m_lwPhones, SIGNAL(itemDoubleClicked(QListWidgetItem*)), 
        this, SLOT(onDoubleClick(QListWidgetItem*)));
    hbox->addWidget(m_lwPhones, 1);
    QPushButton* bDelete = new QPushButton;
    bDelete->setIcon(QIcon(":delPhone"));
    bDelete->setToolTip(tr("Delete phone number"));
    connect(bDelete, SIGNAL(clicked()), this, SLOT(onDeletePhone()));
    QVBoxLayout* vbox = new QVBoxLayout;
    vbox->addWidget(bDelete);
    vbox->addStretch();
    hbox->addLayout(vbox);
    mainLayout->addLayout(hbox);
    
    hbox = new QHBoxLayout;
    hbox->addWidget(new QLabel(tr("Phone number:")));
    m_lePhone = new QLineEdit;
    hbox->addWidget(m_lePhone, 1);
    QPushButton* bAdd = new QPushButton;
    bAdd->setIcon(QIcon(":addPhone"));
    bAdd->setToolTip(tr("Add phone number"));
    connect(bAdd, SIGNAL(clicked()), this, SLOT(onAddPhone()));
    hbox->addWidget(bAdd);
    mainLayout->addLayout(hbox);

    setLayout(mainLayout);
}

PhonesWidget::~PhonesWidget()
{
}

void PhonesWidget::setPhones(const Phones& phones)
{
    m_lwPhones->clear();
    for (int i=0; i<phones.size(); ++i)
        m_lwPhones->addItem(new QListWidgetItem(phones[i].number));
}

Phones PhonesWidget::phones() const
{
    Phones phs;
    for (int i=0; i<m_lwPhones->count(); ++i)
        phs.push_back(m_lwPhones->item(i)->text());
    qDebug() << __FILE__ << ":" << __LINE__ << "phones count: " << m_lwPhones->count();
    return phs;
}

void PhonesWidget::onAddPhone()
{
    QString phNum = m_lePhone->text();
    QList<QListWidgetItem*> items = m_lwPhones->findItems(phNum, Qt::MatchExactly);
    if (!items.empty())
        return;

    m_lwPhones->addItem(new QListWidgetItem(phNum));
}

void PhonesWidget::onDeletePhone()
{
    QListWidgetItem* item = m_lwPhones->currentItem();
    if (!item)
        return;
    delete item;
}

void PhonesWidget::onDoubleClick(QListWidgetItem* item)
{
    if (!item)
        return;
    m_lePhone->setText(item->text());
}

