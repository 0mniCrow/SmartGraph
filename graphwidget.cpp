#include "graphwidget.h"
#include "graphedge.h"
#include "graphnode.h"

GraphWidget::GraphWidget(QWidget *tata):QGraphicsView(tata)
{
    QGraphicsScene* scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setSceneRect(-200,-200,400,400);
    setScene(scene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    scale(qreal(0.8),qreal(0.8));
    setMinimumSize(400,400);
    setWindowTitle(tr("Elastic Nodes"));

    GraphNode* node1 = new GraphNode(1,this);
    GraphNode* node2 = new GraphNode(2,this);
    GraphNode* node3 = new GraphNode(3,this);
    GraphNode* node4 = new GraphNode(4,this);
    _centralNode_ = new GraphNode(0,this);
    GraphNode* node5 = new GraphNode(5,this);
    GraphNode* node6 = new GraphNode(6,this);
    GraphNode* node7 = new GraphNode(7,this);
    GraphNode* node8 = new GraphNode(8,this);

    scene->addItem(node1);
    scene->addItem(node2);
    scene->addItem(node3);
    scene->addItem(node4);
    scene->addItem(_centralNode_);
    scene->addItem(node5);
    scene->addItem(node6);
    scene->addItem(node7);
    scene->addItem(node8);
    scene->addItem(new GraphEdge(node1,node2));
    scene->addItem(new GraphEdge(node2,node3));
    scene->addItem(new GraphEdge(node2,_centralNode_));
    scene->addItem(new GraphEdge(node3,node5));
    scene->addItem(new GraphEdge(node4,node1));
    scene->addItem(new GraphEdge(node4,_centralNode_));
    scene->addItem(new GraphEdge(_centralNode_,node5));
    scene->addItem(new GraphEdge(_centralNode_,node7));
    scene->addItem(new GraphEdge(node5,node8));
    scene->addItem(new GraphEdge(node6,node4));
    scene->addItem(new GraphEdge(node7,node6));
    scene->addItem(new GraphEdge(node8,node7));

    node1->setPos(-50,-50);
    node2->setPos(0,-50);
    node3->setPos(50,-50);
    node4->setPos(-50,0);
    _centralNode_->setPos(0,0);
    node5->setPos(50,0);
    node6->setPos(-50,50);
    node7->setPos(0,50);
    node8->setPos(50,50);
    return;
}

void GraphWidget::itemMoved()
{
    if(!_timerID_)
    {
        _timerID_ = startTimer(1000/25);
    }
}

void GraphWidget::shuffle()
{
    const QList<QGraphicsItem*> items = scene()->items();
    for(QGraphicsItem* item: items)
    {
        if(qgraphicsitem_cast<GraphNode*>(item))
        {
            item->setPos(-150+QRandomGenerator::global()->bounded(300),
                         -150+QRandomGenerator::global()->bounded(300));
        }
    }
    return;
}

void GraphWidget::zoomIn()
{
    scaleView(qreal(1.2));
    return;
}

void GraphWidget::zoomOut()
{
    scaleView(qreal(1.2));
    return;
}

void GraphWidget::keyPressEvent(QKeyEvent* k_event)
{
    switch(k_event->key())
    {
    case Qt::Key_Up:
    {
        _centralNode_->moveBy(0,-20);
    }
        break;
    case Qt::Key_Down:
    {
        _centralNode_->moveBy(0,20);
    }
        break;
    case Qt::Key_Left:
    {
        _centralNode_->moveBy(-20,0);
    }
        break;
    case Qt::Key_Right:
    {
        _centralNode_->moveBy(20,0);
    }
        break;
    case Qt::Key_Plus:
    {
        zoomIn();
    }
        break;
    case Qt::Key_Minus:
    {
        zoomOut();
    }
        break;
    case Qt::Key_Space:
    case Qt::Key_Enter:
    {
        shuffle();
    }
        break;
    default:
    {
        k_event->ignore();
        QGraphicsView::keyPressEvent(k_event);
        return;
    }
    }
    k_event->accept();
    return;
}

void GraphWidget::timerEvent(QTimerEvent* t_event)
{
    Q_UNUSED(t_event);

    QList<GraphNode*> nodes;
    const QList<QGraphicsItem*> items = scene()->items();
    for(QGraphicsItem* item: items)
    {
        GraphNode* node = nullptr;
        node = qgraphicsitem_cast<GraphNode*>(item);
        if(node)
        {
            nodes.append(node);
            if(node->type()==GraphEdge::EdgeType)
            {
                nodes.pop_back();
            }
        }
    }

    for(GraphNode* node: qAsConst(nodes))
    {
        node->calculateForces();
    }
    bool items_r_moving = false;
    for(GraphNode* node:qAsConst(nodes))
    {
        if(node->advancePosition())
        {
            items_r_moving = true;
        }
    }
    if(!items_r_moving)
    {
        killTimer(_timerID_);
        _timerID_ = 0;
    }
    return;
}

void GraphWidget::wheelEvent(QWheelEvent* w_event)
{
    scaleView(pow(2.0,-w_event->angleDelta().y()/240.0));
    return;
}

void GraphWidget::drawBackground(QPainter* painter, const QRectF& rect)
{
    //Q_UNUSED(rect);

    QRectF sceneRect = this->sceneRect();
    QRectF rightShadow(sceneRect.right(),sceneRect.top()+5,5,sceneRect.height());
    QRectF bottomShadow(sceneRect.left()+5,sceneRect.bottom(),sceneRect.width(),5);

    if(rightShadow.intersects(rect)||rightShadow.contains(rect))
    {
        painter->fillRect(rightShadow,Qt::darkGray);
    }
    if(bottomShadow.intersects(rect)||bottomShadow.contains(rect))
    {
        painter->fillRect(bottomShadow,Qt::darkGray);
    }

    QLinearGradient gradient(sceneRect.topLeft(),sceneRect.bottomRight());
    gradient.setColorAt(0,Qt::white);
    gradient.setColorAt(1,Qt::lightGray);
    painter->fillRect(rect.intersected(sceneRect),gradient);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(sceneRect);

    QRectF textRect(sceneRect.left()+4,sceneRect.top()+4,
                    sceneRect.width()-4,sceneRect.height()-4);
    QString msg(tr("Націсніце і перасоўвайце ноды па акну, "
                   "набліжайце і аддаляйце колам мышы, ці "
                   "кнопкамі '+' або '-'"));
    QFont font = painter->font();
    font.setBold(true);
    font.setPointSize(14);
    painter->setFont(font);
    painter->setPen(Qt::lightGray);
    painter->drawText(textRect.translated(2,2),msg);
    painter->setPen(Qt::black);
    painter->drawText(textRect,msg);
    return;
}

void GraphWidget::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor,scaleFactor).mapRect(QRectF(0,0,1,1)).width();
    if(factor<0.07||factor>100)
    {
        return;
    }
    scale(scaleFactor,scaleFactor);
}
