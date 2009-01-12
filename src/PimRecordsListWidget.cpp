
#include "PimRecordsListWidget.h"

PimRecordsListWidget::PimRecordsListWidget()
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    setDragDropMode(QAbstractItemView::DragOnly);
}

PimRecordsListWidget::~PimRecordsListWidget()
{
}

