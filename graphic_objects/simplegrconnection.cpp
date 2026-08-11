#include "simplegrconnection.h"
#include "abstractGrItem.h"


SimpleGrConnection::SimpleGrConnection(AbstractGrItem* source,
                                       AbstractGrItem* destination,
                                       ItemCommunicator *communicator,
                                       const item_id_t& id,
                                       bool directed,
                                       QGraphicsObject* tata):
    AbstractGrConnection(id,directed,tata)
{
    if((!source)|| (!destination) || (!communicator))
    {
        setMode(GrEdge_Null);
    }
    else
    {
        setMode(GrEdge_regular);
    }
    setSource(source);
    setDestination(destination);
    setCommunicator(communicator);
    return;
}

SimpleGrConnection::SimpleGrConnection(AbstractGrItem* source, char mode,
                                       ItemCommunicator *communicator,
                                       bool directed,
                                       const item_id_t& id,
                                       QGraphicsObject* tata):
    AbstractGrConnection(id,directed,tata)
{
    if((!source)||(!communicator))
    {
        setMode(GrEdge_Null);
    }
    else
    {
        setMode(mode);
    }
    setSource(source);
    setCommunicator(communicator);
    return;
}

void SimpleGrConnection::setStartPoint(coord_real x, coord_real y)
{
    _src_point_.setX(x);
    _src_point_.setY(y);
    redraw();
    return;
}

void SimpleGrConnection::setEndPoint(coord_real x, coord_real y)
{
    _dest_point_.setX(x);
    _dest_point_.setY(y);
    redraw();
    return;
}

coord_real SimpleGrConnection::getStartX() const
{
    return _src_point_.x();
}

coord_real SimpleGrConnection::getStartY() const
{
    return _src_point_.y();
}

coord_real SimpleGrConnection::getFinX() const
{
    return _dest_point_.x();
}

coord_real SimpleGrConnection::getFinY() const
{
    return _dest_point_.y();
}

QRectF SimpleGrConnection::boundingRect() const
{
    char mode(getMode());
    if(mode == GrEdge_incomplete||
            mode == GrEdge_deletion)
    {
        if(!getSource())
        {
            return QRectF();
        }
    }
    else
    {
        if(!getSource()||!getDestination())
        {
            return QRectF();
        }
    }
    qreal extra = (ABSTRACT_EDGE_WIDTH+getArrowSize())/2.0;
    return QRectF(_src_point_,
                  QSizeF(_dest_point_.x()-_src_point_.x(),
                         _dest_point_.y()-_src_point_.y())
                  ).
            normalized().
            adjusted(-extra,-extra,extra,extra);
}

bool SimpleGrConnection::isOperable() const noexcept
{
    char mode = getMode();
    if(mode == GrEdge_incomplete||
            mode == GrEdge_deletion)
    {
        if(!hasSourceItem())
        {
            return false;
        }
    }
    else
    {
        if(!hasMainItems())
        {
            return false;
        }
    }
    return true;
}

void SimpleGrConnection::getArrowSholders(double arr_atan, const QPointF& arr_head,
                      QPointF& first_sholder,QPointF& sec_sholder,bool dest_point)
{
    qreal arr_size(getArrowSize());

    double first_point_angle = 0.0;
    double last_point_angle = 0.0;
    if(dest_point)
    {
        first_point_angle = arr_atan-M_PI/3;
        last_point_angle = arr_atan-M_PI+M_PI/3;
    }
    else
    {
        first_point_angle = arr_atan+M_PI/3;
        last_point_angle = arr_atan+M_PI-M_PI/3;
    }

    first_sholder =
            arr_head +
            QPointF(sin(first_point_angle)*arr_size,
                    cos(first_point_angle)*arr_size);

    sec_sholder =
            arr_head +
            QPointF(sin(last_point_angle)*arr_size,
                    cos(last_point_angle)*arr_size);
    return;
}

void SimpleGrConnection::paint(QPainter* painter,
           const QStyleOptionGraphicsItem* option,
           QWidget* widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    if(!isOperable())
    {
        return;
    }
    QLineF line(_src_point_,_dest_point_);
    if(qFuzzyCompare(line.length(),qreal(0.0)))
    {
        return;
    }
    char mode = getMode();
    painter->setPen(QPen(
                        (mode==GrEdge_deletion)?
                            Qt::darkRed:
                            Qt::black,
                        EDGE_WIDTH,
                        Qt::SolidLine,
                        Qt::RoundCap,
                        Qt::RoundJoin));
    painter->drawLine(line);
    painter->setBrush(Qt::black);
    double angle = std::atan2(-line.dy(),line.dx());

    if(!isDirected() && mode==GrEdge_regular)
    {
        QPointF sourceArrowP1,sourceArrowP2;
        getArrowSholders(angle,_src_point_,sourceArrowP1,sourceArrowP2, false);
        painter->drawPolygon(QPolygonF()<<
                             line.p1()<<
                             sourceArrowP1<<
                             sourceArrowP2);
    }
    QPointF destArrowP1,destArrowP2;
    getArrowSholders(angle,_dest_point_,destArrowP1,destArrowP2,true);
    painter->drawPolygon(QPolygonF()<<
                         line.p2()<<
                         destArrowP1<<
                         destArrowP2);
    return;
}

void SimpleGrConnection::redraw()
{
    if(isVisible() && isOperable())
    {
        update();
    }
    return;
}

QPainterPath SimpleGrConnection::shape() const
{
    QPainterPath path(_src_point_);
    QLineF line(_src_point_,_dest_point_);
    if(line.length()>qreal(getSource()->getRadius()))
    {
        line.setLength(line.length()-qreal(getSource()->getRadius())*2);
    }
    path.lineTo(line.p2());
    return path;
}


void SimpleGrConnection::recalculate()
{
    if(!hasMainItems())
    {
        return;
    }

    QLineF line(mapFromItem(getSource(),0,0),mapFromItem(getDestination(),0,0));
    qreal length = line.length();
    qreal src_radius = static_cast<qreal>(getSource()->getRadius());
    qreal dest_radius = static_cast<qreal>(getDestination()->getRadius());
    prepareGeometryChange();
    if(length>(src_radius>dest_radius?src_radius:dest_radius))
    {
        QPointF edgeOffsetSrc((line.dx()*src_radius)/length,
                           (line.dy()*src_radius)/length);
        QPointF edgeOffsetDest((line.dx()*dest_radius)/length,
                           (line.dy()*dest_radius)/length);
        _src_point_ = line.p1() + edgeOffsetSrc;
        _dest_point_ = line.p2() - edgeOffsetDest;
    }
    else
    {
        _src_point_ = _dest_point_ = line.p1();
    }
    return;
}

int SimpleGrConnection::type() const
{
    return Type;
}

char SimpleGrConnection::grConnectionType() const noexcept
{
    return SimpleConnection;
}
