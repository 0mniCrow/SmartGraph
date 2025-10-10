#include "gviewedge.h"

GViewEdge::GViewEdge(GViewItem *source,
                     GViewItem *destination, int vert_radius,
                     bool directed, GViewEdge_mode mode):
    _src_item_(source), _dest_item_(destination),
    _vertex_radius_(vert_radius),
    _directed_(directed),_mode_(mode)
{
    setAcceptedMouseButtons(Qt::NoButton);
    setZValue(-1);
    return;
}

GViewEdge::GViewEdge(GViewItem* source, int vert_radius,
                     bool direction,
                     GViewEdge_mode mode):
    _src_item_(source),_vertex_radius_(vert_radius),
    _directed_(direction),_mode_(mode)
{
    setAcceptedMouseButtons(Qt::NoButton);
    _dest_item_ = nullptr;
    return;
}

void GViewEdge::setVertRadius(int radius)
{
    _vertex_radius_ = radius;
    recalculate();
    return;
}

void GViewEdge::setArrowSize(qreal arrow_size)
{
    _arrowSize_ = arrow_size;
    update();
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

GViewItem* GViewEdge::setSource(GViewItem* new_src)
{
    GViewItem* prev_item = _src_item_;
    _src_item_ = new_src;
    return prev_item;
}

bool GViewEdge::setDest(GViewItem* new_dest)
{
    if(_mode_ != GVedge_incomplete ||
            _dest_item_)
    {
        return false;
    }
    _mode_ = GVedge_regular;
    //GViewItem* prev_item = _dest_item_;
    _dest_item_ = new_dest;
    return true; //prev_item;
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
    if(length>static_cast<qreal>(_vertex_radius_))
    {
        QPointF edgeOffset((line.dx()*_vertex_radius_)/length,
                           (line.dy()*_vertex_radius_)/length);
        _src_point_ = line.p1() + edgeOffset;
        _dest_point_ = line.p2() - edgeOffset;
    }
    else
    {
        _src_point_ = _dest_point_ = line.p1();
    }
    return;
}

void GViewEdge::searchDestination(const QPointF& point)
{
    if(!_src_item_)
    {
        return;
    }
    QLineF line(mapFromItem(_src_item_,0,0),mapFromScene(point));
    qreal length = line.length();

    prepareGeometryChange();
    if(length>qreal(_vertex_radius_))
    {
        QPointF edgeOffset((line.dx()*_vertex_radius_)/length,
                           (line.dy()*_vertex_radius_/length));
        _src_point_ = line.p1()+edgeOffset;
        _dest_point_ = line.p2();
    }
    else
    {
        _src_point_ = _dest_point_ = line.p1();
    }
    return;
}

qreal GViewEdge::length() const
{
    /*
     * QPointF src(mapFromItem(_src_item_,0,0));
     * QPointF dest(mapFromItem(_dest_item_,0,0));
     * qreal dx = dest.x() - src.x();
     * qreal dy = dest.y() - src.y();
    */
    QPointF delta(mapFromItem(_src_item_,0,0) - mapFromItem(_dest_item_,0,0));
    return std::hypot(delta.x(),delta.y());
}

void GViewEdge::fixateLength()
{
    _weight_ = length();
    return;
}

QRectF GViewEdge::boundingRect() const
{
    if(_mode_ == GVedge_incomplete||
            _mode_ == GVedge_deletion)
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
    qreal extra = (EDGE_WIDTH+_arrowSize_)/2.0;
    return QRectF(_src_point_,
                  QSizeF(_dest_point_.x()-_src_point_.x(),
                         _dest_point_.y()-_src_point_.y())
                  ).
            normalized().
            adjusted(-extra,-extra,extra,extra);
}

QPainterPath GViewEdge::shape() const
{
    QPainterPath path(_src_point_);
    QLineF line(_src_point_,_dest_point_);
    if(line.length()>qreal(_vertex_radius_))
    {
        line.setLength(line.length()-qreal(_vertex_radius_)*2);
    }
    path.lineTo(line.p2());
    return path;
}

void GViewEdge::paint(QPainter* painter,
           const QStyleOptionGraphicsItem* option,
           QWidget* widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    if(_mode_ == GVedge_incomplete||
            _mode_ == GVedge_deletion)
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
                        (_mode_==GVedge_deletion)?
                            Qt::darkRed:
                            Qt::black,
                        EDGE_WIDTH,
                        Qt::SolidLine,
                        Qt::RoundCap,
                        Qt::RoundJoin));
    painter->drawLine(line);
    painter->setBrush(Qt::black);
    double angle = std::atan2(-line.dy(),line.dx());
    if(!_directed_ && _mode_==GVedge_regular)
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
