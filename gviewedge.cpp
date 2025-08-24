#include "gviewedge.h"

GViewEdge::GViewEdge(GViewItem *source,
                     GViewItem *destination,
                     bool directed):
    _src_item_(source),_dest_item_(destination),_directed_(directed)
{
    return;
}


GViewItem* GViewEdge::source() const
{
    return _src_item_;
}

GViewItem* GViewEdge::destination() const
{
    return _dest_item_;
}

void GViewEdge::recalculate()
{
    if(!_src_item_||!_dest_item_)
    {
        return;
    }

    QLineF line(mapFromItem(_src_item_,0,0),mapFromItem(_dest_item_,0,0));
    qreal length = line.length();

    prepareGeometryChange();
    if(length>qreal(20.0))
    {
        QPointF edgeOffset((line.dx()*10)/length,(line.dy()*10)/length);
        _src_point_ = line.p1()+edgeOffset;
        _dest_point_ =  line.p2() - edgeOffset;
    }
    else
    {
        _src_point_ = _dest_point_ = line.p1();
    }
    return;
}

QRectF GViewEdge::boundingRect() const
{
    if(!_src_item_||!_dest_item_)
    {
        return QRectF();
    }

    qreal penWidth = 1;
    qreal extra = (penWidth)/2.0;
    return QRectF(_src_point_,
                  QSizeF(_dest_point_.x()-_src_point_.x(),
                         _dest_point_.y()-_src_point_.y())
                  ).
            normalized().
            adjusted(-extra,-extra,extra,extra);
}
void GViewEdge::paint(QPainter* painter,
           const QStyleOptionGraphicsItem* option,
           QWidget* widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    if(!_src_item_||!_dest_item_)
    {
        return;
    }
    QLineF line(_src_point_,_dest_point_);
    if(qFuzzyCompare(line.length(),qreal(0.0)))
    {
        return;
    }
    painter->setPen(QPen(Qt::black,1,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin));
    painter->drawLine(line);
    return;
}
