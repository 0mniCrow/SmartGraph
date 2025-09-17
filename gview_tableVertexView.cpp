#include "gview_tableVertexView.h"

VertexList::VertexList(SpacingDelegate* delegate, QWidget* tata):
    QTableView(tata),_delegate_(delegate)
{
//    _def_row_height_ = 0;
//    if(delegate)
//    {
//        setItemDelegate(delegate);
//    }
    return;
}
void VertexList::setSpacingDelegate(SpacingDelegate* delegate)
{
//    if(delegate)
//    {
//        setItemDelegate(_delegate_=delegate);
//    }
    return;
}

PhantomRowProxyModel* VertexList::getProxyModel()
{
    return dynamic_cast<PhantomRowProxyModel*>(model());
}

//bool VertexList::event(QEvent* event)
//{
//    qDebug()<<"Event type: " << event->type();
//    return QTableView::event(event);
//}

void VertexList::dragMoveEvent(QDragMoveEvent* d_event)
{ 
    QModelIndex index(indexAt(d_event->position().toPoint()));
    if(index.isValid())
    {
//        _delegate_->setDropRow(index.row());
//        _delegate_->setDragActive(true);
//        if(!_def_row_height_)
//        {
//            _def_row_height_ = rowHeight(index.row());
//            _cur_index_ = index;
//            setRowHeight(index.row(),_def_row_height_+40);
//        }
//        int dragged_row = getProxyModel()->getSourceModel()->extractMimeData(d_event->mimeData());
//        if(index.row()!=dragged_row)
//        {

//        }
        auto pr_mod = getProxyModel();
        if(pr_mod->isPhantomRowSet())
        {
            pr_mod->setPhantomRow(getProxyModel()->getSourceModel()->extractMimeData(d_event->mimeData()));
        }
        viewport()->update();
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
//    _delegate_->setDragActive(false);
//    if(_def_row_height_)
//    {
//        setRowHeight(_cur_index_.row(),_def_row_height_);
//        _def_row_height_ = 0;
//    }
    getProxyModel()->clearPhantomRow();
    viewport()->update();
    QTableView::dropEvent(d_event);
    return;
}

void VertexList::startDrag(Qt::DropActions supportedActions)
{
    QModelIndex index = currentIndex();
    if(index.isValid())
    {
        //model()->setDraggedRow(index.row());
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
