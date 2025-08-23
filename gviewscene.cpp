#include "gviewscene.h"

GViewScene::GViewScene(QObject *tata):QGraphicsScene(tata),_add_mode_(false)
{

}
void GViewScene::setAddMode(bool add_mode)
{
    if(add_mode)
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
    _add_mode_=add_mode;
    return;
}
void GViewScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* m_event)
{
    if(_add_mode_)
    {
        GViewItem* item = new GViewItem("Info",QColor::fromRgb(QRandomGenerator::global()->generate()));
        addItem(item);
        item->setPos(m_event->scenePos());
        QApplication::restoreOverrideCursor();
        _add_mode_ = false;
    }
    QGraphicsScene::mouseReleaseEvent(m_event);
    return;
}

void GViewScene::mouseMoveEvent(QGraphicsSceneMouseEvent* m_event)
{
    return;
}
