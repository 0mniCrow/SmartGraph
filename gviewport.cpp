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
                                        Qt::gray/*QColor::fromRgb(
                                            QRandomGenerator::global()->generate()
                                            )*/
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
        selectItem(item);
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

void GViewPort::selectItem(GViewItem* selected_item, bool outside)
{
    if(selected_item)
    {
        if(selected_item != _selected_vertex_)
        {
            if(_selected_vertex_)
                _selected_vertex_->setSelected(false);
            _selected_vertex_ = selected_item;
            _selected_vertex_->setSelected(true);
            if(outside)
            {
                return;
            }
            QString selected("Selected Item: ");
            selected.append(_selected_vertex_->info());
            emit selectedInfo(selected);
            emit viewNewSelect(_selected_vertex_);
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
    return;
}

void GViewPort::outsideNewSelect(GViewItem* selected_item)
{
    selectItem(selected_item,true);
    return;
}
