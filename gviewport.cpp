#include "gviewport.h"

GViewPort::GViewPort(QWidget *tata):QGraphicsView(tata),
    _add_mode_(false),_delete_mode_(false)
{
    return;
}

void GViewPort::changeAddMode(bool mode)
{
    dynamic_cast<GViewScene*>(scene())->setAddMode(mode);
//    if(mode)
//    {
//        QPixmap new_cursor_pix("sphere.png");
//        QCursor new_cursor(new_cursor_pix.scaled(new_cursor_pix.width()/2,
//                                                 new_cursor_pix.height()/2,Qt::KeepAspectRatio));
//        QApplication::setOverrideCursor(new_cursor);

//    }
//    else
//    {
//        QApplication::restoreOverrideCursor();
//    }
//    _add_mode_=mode;
    return;
}

void GViewPort::changeDeleteMode(bool mode)
{
    if(mode)
    {
        QPixmap new_cursor_pix("x_sign.png");
        QCursor new_cursor(new_cursor_pix.scaled(new_cursor_pix.width()/2,
                                                 new_cursor_pix.height()/2,
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

void GViewPort::changeMouseTrackMode(bool mode)
{
    setMouseTracking(mode);
}

void GViewPort::mouseReleaseEvent(QMouseEvent* m_event)
{
//    if(_add_mode_)
//    {
//        GViewItem* item = new GViewItem("Info",QColor::fromRgb(QRandomGenerator::global()->generate()));
//        scene()->addItem(item);
//        item->setPos(m_event->pos());
//        QApplication::restoreOverrideCursor();
//        _add_mode_ = false;
//    }
    if(_delete_mode_)
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
    QGraphicsView::mouseReleaseEvent(m_event);
    return;
}
