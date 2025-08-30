#include "gviewport.h"

GViewPort::GViewPort(QWidget *tata):QGraphicsView(tata),
    _add_mode_(false),_delete_mode_(false),_add_edge_mode_(false)
{
    _new_edge_=  nullptr;
    _del_edge_=  nullptr;
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
    if(_vertices_.contains(vertex))
    {
        return;
    }
    _vertices_.push_back(vertex);
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
    QList<GViewItem*>::const_iterator it = std::find(_vertices_.cbegin(),_vertices_.cend(),vertex);
    if(it!=_vertices_.end())
    {
        delLinkedEdges(vertex);
        scene()->removeItem(*it);
        delete *it;
        _vertices_.erase(it);
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
            if(!(*it)->isDirected())
            {
                (*it)->destination()->delEdge(*it);
            }
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
                edge->destination()->addEdge(edge);
                return true;
            }
            else
            {
                return false;
            }
        }
    }
    GViewEdge * new_edge = new GViewEdge(source,dest,directed);
    source->addEdge(new_edge);
    if(!directed)
    {
        dest->addEdge(new_edge);
    }
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
    _new_edge_ = new GViewEdge(src,true);
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
                edge->destination()->addEdge(edge);
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
    _new_edge_ = new GViewEdge(src,true,GViewEdge::GVedge_deletion);
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


void GViewPort::changeAddMode(bool mode)
{
    if(mode)
    {
        QPixmap new_cursor_pix("sphere.png");
        QCursor new_cursor(new_cursor_pix.scaled(new_cursor_pix.width()/2,
                                                 new_cursor_pix.height()/2,Qt::KeepAspectRatio));
        QApplication::setOverrideCursor(new_cursor);

    }
    else
    {
        QApplication::restoreOverrideCursor();
    }
    _add_mode_=mode;
    return;
}

void GViewPort::changeDeleteMode(bool mode)
{
    if(mode)
    {
        QPixmap new_cursor_pix("x_sign.png");
        QCursor new_cursor(new_cursor_pix.scaled(new_cursor_pix.width()/3,
                                                 new_cursor_pix.height()/3,
                                                 Qt::KeepAspectRatio));
        QApplication::setOverrideCursor(new_cursor);
    }
    else
    {
        QApplication::restoreOverrideCursor();
    }
    _delete_mode_ = mode;
    return;
}

void GViewPort::changeAddEdgeMode(bool mode)
{
    _add_edge_mode_ = mode;
    return;
}

void GViewPort::mouseReleaseEvent(QMouseEvent* m_event)
{
    switch(_mode_)
    {
    case GPort_add:
    {
        GViewItem* item = new GViewItem("Info",
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

    }
    }

    /*
    if(_add_mode_)
    {
        GViewItem* item = new GViewItem("Info",QColor::fromRgb(QRandomGenerator::global()->generate()));
        scene()->addItem(item);
        item->setPos(mapToScene(m_event->pos()));
        QApplication::restoreOverrideCursor();
        _add_mode_ = false;
    }
    else if(_delete_mode_)
    {
        _delete_mode_ = false;
        QGraphicsItem* base_item = scene()->itemAt(mapToScene(m_event->pos()),transform());
        if(base_item)
        {
            GViewItem* item = qgraphicsitem_cast<GViewItem*>(base_item);
            if(item)
            {
                scene()->removeItem(item);
                delete item;
                QApplication::restoreOverrideCursor();
            }
            else
            {
                _delete_mode_=true;
            }
        }
        else
        {
            _delete_mode_=true;
        }
    }
    else if(_add_edge_mode_)
    {
        if(!_new_edge_)
        {
            QGraphicsItem* base_item = scene()->itemAt(mapToScene(m_event->pos()),transform());
            if(base_item)
            {
                GViewItem* item = qgraphicsitem_cast<GViewItem*>(base_item);
                if(item)
                {
                    _new_edge_ = new GViewEdge(item);
                    scene()->addItem(_new_edge_);
                    setMouseTracking(true);
                }
            }
        }
        else
        {
            QGraphicsItem* base_item = scene()->itemAt(mapToScene(m_event->pos()),transform());
            if(base_item)
            {
                GViewItem* item = qgraphicsitem_cast<GViewItem*>(base_item);
                if(item)
                {
                    if(_new_edge_->source()!=item)
                    {
                        _new_edge_->setDest(item);
                        _new_edge_->source()->addEdge(_new_edge_);
                        _new_edge_->destination()->addEdge(_new_edge_);
                        setMouseTracking(false);
                        _add_edge_mode_ = false;
                        _new_edge_ = nullptr;
                    }
                }
            }
        }
    }
    */


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

//    if(_add_edge_mode_&&_new_edge_)
//    {
//        _new_edge_->searchDestination(mapToScene(m_event->pos()));
//    }
    QGraphicsView::mouseMoveEvent(m_event);
    return;
}
