#include "gviewscene.h"

GViewScene::GViewScene(QObject *tata):QGraphicsScene(tata)
{
    setItemIndexMethod(QGraphicsScene::NoIndex);
    return;
}


void GViewScene::mousePressEvent(QGraphicsSceneMouseEvent* m_event)
{
    if(qgraphicsitem_cast<GViewItem*>(itemAt(m_event->scenePos(),QTransform())))
    {
        QGraphicsScene::mousePressEvent(m_event);
    }
    else
    {
       m_event->accept();
    }
    return;
}

void GViewScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* m_event)
{
    if(qgraphicsitem_cast<GViewItem*>(itemAt(m_event->scenePos(),QTransform())))
    {
        QGraphicsScene::mouseDoubleClickEvent(m_event);
    }
    else
    {
       m_event->accept();
    }
    return;
}

void GViewScene::drawBackground(QPainter* painter, const QRectF& rect)
{
    if(_bg_.isNull())
    {
        return;
    }
    painter->save();
    painter->drawPixmap(rect,_bg_.copy(),rect);
    painter->restore();
}

bool GViewScene::setBG(const QPixmap& source_bg)
{
    _bg_ = source_bg;
    return true;
}
