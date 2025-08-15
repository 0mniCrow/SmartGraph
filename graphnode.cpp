#include "graphnode.h"

GraphNode::GraphNode(local_id_type id):_id_(id),_is_hovered_(false)
{
    setFlags(ItemIsMovable);
    setFlags(ItemSendsGeometryChanges);
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
    //edge.lock()->adjust();
    ///TODO: праверка на існаваньне сувязі.
    return;
}
void GraphNode::addEdge(const local_id_type& linked_id)
{
    for(edge_ptr& ptr: _edges_)
    {

    }
    return;
}
void GraphNode::removeEdge(edge_ptr edge)
{
    ///Магчыма, гэта працаваць не будзе
    return;
}
void GraphNode::removeEdge(uint linked_id)
{
    for(edge_ptr& ptr:_edges_)
    {

    }
    return;
}

void GraphNode::setValue(const local_val_type& value)
{
    _value_ = value;
    return;
}
QList<edge_ptr> GraphNode::edges() const
{
    return _edges_;
}

QRectF GraphNode::boundingRect() const
{
    qreal adjust = 2;
    return QRectF(-10-adjust, -1-adjust,23+adjust,23+adjust);
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
    if(_is_hovered_)
    {
        painter->setPen(QPen(Qt::red,2));
        painter->setBrush(Qt::white);
    }
    else
    {
        painter->setPen(Qt::NoPen);
        painter->setBrush(Qt::darkGray);
    }
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
        for(edge_ptr& edge:_edges_)
        {
            //edge.lock()->adjust();
        }
        //scene()->views().first()->itemMoved();
    }
        break;
    default:
    {
        break;
    }
    }
    return QGraphicsItem::itemChange(change,value);
}

void GraphNode::mousePressEvent(QGraphicsSceneMouseEvent * m_event)
{
    update();
    QGraphicsItem::mousePressEvent(m_event);
    return;
}
void GraphNode::mouseReleaseEvent(QGraphicsSceneMouseEvent * m_event)
{
    update();
    QGraphicsItem::mouseReleaseEvent(m_event);
    return;
}

void GraphNode::hoverEnterEvent(QGraphicsSceneHoverEvent * h_event)
{
    _is_hovered_ = true;
}
void GraphNode::hoverLeaveEvent(QGraphicsSceneHoverEvent * h_event)
{
    _is_hovered_ = false;
}

