#include "gviewedge.h"

GViewEdge::GViewEdge(GViewItem *source,
                     GViewItem *destination,
                     bool directed, GViewEdge_mode mode):
    _src_item_(source), _dest_item_(destination),
    _directed_(directed),_mode_(mode)
{
    //_incomplete_ = false;
    return;
}

GViewEdge::GViewEdge(GViewItem* source,
                     bool direction,
                     GViewEdge_mode mode):
    _src_item_(source),_directed_(direction),
    _mode_(mode)
{
    //_incomplete_ = true;
    _dest_item_ = nullptr;
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
//    if(_incomplete_)
//    {
//        _incomplete_ = false;
//    }
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

void GViewEdge::searchDestination(const QPointF& point)
{
    if(!_src_item_)
    {
        return;
    }
    QLineF line(mapFromItem(_src_item_,0,0),mapFromScene(point));
    qreal length = line.length();

    prepareGeometryChange();
    if(length>qreal(20.0))
    {
        QPointF edgeOffset((line.dx()*10)/length,(line.dy()*10/length));
        _src_point_ = line.p1()+edgeOffset;
        _dest_point_ = line.p2();
    }
    else
    {
        _src_point_ = _dest_point_ = line.p1();
    }
    return;
}

QRectF GViewEdge::boundingRect() const
{
    if(/*_incomplete_*/_mode_ == GVedge_incomplete||
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

    qreal penWidth = 1;
    qreal extra = (penWidth)/2.0;
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
    if(line.length()>20.0)
    {
        line.setLength(line.length()-20.0);
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
    if(/*_incomplete_*/_mode_ == GVedge_incomplete||
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
    return;
}
