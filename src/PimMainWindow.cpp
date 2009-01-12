#include <QtGui>

#include "PimMainWindow.h"

PimPluginAction::PimPluginAction(const QIcon& icon, const QString& text, QObject* parent, int uid)
    : QAction(icon, text, parent), m_pluginUid(uid)
{
    connect(this, SIGNAL(triggered()), this, SLOT(onTrigger()));
}

PimPluginAction::~PimPluginAction()
{}

void PimPluginAction::onTrigger()
{
    emit triggered(m_pluginUid);
}

#include "PluginManager.h"
#include "PimDatabase.h"
#include "PimGroupDlg.h"
#include "PimGroupsTreeWidget.h"
#include "PimRecordsListWidget.h"

#define DBG qDebug() << __FILE__ << ":" << __LINE__ << " "

/***************
 * construction
 ****************/
PimMainWindow::PimMainWindow()
    : m_settings("qwerty", "Pim")
{
    setWindowTitle("Pim");
    QSplitter* spl = new QSplitter(Qt::Horizontal);
    m_twGroups = new PimGroupsTreeWidget;
    connect(m_twGroups, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onGroupPopup(const QPoint &)));
    connect(m_twGroups, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(onSelectGroup(QTreeWidgetItem*, int)));
    connect(m_twGroups, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(onEditGroup()));   

    spl->addWidget(m_twGroups);
    spl->setStretchFactor(0, 1);
    
    QWidget* recordsWdg = new QWidget;
    QVBoxLayout* vbox = new QVBoxLayout;
    
    m_lRecordsCaption = new QLabel;
    vbox->addWidget(m_lRecordsCaption);
    
    QSplitter* rspl = new QSplitter(Qt::Vertical);

    m_lwRecords = new PimRecordsListWidget;
    connect(m_lwRecords, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onRecordsPopup(const QPoint &)));
    connect(m_lwRecords, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(onEditRecord()));   
    connect(m_lwRecords, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(onSelectRecord(QListWidgetItem*)));
    rspl->addWidget(m_lwRecords);
    rspl->setStretchFactor(0, 5);

    m_teSummary = new QTextEdit;
    m_teSummary->setReadOnly(true);
    rspl->addWidget(m_teSummary);
    rspl->setStretchFactor(1, 1);

    vbox->addWidget(rspl, 1);

    recordsWdg->setLayout(vbox);

    spl->addWidget(recordsWdg);
    spl->setStretchFactor(1, 2);
    setCentralWidget(spl);
    m_sb = new QStatusBar;
    setStatusBar(m_sb);
    
    createActions();
    createMenuBar();
    enableActions(false);
    
    QPoint pos = m_settings.value("PimMainWindow_Pos", QPoint(30, 60)).toPoint();
    QSize size = m_settings.value("PimMainWindow_Size", QSize(600, 400)).toSize();
    resize(size);
    move(pos);

}

PimMainWindow::~PimMainWindow()
{
    m_settings.setValue("PimMainWindow_Pos", this->pos());
    m_settings.setValue("PimMainWindow_Size", this->size());
}

void PimMainWindow::createActions()
{
    aNewFile = new QAction(QIcon(":new"), tr("New file"), this);
    aNewFile->setShortcut(QString("Ctrl+N"));
    connect(aNewFile, SIGNAL(triggered()), this, SLOT(onNewFile()));

    aOpenFile = new QAction(QIcon(":open"), tr("Open file"), this);
    aOpenFile->setShortcut(QString("Ctrl+O"));
    connect(aOpenFile, SIGNAL(triggered()), this, SLOT(onOpenFile()));
    
    aQuit = new QAction(QIcon(":quit"), tr("Quit"), this);
    connect(aQuit, SIGNAL(triggered()), this, SLOT(onQuit()));
    
    aAbout = new QAction(tr("About"), this);
    connect(aAbout, SIGNAL(triggered()), this, SLOT(onAbout()));

    aAboutQt = new QAction(tr("About Qt"), this);
    connect(aAboutQt, SIGNAL(triggered()), this, SLOT(onAboutQt()));
    
    aNewGroup = new QAction(QIcon(":newGroup"), tr("New group"), this);
    connect(aNewGroup, SIGNAL(triggered()), this, SLOT(onNewGroup()));
    
    aEditGroup = new QAction(QIcon(":editGroup"), tr("Edit group"), this);
    connect(aEditGroup, SIGNAL(triggered()), this, SLOT(onEditGroup()));
    
    aDeleteGroup = new QAction(QIcon(":deleteGroup"), tr("Delete group"), this);
    connect(aDeleteGroup, SIGNAL(triggered()), this, SLOT(onDeleteGroup()));

    const Plugins& plugins = PluginManager::instance()->plugins();
    for (Plugins::const_iterator it = plugins.begin(); it != plugins.end(); ++it)
    {
        PimPluginAction* aNewRecord = new PimPluginAction(QIcon(":newRecord"), it->plugin->name(), this, it->uid);
        connect(aNewRecord, SIGNAL(triggered(int)), this, SLOT(onNewRecord(int)));
        aNewRecords.push_back(aNewRecord);
    }

    aEditRecord = new QAction(QIcon(":editRecord"), tr("Edit record"), this);
    connect(aEditRecord, SIGNAL(triggered()), this, SLOT(onEditRecord()));
    
    aDeleteRecord = new QAction(QIcon(":deleteRecord"), tr("Delete record"), this);
    connect(aDeleteRecord, SIGNAL(triggered()), this, SLOT(onDeleteRecord()));
}

void PimMainWindow::createMenuBar()
{
    QMenu* menu = menuBar()->addMenu(tr("File"));
    menu->addAction(aNewFile);
    menu->addAction(aOpenFile);
    menu->addSeparator();
    menu->addAction(aQuit);

    menu = menuBar()->addMenu(tr("Edit"));
    menu->addAction(aNewGroup);
    menu->addAction(aEditGroup);
    menu->addAction(aDeleteGroup);
    menu->addSeparator();
    QMenu* newRecord = menu->addMenu(tr("New record"));
    for (QVector<PimPluginAction*>::iterator it = aNewRecords.begin(); 
        it != aNewRecords.end(); ++it)
        newRecord->addAction(*it);
    menu->addAction(aEditRecord);
    menu->addAction(aDeleteRecord);
    
    menu = menuBar()->addMenu(tr("Help"));
    menu->addAction(aAbout);
    menu->addAction(aAboutQt);
}

/**********
 * slots
 ***********/
void PimMainWindow::onNewFile()
{
    bool ok;
    QString path = QInputDialog::getText(this, tr("New file"),
                                         tr("File name:"), QLineEdit::Normal, "", &ok);
    if (!ok)
        return;
    if (ok && path.isEmpty())
        return;
    
    if (QFile::exists(path))
    {
        if (QMessageBox::question(this, tr("New file"), 
                tr("File exists. Delete file?"),
                QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
            return;
        QFile::remove(path);
        PimDatabase::instance()->create(path);
    }

    PimDatabase::instance()->create(path);
    createGroups();
    enableActions(true);
}

void PimMainWindow::onOpenFile()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Select file"));
    if (!path.length())
        return;
    if (!QFile::exists(path))
    {
        if (QMessageBox::question(this, tr("Select file"), 
                tr("File doesn't exists. Create file?"),
                QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
            return;
        PimDatabase::instance()->create(path);
    }
    else
        PimDatabase::instance()->open(path);
    
    delete m_twGroups->topLevelItem(0);
    loadGroups(-1, NULL);
    enableActions(true);
    onSelectGroup(m_twGroups->topLevelItem(0), 0);
}

void PimMainWindow::onQuit()
{
    PimDatabase::instance()->close();
    close();
}

void PimMainWindow::onAbout()
{
    DBG << __FUNCTION__;
}

void PimMainWindow::onAboutQt()
{
    QMessageBox::aboutQt(this, tr("About Qt"));
}

void PimMainWindow::onRecordsPopup(const QPoint& pos)
{
    QMenu menu(this);

    QMenu* newRecord = menu.addMenu(tr("New record"));
    for (QVector<PimPluginAction*>::iterator it = aNewRecords.begin(); 
        it != aNewRecords.end(); ++it)
        newRecord->addAction(*it);

    menu.addAction(aEditRecord);
    menu.addAction(aDeleteRecord);
    
    menu.exec(m_lwRecords->mapToGlobal(pos));
}

void PimMainWindow::onGroupPopup(const QPoint& pos)
{
    QMenu menu(this);
    menu.addAction(aNewFile);
    menu.addAction(aOpenFile);
    menu.addSeparator();
    menu.addAction(aNewGroup);
    menu.addAction(aEditGroup);
    menu.addAction(aDeleteGroup);
    menu.addSeparator();
    menu.addAction(aQuit);

    menu.exec(m_twGroups->mapToGlobal(pos));
}

void PimMainWindow::onSelectGroup(QTreeWidgetItem* item, int)
{
    if (!item)
        return;

    int idGroup = item->data(0, Qt::UserRole).toInt();
    Records recs;
    PimDatabase::instance()->loadRecords(idGroup, recs);
    DBG << "loaded " << recs.size() << " records for group: " << idGroup;
    
    m_lwRecords->clear();
    for (Records::iterator it = recs.begin(); it != recs.end(); ++it)
    {
        PimPluginInterface* pi = PluginManager::instance()->plugin(it->type);
        if (!pi)
            continue;
        QString name = pi->recordDisplayName(it->id);
        
        QListWidgetItem* item = new QListWidgetItem(name);
        item->setData(Qt::UserRole, it->id);
        item->setIcon(pi->icon());
        m_lwRecords->addItem(item);
    }
    QString caption = m_twGroups->itemPath(item);
    m_lRecordsCaption->setText(caption);
}

void PimMainWindow::onSelectRecord(QListWidgetItem* item)
{
    if (!item)
        return;
    Record rec;
    PimDatabase::instance()->loadRecord(item->data(Qt::UserRole).toInt(), rec);
    QString summary = PluginManager::instance()->plugin(rec.type)->recordSummary(rec.id);
    m_teSummary->document()->setPlainText(summary);
}

void PimMainWindow::onNewGroup()
{
    QTreeWidgetItem* item = m_twGroups->currentItem();
    if (!item)
        item = m_twGroups->topLevelItem(0);

    PimGroupDlg dlg(this);
    if (dlg.exec() != QDialog::Accepted)
        return;
    
    Group group;
    group.name = dlg.groupName();
    group.notes = dlg.groupNotes();
    group.group = item->data(0, Qt::UserRole).toInt();
    PimDatabase::instance()->newGroup(group);
    DBG << "created new group: " << group;
    
    QTreeWidgetItem* newItem = new QTreeWidgetItem((QTreeWidgetItem*)NULL, QStringList(group.name), 1);
    newItem->setData(0, Qt::UserRole, group.id);
    newItem->setIcon(0, QIcon(":closeGroup"));
    item->addChild(newItem);
}

void PimMainWindow::onEditGroup()
{
    QTreeWidgetItem* item = m_twGroups->currentItem();
    if (!item)
        return;
    Group group;
    PimDatabase::instance()->loadGroup(item->data(0, Qt::UserRole).toInt(), group);

    PimGroupDlg dlg(this, group.name, group.notes);
    if (dlg.exec() != QDialog::Accepted)
        return;
    group.name = dlg.groupName();
    group.notes = dlg.groupNotes();
    PimDatabase::instance()->saveGroup(group);
    PimDatabase::instance()->commit();
    item->setText(0, group.name);
    DBG << "saved group: " << group;
}

void PimMainWindow::onDeleteGroup()
{
    QTreeWidgetItem* item = m_twGroups->currentItem();
    if (!item)
        return;
    if (item == m_twGroups->topLevelItem(0))
        return;
    if (QMessageBox::question(this, 
        tr("Delete group"),
        tr("Are you sure?"), 
        QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
        return;
    Group gr;
    PimDatabase::instance()->loadGroup(item->data(0, Qt::UserRole).toInt(), gr);
    PimDatabase::instance()->deleteGroup(gr);
    
    QList<QTreeWidgetItem*> children = item->takeChildren();
    item->parent()->addChildren(children);
    delete item;
}

void PimMainWindow::onNewRecord(int uid)
{
    QTreeWidgetItem* item = m_twGroups->currentItem();
    if (!item)
        item = m_twGroups->topLevelItem(0);

    PimDatabase::instance()->transaction();
    Record r;
    r.group = item->data(0, Qt::UserRole).toInt();
    r.type = uid;
    PimDatabase::instance()->newRecord(r);
    PimPluginInterface* pi = PluginManager::instance()->plugin(r.type);
    if (!pi)
        return;
    if (!pi->newRecord(this, r.id))
    {
        PimDatabase::instance()->rollback();
        return;
    }
    
    PimDatabase::instance()->commit();

    QListWidgetItem* newItem = new QListWidgetItem(pi->recordDisplayName(r.id));
    newItem->setData(Qt::UserRole, r.id);
    newItem->setIcon(pi->icon());
    m_lwRecords->addItem(newItem);

    DBG << "created new record:" << r;
}

void PimMainWindow::onEditRecord()
{
    QListWidgetItem* item = m_lwRecords->currentItem();
    if (!item)
        return;

    PimDatabase::instance()->transaction();
    Record rec;
    rec.id = item->data(Qt::UserRole).toInt();
    PimDatabase::instance()->loadRecord(rec.id, rec);
    PimPluginInterface* pi = PluginManager::instance()->plugin(rec.type);
    if (!pi)
        return;
    if (!pi->editRecord(this, rec.id))
    {
        PimDatabase::instance()->rollback();
        return;
    }
    item->setText(pi->recordDisplayName(rec.id));
    PimDatabase::instance()->commit();
    DBG << "saved record: " << rec;
}

void PimMainWindow::onDeleteRecord()
{
    QListWidgetItem* item = m_lwRecords->currentItem();
    if (!item)
        return;
    if (QMessageBox::question(this, 
        tr("Delete record"),
        tr("Are you sure?"), 
        QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
        return;
    int idRec = item->data(Qt::UserRole).toInt();
    PimDatabase::instance()->deleteRecord(idRec);
    delete item;
    DBG << "deleted record: " << idRec;
}

/****************
 * other methods
 *****************/
void PimMainWindow::enableActions(bool status)
{
    aNewGroup->setEnabled(status);
    aEditGroup->setEnabled(status);
    aDeleteGroup->setEnabled(status);

    for (QVector<PimPluginAction*>::iterator it = aNewRecords.begin(); 
        it != aNewRecords.end(); ++it)
        (*it)->setEnabled(status);

    aEditRecord->setEnabled(status);
    aDeleteRecord->setEnabled(status);
}

void PimMainWindow::createGroups()
{
    Group g;
    g.name = tr("Personal information");
    g.group = -1;
    PimDatabase::instance()->newGroup(g);

    Plugins pl = PluginManager::instance()->plugins();
    for (Plugins::const_iterator it = pl.begin(); it != pl.end(); ++it)
        it->plugin->init();

    loadGroups(-1, NULL);
    onSelectGroup(m_twGroups->topLevelItem(0), 0);
}

void PimMainWindow::loadGroups(int group, QTreeWidgetItem* root)
{
    Groups groups;
    PimDatabase::instance()->loadGroups(groups, group);
    for (GroupsConstIter it = groups.begin(); it != groups.end(); ++it)
    {
        QTreeWidgetItem* item = new QTreeWidgetItem((QTreeWidgetItem*)NULL, QStringList(it->name), 1);
        item->setData(0, Qt::UserRole, it->id);
        item->setIcon(0, QIcon(":closeGroup"));
        
        if (!root)
            m_twGroups->addTopLevelItem(item);
        else
            root->addChild(item);
        loadGroups(it->id, item);
        item->setExpanded(true);
    }

}

