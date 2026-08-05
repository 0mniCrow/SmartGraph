#include "abstractgrconnection.h"
#include "abstractGrItem.h"

AbstractGrConnection::AbstractGrConnection(const item_id_t &id, bool directed, QGraphicsObject *tata):
    QGraphicsObject(tata),AbstractGrInterface(id),_src_item_(nullptr),_dest_item_(nullptr),
    _directed_(directed),_weight_(0),_mode_(GrEdge_Null)
{
    return;
}

QRectF AbstractGrConnection::boundingRect() const
{
    if(_mode_ == GrEdge_incomplete||
            _mode_ == GrEdge_deletion)
    {
        if(!_src_item_)
        {
            return QRectF();
        }
    }
    else
    {
        if(!_src_item_||!_dest_item_)
        {
            return QRectF();
        }
    }
    qreal extra = (ABSTRACT_EDGE_WIDTH+_arrowSize_)/2.0;
    return QRectF(_src_point_,
                  QSizeF(_dest_point_.x()-_src_point_.x(),
                         _dest_point_.y()-_src_point_.y())
                  ).
            normalized().
            adjusted(-extra,-extra,extra,extra);
}

void AbstractGrConnection::paint(QPainter* painter,
           const QStyleOptionGraphicsItem* option,
           QWidget* widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    if(_mode_ == GrEdge_incomplete||
            _mode_ == GrEdge_deletion)
    {
        if(!_src_item_)
        {
            return;
        }
    }
    else
    {
        if(!_src_item_||!_dest_item_)
        {
            return;
        }
    }
    QLineF line(_src_point_,_dest_point_);
    if(qFuzzyCompare(line.length(),qreal(0.0)))
    {
        return;
    }
    painter->setPen(QPen(
                        (_mode_==GrEdge_deletion)?
                            Qt::darkRed:
                            Qt::black,
                        EDGE_WIDTH,
                        Qt::SolidLine,
                        Qt::RoundCap,
                        Qt::RoundJoin));
    painter->drawLine(line);
    painter->setBrush(Qt::black);
    double angle = std::atan2(-line.dy(),line.dx());
    if(!_directed_ && _mode_==GrEdge_regular)
    {
        QPointF sourceArrowP1 =
                _src_point_ +
                QPointF(sin(angle+M_PI/3)*_arrowSize_,
                        cos(angle+M_PI/3)*_arrowSize_);

        QPointF sourceArrowP2 =
                _src_point_ +
                QPointF(sin(angle+M_PI - M_PI/3)*_arrowSize_,
                        cos(angle+M_PI-M_PI/3)*_arrowSize_);

        painter->drawPolygon(
                    QPolygonF()<<line.p1()<<sourceArrowP1<<sourceArrowP2);
    }
    QPointF destArrowP1 =
            _dest_point_ +
            QPointF(sin(angle-M_PI/3)*_arrowSize_,
                    cos(angle-M_PI/3)*_arrowSize_);
    QPointF destArrowP2 =
            _dest_point_ +
            QPointF(sin(angle-M_PI+M_PI/3)*_arrowSize_,
                    cos(angle-M_PI+M_PI/3)*_arrowSize_);
    painter->drawPolygon(
                QPolygonF()<<line.p2()<<destArrowP1<<destArrowP2);
    return;
}

QPainterPath AbstractGrConnection::shape() const
{
    QPainterPath path(_src_point_);
    QLineF line(_src_point_,_dest_point_);
    if(line.length()>qreal(_src_item_->getRadius()))
    {
        line.setLength(line.length()-qreal(_src_item_->getRadius())*2);
    }
    path.lineTo(line.p2());
    return path;
}

void AbstractGrConnection::recalculate()
{
    if(!_src_item_||!_dest_item_)
    {
        return;
    }

    QLineF line(mapFromItem(_src_item_,0,0),mapFromItem(_dest_item_,0,0));
    qreal length = line.length();
    qreal src_radius = static_cast<qreal>(_src_item_->getRadius());
    qreal dest_radius = static_cast<qreal>(_dest_item_->getRadius());
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

void AbstractGrConnection::setSource(AbstractGrItem* src)
{
    _src_item_ = src;
    return;
}

void AbstractGrConnection::setDestination(AbstractGrItem* dest)
{
    _dest_item_ = dest;
    return;
}

void AbstractGrConnection::setGrX(coord_real x)
{
    Q_UNUSED(x);
    return;
}

void AbstractGrConnection::setGrY(coord_real y)
{
    Q_UNUSED(y);
    return;
}

coord_real AbstractGrConnection::getGrX() const
{
    return x();
}
coord_real AbstractGrConnection::getGrY() const
{
    return y();
}

void AbstractGrConnection::setGrWidth(coord_real width)
{
    Q_UNUSED(width);
}

void AbstractGrConnection::setGrHeight(coord_real height)
{
    Q_UNUSED(height);
}

coord_real AbstractGrConnection::getGrWidth() const
{
    return boundingRect().width();
}

coord_real AbstractGrConnection::getGrHeight() const
{
    return boundingRect().height();
}

void AbstractGrConnection::moveGr(coord_real x, coord_real y)
{
    Q_UNUSED(x) Q_UNUSED(y)
            return;
}

void AbstractGrConnection::drawGr()
{
    update();
    return;
}
