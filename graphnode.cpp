#include "graphnode.h"
#include "graphedge.h"
#include "graphwidget.h"


GraphNode::GraphNode(local_id_type id, GraphWidget* graph):
    _id_(id),
    _graph_(graph),
    _is_hovered_(false)
{
//    setFlag(ItemIsMovable, true);
//    setFlag(ItemIsFocusable,true);
//    setAcceptHoverEvents(true);
    setFlags(ItemSendsGeometryChanges|ItemIsMovable);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);
    return;
}

local_id_type GraphNode::getId() const
{
    return _id_;
}
void GraphNode::setId(const local_id_type& id)
{
    _id_ = id;
    return;
}

void GraphNode::addEdge(edge_ptr edge)
{
    _edges_.append(edge);
    edge->adjust();
    ///TODO: праверка на існаваньне сувязі.
    return;
}

//void GraphNode::addEdge(const local_id_type& linked_id)
//{
//    for(edge_ptr& ptr: _edges_)
//    {

//    }
//    return;
//}

void GraphNode::removeEdge(edge_ptr edge)
{
    auto iter = std::find(_edges_.begin(),_edges_.end(),edge);
    if(iter!=_edges_.end())
    {
        _edges_.erase(iter);
    }
    return;
}

//void GraphNode::removeEdge(uint linked_id)
//{
//    for(edge_ptr& ptr:_edges_)
//    {

//    }
//    return;
//}

void GraphNode::setValue(const local_val_type& value)
{
    _value_ = value;
    return;
}

QList<edge_ptr> GraphNode::edges() const
{
    return _edges_;
}

void GraphNode::calculateForces()
{
    if(!scene()||scene()->mouseGrabberItem()==this)
    {
        _next_position_ = pos();
        return;
    }

    qreal x_velocity = 0;
    qreal y_velocity = 0;

    const QList<QGraphicsItem*> items = scene()->items();
    for(QGraphicsItem* item: items)
    {
        GraphNode * node = qgraphicsitem_cast<GraphNode*>(item);
        if(!node)
        {
            continue;
        }
        QPointF vector = mapToItem(node,0,0);
        qreal dx = vector.x();
        qreal dy = vector.y();
        double length = 2.0*(pow(dx,2)+pow(dy,2));
        if(length>0)
        {
            x_velocity+=(dx*150.0)/length;
            y_velocity+=(dy*150.0)/length;
        }
    }

    double weight = (_edges_.size()+1)*10;
    for(int i = 0; i<_edges_.size();i++)
    {
        GraphEdge * edge = _edges_.at(i);
    //for(const GraphEdge* edge: qAsConst(_edges_))
    //{
        QPointF vector;
        if(edge->sourceNode()==this)
        {
            vector = mapToItem(edge->destNode(),0,0);
        }
        else
        {
            vector = mapToItem(edge->sourceNode(),0,0);
        }
        x_velocity-=vector.x()/weight;
        y_velocity-=vector.y()/weight;
    }

    if(qAbs(x_velocity)<0.1 && qAbs(y_velocity)<0.1)
    {
        x_velocity = 0;
        y_velocity = 0;
    }
    QRectF final_scene = scene()->sceneRect();
    _next_position_ = pos()+QPointF(x_velocity,y_velocity);
    _next_position_.setX(qMin(
                             qMax(
                                 _next_position_.x(),
                                 final_scene.left()+10),
                             final_scene.right()-10));
    _next_position_.setY(qMin(
                             qMax(
                                 _next_position_.y(),
                                 final_scene.top()+10),
                             final_scene.bottom()-10));
    return;
}

bool GraphNode::advancePosition()
{
    if(_next_position_==pos())
    {
        return false;
    }
    setPos(_next_position_);
    return true;

}

QRectF GraphNode::boundingRect() const
{
    qreal adjust = 2;
    return QRectF(-10-adjust, -10-adjust,23+adjust,23+adjust);
}

QPainterPath GraphNode::shape() const
{
    QPainterPath path;
    path.addEllipse(-10,-10,20,20);
    return path;
}

void GraphNode::paint(QPainter * painter,const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    Q_UNUSED(widget)
//    if(_is_hovered_)
//    {
//        painter->setPen(QPen(Qt::red,2));
//        painter->setBrush(Qt::white);
//    }
//    else
//    {
        painter->setPen(Qt::NoPen);
        painter->setBrush(Qt::darkGray);
//    }
    painter->drawEllipse(-7,-7,20,20);
    QRadialGradient gradient(-3,-3,10);
    if(option->state & QStyle::State_Sunken)
    {
        gradient.setCenter(3,3);
        gradient.setFocalPoint(3,3);
        gradient.setColorAt(1,QColor(Qt::yellow).lighter(120));
        gradient.setColorAt(0,QColor(Qt::darkYellow).lighter(120));
    }
    else
    {
        gradient.setColorAt(0,Qt::yellow);
        gradient.setColorAt(1,Qt::darkYellow);
    }
    painter->setBrush(gradient);
    painter->setPen(QPen(Qt::black,0));
    painter->drawEllipse(-10,-10,20,20);

}

QVariant GraphNode::itemChange(GraphicsItemChange change, const QVariant& value)
{
    switch(change)
    {
    case ItemPositionHasChanged:
    {
        for(edge_ptr edge:_edges_)
        {
            edge->adjust();
        }
        _graph_->itemMoved();
    }
        break;
    default:
    {
        ///...
    }
        break;
    }
    return QGraphicsItem::itemChange(change,value);
}

void GraphNode::mousePressEvent(QGraphicsSceneMouseEvent * m_event)
{
    qDebug()<<m_event;
    update();
    QGraphicsItem::mousePressEvent(m_event);
    return;
}
void GraphNode::mouseReleaseEvent(QGraphicsSceneMouseEvent * m_event)
{
    qDebug()<<m_event;
    update();
    QGraphicsItem::mouseReleaseEvent(m_event);
    return;
}

//void GraphNode::hoverEnterEvent(QGraphicsSceneHoverEvent * h_event)
//{
//    Q_UNUSED(h_event)
//    _is_hovered_ = true;
//    return;
//}
//void GraphNode::hoverLeaveEvent(QGraphicsSceneHoverEvent * h_event)
//{
//    Q_UNUSED(h_event)
//    _is_hovered_ = false;
//    return;
//}

