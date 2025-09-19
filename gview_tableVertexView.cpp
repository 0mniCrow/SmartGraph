#include "gview_tableVertexView.h"

VertexList::VertexList(QWidget* tata):
    QTableView(tata)
{
    return;
}

VertexModel* VertexList::vertexModel()
{
    return dynamic_cast<VertexModel*>(model());
}

void VertexList::dragMoveEvent(QDragMoveEvent* d_event)
{ 
    QModelIndex index(indexAt(d_event->position().toPoint()));
    if(index.isValid())
    {
        if(index.row()!=vertexModel()->phantomRow())
        {
            vertexModel()->setPhantomRow(index.row());
        }
    }
    QTableView::dragMoveEvent(d_event);
    return;
}

void VertexList::dragLeaveEvent(QDragLeaveEvent* event)
{
    QTableView::dragLeaveEvent(event);
    return;
}

void VertexList::dropEvent(QDropEvent* d_event)
{
    vertexModel()->clearPhantomRow();
    vertexModel()->clearDraggedRow();
    QTableView::dropEvent(d_event);
    return;
}

void VertexList::startDrag(Qt::DropActions supportedActions)
{
    QModelIndex index = currentIndex();
    if(index.isValid())
    {
        vertexModel()->setDraggedRow(index.row());
    }
    QTableView::startDrag(supportedActions);
    return;
}

///____________________Spacing delegate_______________________________

SpacingDelegate::SpacingDelegate(int space_size, QObject* tata):
    QStyledItemDelegate(tata),_space_size_(space_size),
_drop_row_(-1),_drag_move_active_(false){}

void SpacingDelegate::setDropRow(int drop_row)
{
    _drop_row_ = drop_row;
    return;
}
void SpacingDelegate::setDragActive(bool active)
{
    _drag_move_active_=active;
    return;
}

QSize SpacingDelegate::sizeHint(const QStyleOptionViewItem& opt, const QModelIndex& index) const
{
    if(_drag_move_active_&& index.row()==_drop_row_)
    {
        //QSize hint_size = QStyledItemDelegate::sizeHint(opt,index);
        return QSize(opt.rect.width(),opt.rect.height()+_space_size_);
    }
    return QStyledItemDelegate::sizeHint(opt,index);
}

void SpacingDelegate::paint(QPainter* painter, const QStyleOptionViewItem& opt, const QModelIndex& index) const
{
    QStyleOptionViewItem option = opt;
    if(_drag_move_active_&& index.row()==_drop_row_)
    {
        painter->fillRect(option.rect,QColor(100,149,237));
    }
    QStyledItemDelegate::paint(painter,option,index);
}
