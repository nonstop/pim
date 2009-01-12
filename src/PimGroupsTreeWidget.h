#ifndef PIMGROUPSTREEWIDGET_H
#define PIMGROUPSTREEWIDGET_H

#include <QTreeWidget>

class PimGroupsTreeWidget
    : public QTreeWidget
{
    Q_OBJECT
public:
    PimGroupsTreeWidget();
    virtual ~PimGroupsTreeWidget();

    QString itemPath(QTreeWidgetItem*);
protected:
    virtual void dropEvent(QDropEvent*);


};

#endif /* PIMGROUPSTREEWIDGET_H */

