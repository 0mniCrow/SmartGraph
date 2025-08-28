#include "gviewport.h"

GViewPort::GViewPort(QWidget *tata):QGraphicsView(tata),
    _add_mode_(false),_delete_mode_(false),_add_edge_mode_(false)
{
    _new_edge_=  nullptr;
    return;
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
    QGraphicsView::mouseReleaseEvent(m_event);
    return;
}

void GViewPort::mouseMoveEvent(QMouseEvent* m_event)
{
    if(_add_edge_mode_&&_new_edge_)
    {
        _new_edge_->searchDestination(mapToScene(m_event->pos()));
    }
    QGraphicsView::mouseMoveEvent(m_event);
    return;
}
