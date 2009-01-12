#ifndef PIMMAINWINDOW_H
#define PIMMAINWINDOW_H

#include <QAction>
class PimPluginAction
    : public QAction
{
    Q_OBJECT
public:
    PimPluginAction(const QIcon&, const QString&, QObject*, int uid);
    virtual ~PimPluginAction();
signals:
    void triggered(int);
private slots:
    void onTrigger();
private:
    int m_pluginUid;
};

#include <QMainWindow>
#include <QSettings>
#include <QVector>

class QStatusBar;
class QAction;
class PimGroupsTreeWidget;
class PimRecordsListWidget;
class QTreeWidgetItem;
class QListWidgetItem;
class QLabel;
class QTextEdit;

class PimMainWindow
    : public QMainWindow
{
    Q_OBJECT
public:
    PimMainWindow();
    virtual ~PimMainWindow();
private slots:
    void onNewFile();
    void onOpenFile();
    void onQuit();
    void onAbout();
    void onAboutQt();
    void onGroupPopup(const QPoint&);
    void onRecordsPopup(const QPoint&);
    void onSelectGroup(QTreeWidgetItem*, int);
    void onSelectRecord(QListWidgetItem*);
    void onNewGroup();
    void onEditGroup();
    void onDeleteGroup();
    void onNewRecord(int);
    void onEditRecord();
    void onDeleteRecord();
private:
    void enableActions(bool);
    void createActions();
    void createMenuBar();

    void createGroups();
    void loadGroups(int parent, QTreeWidgetItem* root);
    
    QSettings m_settings;
    QStatusBar* m_sb;
    PimGroupsTreeWidget* m_twGroups;
    PimRecordsListWidget* m_lwRecords;
    QLabel* m_lRecordsCaption;
    QTextEdit* m_teSummary;
// actions
    QAction* aNewFile;
    QAction* aOpenFile;
    QAction* aQuit;
    QAction* aAbout;
    QAction* aAboutQt;
    QAction* aNewGroup;
    QAction* aEditGroup;
    QAction* aDeleteGroup;
    QVector<PimPluginAction*> aNewRecords;
    QAction* aEditRecord;
    QAction* aDeleteRecord;
};

#endif /* PIMMAINWINDOW_H */

