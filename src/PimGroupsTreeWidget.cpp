#include <QtGui>
#include <QDebug>

#include "PimDatabase.h"
#include "PimGroupsTreeWidget.h"

PimGroupsTreeWidget::PimGroupsTreeWidget()
{
    setColumnCount(1);

    QStringList labels;
    labels << tr("Groups");
    setHeaderLabels(labels);
    setContextMenuPolicy(Qt::CustomContextMenu);

    setAcceptDrops(true);
    setDragEnabled(true);
    setDragDropMode(QAbstractItemView::InternalMove);
    setDropIndicatorShown(true);
}

PimGroupsTreeWidget::~PimGroupsTreeWidget()
{
}

void PimGroupsTreeWidget::dropEvent(QDropEvent* event)
{
    QTreeWidgetItem* itemDragged = currentItem();
    if (itemDragged == topLevelItem(0))
        return;
    QTreeWidgetItem* itemDest = itemAt(event->pos());
    if (!itemDest)
        return;
   
    Group gr;
    PimDatabase::instance()->loadGroup(itemDragged->data(0, Qt::UserRole).toInt(), gr);
    gr.group = itemDest->data(0, Qt::UserRole).toInt();
    PimDatabase::instance()->saveGroup(gr);
    QTreeWidget::dropEvent(event);
}

QString PimGroupsTreeWidget::itemPath(QTreeWidgetItem* item)
{
    if (!item)
        return "";
    QString res;
    while (item)
    {
        res = " / " + res;
        res = item->text(0) + res;
        item = item->parent();
    }
    res = res.left(res.length() - 3);
    return res;
}

