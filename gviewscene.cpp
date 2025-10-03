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
