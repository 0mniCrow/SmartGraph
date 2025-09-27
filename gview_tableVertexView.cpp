#include "gview_tableVertexView.h"

VertexList::VertexList(SelectedRow *styleDelegate, QWidget* tata):
    QTableView(tata)
{
    setItemDelegate(styleDelegate);
    connect(this,&QObject::destroyed,styleDelegate,&QObject::deleteLater);
    //QItemSelectionModel * cur_select_model = selectionModel();
    //setSelectConn();
    return;
}

void VertexList::setSelectConn()
{
    connect(selectionModel(),&QItemSelectionModel::currentRowChanged,this,&VertexList::newSelection);
    return;
}

VertexModel* VertexList::vertexModel()
{
    return dynamic_cast<VertexModel*>(model());
}

SelectedRow* VertexList::selectedRowDelegate()
{
    return dynamic_cast<SelectedRow*>(itemDelegate());
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
    selectedRowDelegate()->clearDragAction();
    QTableView::dropEvent(d_event);
    QPointF tyk = d_event->position();
    QModelIndex index = indexAt(tyk.toPoint());
    if(index.isValid())
    {
        selectionModel()->clearSelection();
        selectionModel()->select(index,QItemSelectionModel::Select|
                                 QItemSelectionModel::Rows);
        setCurrentIndex(index);
        setFocus();
    }
    //selectionModel()->select()
    //setSelection(d_event->position(),QItemSelectionModel::Select);
    return;
}

void VertexList::startDrag(Qt::DropActions supportedActions)
{
    QModelIndex index = currentIndex();
    if(index.isValid())
    {
        vertexModel()->setDraggedRow(index.row());
        selectedRowDelegate()->setDragAction(true);
    }
    QTableView::startDrag(supportedActions);

    return;
}

void VertexList::changeSelection(const QModelIndex& index)
{
    selectionModel()->clearSelection();
    if(index.isValid())
    {
        selectionModel()->select(index,QItemSelectionModel::Select|
                                 QItemSelectionModel::Rows);
        setCurrentIndex(index);
        //setFocus();
    }
    return;
}

void VertexList::outsideNewSelect(GViewItem* selected_item)
{
    if(selected_item &&
            vertexModel()->contains(selected_item))
    {
        int row = vertexModel()->rowIndex(selected_item);
        if(row<0)
        {
            return;
        }
        changeSelection(vertexModel()->index(row,0));
    }
    else
    {
        changeSelection(QModelIndex());
    }
    return;
}

void VertexList::newSelection(const QModelIndex& newObject,
                              const QModelIndex& prevObject)
{
    Q_UNUSED(prevObject);
    if(!newObject.isValid())
    {
        return;
    }
    GViewItem * selected_item = vertexModel()->at(newObject.row());
    if(selected_item)
    {
        emit listNewSelect(selected_item);
    }
    return;
}

///____________________Spacing delegate_______________________________

SelectedRow::SelectedRow(QObject* tata):
    QStyledItemDelegate(tata),_drag_action_(false)
{
    return;
}

void SelectedRow::paint(QPainter* painter, const QStyleOptionViewItem& opt, const QModelIndex& index) const
{
    if(_drag_action_&&
            opt.state& QStyle::State_Selected &&
            !(opt.state& QStyle::State_MouseOver))
    {
        painter->save();
        painter->setPen(QPen(Qt::black,1,Qt::SolidLine,Qt::SquareCap,Qt::BevelJoin));
        painter->setBrush(QColor(248,255,182));
        painter->drawRect(opt.rect);
        QString data = index.data().toString();
        painter->drawText(opt.rect,data,Qt::AlignLeft|Qt::AlignVCenter);
        painter->restore();
    }
    else
    {
        QStyledItemDelegate::paint(painter,opt,index);
    }
    return;
}

void SelectedRow::setDragAction(bool drag_action)
{
    _drag_action_ = drag_action;
    return;
}
void SelectedRow::clearDragAction()
{
    _drag_action_ = false;
    return;
}
