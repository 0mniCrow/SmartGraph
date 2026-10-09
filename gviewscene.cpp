#include "gviewscene.h"
#include "graphic_objects/abstractgrqtitem.h"
#include "graphic_objects/abstractgrqtconnection.h"

GViewScene::GViewScene(QObject *tata):QGraphicsScene(tata)
{
    setItemIndexMethod(QGraphicsScene::NoIndex);
    return;
}

GViewScene::~GViewScene()
{
    auto all_items = items();
    QSet<QGraphicsItem*> for_deletion;
    //! Гэта патрэбна, пакуль уся сістэма не пераведзена на графічныя аб'екты
    //! Далей праверку на дынамічную трансфармацыю можна будзе выдаліць.
    for(QGraphicsItem* s_item:all_items)
    {
        if(dynamic_cast<AbstractGrQtItem*>(s_item)||
                dynamic_cast<AbstractGrQtConnection*>(s_item))
        {
            removeItem(s_item);
        }
        else
        {
            removeItem(s_item);
            for_deletion.insert(s_item);
        }
    }
    qDeleteAll(for_deletion);
    return;
}

void GViewScene::mousePressEvent(QGraphicsSceneMouseEvent* m_event)
{
    if(qgraphicsitem_cast<GViewItem*>(itemAt(m_event->scenePos(),QTransform())))
    {
        QGraphicsScene::mousePressEvent(m_event);
//        qDebug()<<"scene mousePress occured";
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
//        qDebug()<<"scene doubleClick occured";
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
