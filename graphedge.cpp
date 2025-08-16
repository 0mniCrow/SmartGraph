#include "graphedge.h"
#include "graphnode.h"

GraphEdge::GraphEdge(GraphNode* src, GraphNode* dst, bool directed):_source_node_(src),_dest_node_(dst),_directed_(directed)
{
    setAcceptedMouseButtons(Qt::NoButton);
    src->addEdge(this);
    dst->addEdge(this);
    adjust();
    return;
}
GraphNode* GraphEdge::sourceNode() const
{
    return _source_node_;
}
GraphNode* GraphEdge::destNode() const
{
    return _dest_node_;
}

void GraphEdge::adjust()
{
    if(!_source_node_||!_dest_node_)
    {
        return;
    }

    QLineF line(mapFromItem(_source_node_,0,0),mapFromItem(_dest_node_,0,0));
    qreal length = line.length();

    prepareGeometryChange();
    if(length>qreal(20.0))
    {
        QPointF edgeOffset((line.dx()*10)/length,(line.dy()*10)/length);
        _source_coord_ = line.p1()+edgeOffset;
        _dest_coord_ =  line.p2() - edgeOffset;
    }
    else
    {
        _source_coord_ = _dest_coord_ = line.p1();
    }
    return;
}


QRectF GraphEdge::boundingRect() const
{
    if(!_source_node_||!_dest_node_)
    {
        return QRectF();
    }

    qreal penWidth = 1;
    qreal extra = (penWidth+_arrowSize_)/2.0;
    return QRectF(_source_coord_,QSizeF(_dest_coord_.x()-_source_coord_.x(),
                                        _dest_coord_.y()-_source_coord_.y())).normalized().adjusted(
                -extra,-extra,extra,extra);
}
void GraphEdge::paint(QPainter* painter,
                      const QStyleOptionGraphicsItem* option,
                      QWidget* widget)
{
    if(!_source_node_||!_dest_node_)
    {
        return;
    }
    QLineF line(_source_coord_,_dest_coord_);
    if(qFuzzyCompare(line.length(),qreal(0.0)))
    {
        return;
    }
    painter->setPen(QPen(Qt::black,1,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin));
    painter->drawLine(line);

    double angle = std::atan2(-line.dy(),line.dx());
    QPointF sourceArrowP1 = _source_coord_ + QPointF(sin(angle+M_PI/3)*_arrowSize_,
                                                     cos(angle+M_PI/3)*_arrowSize_);
    QPointF sourceArrowP2 = _source_coord_ + QPointF(sin(angle+M_PI - M_PI/3)*_arrowSize_,
                                                     cos(angle+M_PI-M_PI/3)*_arrowSize_);
    QPointF destArrowP1 = _dest_coord_ + QPointF(sin(angle-M_PI/3)*_arrowSize_,
                                                 cos(angle-M_PI/3)*_arrowSize_);
    QPointF destArrowP2 = _dest_coord_ + QPointF(sin(angle-M_PI+M_PI/3)*_arrowSize_,
                                                 cos(angle-M_PI+M_PI/3)*_arrowSize_);

    painter->setBrush(Qt::black);
    painter->drawPolygon(QPolygonF()<<line.p1()<<sourceArrowP1<<sourceArrowP2);
    painter->drawPolygon(QPolygonF()<<line.p2()<<destArrowP1<<destArrowP2);
}
