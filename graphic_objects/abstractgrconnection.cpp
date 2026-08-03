#include "abstractgrconnection.h"
#include "abstractGrItem.h"

AbstractGrConnection::AbstractGrConnection(const item_id_t &id, bool directed, QGraphicsObject *tata):
    QGraphicsObject(tata),AbstractGrInterface(id),_src_item_(nullptr),_dest_item_(nullptr),
    _directed_(directed),_weight_(0),_mode_(GrEdge_Null)
{

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

}

void AbstractGrConnection::setDestination(AbstractGrItem* dest)
{

}

void AbstractGrConnection::setGrX(coord_real x)
{

}

void AbstractGrConnection::setGrY(coord_real y)
{

}

coord_real AbstractGrConnection::getGrX() const
{

}
coord_real AbstractGrConnection::getGrY() const
{

}

void AbstractGrConnection::setGrWidth(coord_real width)
{

}

void AbstractGrConnection::setGrHeight(coord_real height)
{

}

coord_real AbstractGrConnection::getGrWidth() const
{

}

coord_real AbstractGrConnection::getGrHeight() const
{

}

void AbstractGrConnection::moveGr(coord_real x, coord_real y)
{

}

void AbstractGrConnection::drawGr()
{

}
