#include "gviewport.h"
/*
 * //todo:
 * Дадаць: сутыкненьне вяршын,
 * стварэньне іконак.
 * Дадаць да вяршын трыманьне відарысаў.
 * Дадаць магчымасьць адсякаць квадрат іконкі.
 *
 *
 * (done) Зрабіць: сьпіс усіх створаных вяршын.
 * Магчыма перацягваць вяршыны,
 * каб ставіць адну за адной, сартыроўка.
*/
GViewPort::GViewPort(int vertex_radius, VertexModel *model, QWidget *tata):
    QGraphicsView(tata),
    _vertices_(model),_vertex_radius_(vertex_radius),_counter_(0)
{
    _new_edge_=  nullptr;
    _del_edge_=  nullptr;
    _selected_vertex_= nullptr;
    return;
}

QGraphicsItem* GViewPort::grabItem(QMouseEvent* m_event)
{
    return scene()->itemAt(mapToScene(m_event->pos()),transform());
}

GViewItem* GViewPort::grabGItem(QMouseEvent* m_event)
{
    QGraphicsItem * g_item = grabItem(m_event);
    if(g_item)
    {
        GViewItem* item = qgraphicsitem_cast<GViewItem*>(g_item);
        if(item)
        {
            return item;
        }
    }
    return nullptr;
}

void GViewPort::addItem(GViewItem *vertex, const QPoint &pos)
{
    if(_mode_!= GPort_add || !vertex)
    {
        return;
    }
    if(_vertices_->contains(vertex))//if(_vertices_.contains(vertex))
    {
        return;
    }
    _vertices_->addItem(vertex);//_vertices_.push_back(vertex);
    scene()->addItem(vertex);
    vertex->setPos(mapToScene(pos));
    setMode(GPort_NoMode);
    return;
}

void GViewPort::deleteItem(GViewItem* vertex)
{
    if(_mode_!= GPort_delete ||
            !vertex)
    {
        return;
    }
    if(vertex->isSelected())
    {
        vertex->setSelected(false);
        _selected_vertex_ = nullptr;
    }
    auto it = _vertices_->find(vertex);
    //QList<GViewItem*>::const_iterator it = std::find(_vertices_.cbegin(),_vertices_.cend(),vertex);
    if(it!=_vertices_->end())//if(it!=_vertices_.end())
    {

        delLinkedEdges(vertex);
        scene()->removeItem(*it);
        delete *it;
        _vertices_->removeItem(*it);
        //_vertices_.erase(it);
        setMode(GPort_NoMode);
    }
    return;
}

void GViewPort::delLinkedEdges(GViewItem*vertex)
{
    if(!vertex)
    {
        return;
    }
    QList<GViewEdge*>::const_iterator it = _edges_.cbegin();
    while(it!=_edges_.cend())
    {
        if((*it)->source()==vertex||(*it)->destination()==vertex)
        {
            (*it)->source()->delEdge(*it);
            (*it)->destination()->delEdge(*it);
            scene()->removeItem(*it);
            delete *it;
            it = _edges_.erase(it);

        }
        else
        {
            it++;
        }
    }
    return;
}

bool GViewPort::addEdge(GViewItem* source, GViewItem* dest, bool directed)
{
    if(!source || !dest)
    {
        return false;
    }

    for(GViewEdge* edge: _edges_)
    {
        if(edge->source()==source&&edge->destination()==dest)
        {
            return false;
        }
        else if(edge->destination()==source&&edge->source()==dest)
        {
            if(edge->isDirected())
            {
                edge->setDirected(false);
                return true;
            }
            else
            {
                return false;
            }
        }
    }
    GViewEdge * new_edge = new GViewEdge(source,dest,_vertex_radius_,directed);
    source->addEdge(new_edge);
    dest->addEdge(new_edge);
    scene()->addItem(new_edge);
    _edges_.push_back(new_edge);
    return true;
}

void GViewPort::startAddEdge(GViewItem* src)
{
    if(_mode_ != GPort_startAddEdge ||
            _new_edge_|| !src)
    {
        return;
    }
    _new_edge_ = new GViewEdge(src,_vertex_radius_,true);
    scene()->addItem(_new_edge_);
    setMouseTracking(true);
    _mode_=GPort_finAddEdge;
    return;
}

void GViewPort::finishAddEdge(GViewItem* dest)
{

    if(_mode_!= GPort_finAddEdge ||
            !_new_edge_ || !dest)
    {
        return;
    }
    if(_new_edge_->source()==dest)
    {
        return;
    }
    setMouseTracking(false);

    for(GViewEdge* edge:_edges_)
    {
        if(edge->source()==_new_edge_->source() &&
                edge->destination() == dest)
        {
            scene()->removeItem(_new_edge_);
            delete _new_edge_;
            _new_edge_ = nullptr;
            setMode(GPort_NoMode);
            return;
        }
        else if(edge->destination()==_new_edge_->source()&&
                edge->source()==dest)
        {
            if(edge->isDirected())
            {
                edge->setDirected(false);
                //edge->destination()->addEdge(edge);
            }
            scene()->removeItem(_new_edge_);
            delete _new_edge_;
            _new_edge_ = nullptr;
            setMode(GPort_NoMode);
            return;
        }
    }
    _new_edge_->source()->addEdge(_new_edge_);
    _new_edge_->setDest(dest);
    _new_edge_->destination()->addEdge(_new_edge_);
    _edges_.push_back(_new_edge_);
    _new_edge_ = nullptr;
    setMode(GPort_NoMode);
    return;
}

void GViewPort::startDelEdge(GViewItem* src)
{
    if(_mode_ != GPort_startDelEdge ||
            _del_edge_||
            _new_edge_||
            !src)
    {
        return;
    }
    _del_edge_ = src;
    _new_edge_ = new GViewEdge(src,_vertex_radius_,true,GViewEdge::GVedge_deletion);
    scene()->addItem(_new_edge_);
    _mode_=GPort_finDelEdge;
    setMouseTracking(true);
    return;
}

void GViewPort::finishDelEdge(GViewItem* dest)
{
    if(_mode_ != GPort_finDelEdge ||
            !_del_edge_||
            !_new_edge_||
            !dest)
    {
        return;
    }
    QList<GViewEdge*>::const_iterator it = _edges_.constBegin();
    while(it!=_edges_.constEnd())
    {
        if((*it)->source()==_del_edge_ && (*it)->destination()==dest)
        {
            break;
        }
        else if((*it)->source()==dest &&
                (*it)->destination()==_del_edge_ &&
                !(*it)->isDirected())
        {
            break;
        }
        it++;
    }
    if(it!=_edges_.constEnd())
    {
        (*it)->source()->delEdge(*it);
        (*it)->destination()->delEdge(*it);
        if(!(*it)->isDirected())
        {
            (*it)->destination()->delEdge(*it);
        }
        scene()->removeItem(*it);
        delete *it;
        _edges_.erase(it);
        _del_edge_ = nullptr;
        scene()->removeItem(_new_edge_);
        delete _new_edge_;
        _new_edge_=nullptr;
        setMouseTracking(false);
        setMode(GPort_NoMode);
    }
    return;
}

void GViewPort::setMode(GPort_Mode mode)
{
    switch(_mode_)
    {
    case GPort_add:
    case GPort_delete:
    {
        QApplication::restoreOverrideCursor();
    }
        break;
    case GPort_finAddEdge:
    {
        if(_new_edge_)
        {
            scene()->removeItem(_new_edge_);
            delete _new_edge_;
            _new_edge_ = nullptr;
        }
    }
        break;
    case GPort_finDelEdge:
    {
        if(_new_edge_)
        {
            scene()->removeItem(_new_edge_);
            delete _new_edge_;
            _new_edge_ = nullptr;
        }
        if(_del_edge_)
        {
            _del_edge_ = nullptr;
        }
    }
        break;
    default:
    {

    }
    }

    switch(mode)
    {
    case GPort_add:
    {
        QPixmap new_cursor_pix("sphere.png");
        QCursor new_cursor(new_cursor_pix.scaled(new_cursor_pix.width()/2,
                                                 new_cursor_pix.height()/2,Qt::KeepAspectRatio));
        QApplication::setOverrideCursor(new_cursor);
    }
        break;
    case GPort_delete:
    {
        QPixmap new_cursor_pix("x_sign.png");
        QCursor new_cursor(new_cursor_pix.scaled(new_cursor_pix.width()/3,
                                                 new_cursor_pix.height()/3,
                                                 Qt::KeepAspectRatio));
        QApplication::setOverrideCursor(new_cursor);
    }
        break;
    default:
    {

    }
    }

    _mode_ = mode;

}

void GViewPort::setRadius(int radius)
{
    _vertex_radius_ = radius;
//    for(GViewItem* vertex: _vertices_)
//    {
//        vertex->setRadius(radius);
//    }
    for(int i = 0; i <_vertices_->size();i++)
    {
        _vertices_->at(i)->setRadius(radius);
    }
    for(GViewEdge* edge: _edges_)
    {
        edge->setVertRadius(radius);
    }
    return;
}


void GViewPort::mouseReleaseEvent(QMouseEvent* m_event)
{
    switch(_mode_)
    {
    case GPort_add:
    {
        GViewItem* item = new GViewItem(_vertex_radius_,"Vertex N"+QString::number(_counter_++),
                                        QColor::fromRgb(
                                            QRandomGenerator::global()->generate()
                                            )
                                        );
        addItem(item,m_event->pos());
        QApplication::restoreOverrideCursor();

    }
        break;
    case GPort_delete:
    {
        if(GViewItem* g_item = grabGItem(m_event))
        {
            deleteItem(g_item);
            QApplication::restoreOverrideCursor();
        }
    }
        break;
    case GPort_startAddEdge:
    {
        if(GViewItem* g_item = grabGItem(m_event))
        {
            startAddEdge(g_item);
        }
    }
        break;
    case GPort_finAddEdge:
    {
        if(GViewItem* g_item = grabGItem(m_event))
        {
            finishAddEdge(g_item);
        }
    }
        break;
    case GPort_startDelEdge:
    {
        if(GViewItem* g_item = grabGItem(m_event))
        {
            startDelEdge(g_item);
        }
    }
        break;
    case GPort_finDelEdge:
    {
        if(GViewItem* g_item = grabGItem(m_event))
        {
            finishDelEdge(g_item);
        }
    }
        break;
    default:
    {
        GViewItem * item = grabGItem(m_event);
        if(item)
        {
            if(item != _selected_vertex_)
            {
                QString selected("Selected Item: ");
                selected.append(item->info());
                emit selectedInfo(selected);
                _selected_vertex_ = item;
            }
        }
        else
        {
            if(_selected_vertex_)
            {
                _selected_vertex_->setSelected(false);
                _selected_vertex_ = nullptr;
            }
        }
    }
    }
    QGraphicsView::mouseReleaseEvent(m_event);
    return;
}

void GViewPort::mouseMoveEvent(QMouseEvent* m_event)
{
    switch(_mode_)
    {
    case GPort_finAddEdge:
    case GPort_finDelEdge:
    {
        if(_new_edge_)
        {
            _new_edge_->searchDestination(mapToScene(m_event->pos()));
        }
    }
        break;
    default:
    {
    }
    }
    QGraphicsView::mouseMoveEvent(m_event);
    return;
}



///____________________________________________________
VertexModel::VertexModel(QObject * tata):QAbstractTableModel(tata)
{
    return;
}
QVariant VertexModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(section)
    Q_UNUSED(orientation)
    Q_UNUSED(role)
    return QVariant();
}

QVariant VertexModel::data(const QModelIndex& index,int role) const
{
    if(!index.isValid())
    {
        return QVariant();
    }
    if(role==Qt::DisplayRole||role==Qt::EditRole)
    {
        if(index.row()<_vertices_.size())
        {
            if(!_vertices_.at(index.row()))
            {
                return QVariant();
            }
            return _vertices_.at(index.row())->info();
        }
    }
    return QVariant();
}
Qt::ItemFlags VertexModel::flags(const QModelIndex& index) const
{
    if(index.isValid())
    {
        return QAbstractTableModel::flags(index)|Qt::ItemIsEditable|Qt::ItemIsEnabled| Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
    }
    return QAbstractTableModel::flags(index)| Qt::ItemIsDropEnabled;
}
int VertexModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return _vertices_.size();
}
int VertexModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return 1;
}
bool VertexModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if(!index.isValid()|| index.row()>=_vertices_.size())
    {
        return false;
    }
    if(role==Qt::EditRole)
    {
        if(!_vertices_.at(index.row()))
        {
            return false;
        }
        _vertices_[index.row()]->setInfo(value.toString());
        emit dataChanged(index,index);
        return true;
    }
    return false;
}

QStringList VertexModel::mimeTypes() const
{
    QStringList list;
    list.append("application/x-movedVertex");
    return list;
}
QMimeData* VertexModel::mimeData(const QModelIndexList& indexes) const
{
    QMimeData* data = new QMimeData();
    QByteArray encoded;
    QDataStream stream(&encoded,QIODevice::WriteOnly);
    for(const QModelIndex& index: indexes)
    {
        if(index.isValid())
        {
            stream<<index.row();
        }
    }
    data->setData("application/x-movedVertex",encoded);
    return data;
}

bool VertexModel::dropMimeData(const QMimeData* data, Qt::DropAction action,
                  int row, int column, const QModelIndex& parent)
{
    Q_UNUSED(parent)
    Q_UNUSED(column)
    if(action == Qt::IgnoreAction)
    {
        return true;
    }
    if(!data->hasFormat("application/x-movedVertex"))
    {
        return false;
    }
    QByteArray encoded(data->data("application/x-movedVertex"));
    QDataStream stream(&encoded, QIODevice::ReadOnly);
    int insert_row = 0;
    if(row!=-1)
    {
        insert_row = row;
    }
    else if(parent.isValid())
    {
        insert_row = parent.row();
    }
    else
    {
        insert_row = rowCount(QModelIndex());
    }
    int src_row = -1;
    stream>>src_row;
    if(src_row<0 || src_row>=_vertices_.size())
    {
        return false;
    }
    moveRows(QModelIndex(),src_row,1,QModelIndex(),insert_row);
//    beginMoveRows(QModelIndex(),src_row,src_row,
//                  QModelIndex(),insert_row>src_row?insert_row+1:insert_row);
//    std::swap(_vertices_[src_row],_vertices_[insert_row>src_row?insert_row-1:insert_row]);
//    endMoveRows();
    return true;
}

Qt::DropActions VertexModel::supportedDropActions() const
{
    return Qt::MoveAction;
}
Qt::DropActions VertexModel::supportedDragActions() const
{
    return Qt::MoveAction;
}

bool VertexModel::moveRows(const QModelIndex& sourceParent,
              int sourceRow, int count, const
              QModelIndex& destParent, int destChild)
{
    Q_UNUSED(destParent)
    Q_UNUSED(sourceParent)
    if(!count) return false;
    int from = sourceRow>=_vertices_.size()?_vertices_.size()-1:sourceRow;
    int to = destChild>=_vertices_.size()?_vertices_.size()-1:destChild;
    //beginMoveRows(sourceParent,from,from,
    //              destParent,to);
    ///!УВАГА, гэта будзе працаваць толькі для адзіночных пераносаў. Потым трэба будзе
    /// дапрацаваць, каб магчыма было пераносіць некалькі шэрагаў.

    _vertices_.move(from,to);
    //endMoveRows();
    return true;
}

bool VertexModel::insertRows(int row, int count, const QModelIndex& parent)
{
    if(!count) return false;
    beginInsertRows(parent,row,row+count-1);
    for(int i = row; i<row+count;i++)
    {
        _vertices_.insert(i>_vertices_.size()?_vertices_.size():i,nullptr);
    }
    endInsertRows();
    return true;
}
bool VertexModel::removeRows(int row, int count, const QModelIndex& parent)
{
    if(!count) return false;
    beginRemoveRows(parent,row,row+count-1);
    for(int i = row; i<row+count;i++)
    {
        _vertices_.remove(i>=_vertices_.size()?_vertices_.size()-1:i);
    }
    endRemoveRows();
    return true;
}
void VertexModel::addItem(GViewItem* item, int row)
{
    if(_vertices_.contains(item))
    {
        return;
    }
    if(row<0||row>=_vertices_.size())
    {
        row = _vertices_.size();
    }
    beginInsertRows(QModelIndex(),row,row);
    _vertices_.insert(row,item);
    endInsertRows();
    return;
}

void VertexModel::removeItem(GViewItem* item)
{
    int index = _vertices_.indexOf(item);
    if(index<0)
    {
        return;
    }
    beginRemoveRows(QModelIndex(),index,index);
    _vertices_.remove(index);
    endRemoveRows();
    return;
}

int VertexModel::size()
{
    return _vertices_.size();
}

GViewItem* VertexModel::operator[](int num)
{
    if(num<0 || num>= _vertices_.size())
    {
        return nullptr;
    }
    return _vertices_.at(num);
}

GViewItem* VertexModel::at(int num)
{
    if(num<0 || num>= _vertices_.size())
    {
        return nullptr;
    }
    return _vertices_.at(num);
}

QVector<GViewItem*>::const_iterator VertexModel::begin() const
{
    return _vertices_.cbegin();
}

QVector<GViewItem*>::const_iterator VertexModel::end() const
{
    return _vertices_.cend();
}
QVector<GViewItem*>::const_iterator VertexModel::find(GViewItem* item)
{
    return std::find(begin(),end(),item);
}

bool VertexModel::contains(GViewItem* item)const
{
    return _vertices_.contains(item);
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
