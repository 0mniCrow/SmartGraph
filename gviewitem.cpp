#include "gviewitem.h"
#include "gviewedge.h"

GViewItem::GViewItem(int radius, const QString &info,
                     const QColor &color):_radius_(radius),
    _info_(info),_color_(color),
    _is_hovered_(false),_is_clicked_(false)
{
    setFlags(ItemSendsGeometryChanges|ItemIsMovable);
    return;
}

GViewItem::GViewItem(int radius, const QColor& color):
    _radius_(radius),_color_(color),
    _is_hovered_(false),_is_clicked_(false)
{
    setFlags(ItemSendsGeometryChanges|ItemIsMovable);
    return;
}

void GViewItem::setColor(const QColor& color)
{
    _color_=color;
    return;
}
void GViewItem::setInfo(const QString& info)
{
    _info_=info;
    return;
}
QString GViewItem::info()const
{
    return _info_;
}
QColor GViewItem::color()const
{
    return _color_;
}

void GViewItem::addEdge(GViewEdge* edge)
{
    _edges_.append(edge);
    edge->recalculate();
    return;
}

void GViewItem::delEdge(GViewEdge* edge)
{
    if(_edges_.count(edge))
    {
        _edges_.erase(std::find(_edges_.cbegin(),_edges_.cend(),edge));
    }
    return;
}

QRectF GViewItem::boundingRect() const
{
    return QRectF(-_radius_-LINE_WIDTH,
                  -_radius_-LINE_WIDTH,
                  _radius_*2+LINE_WIDTH,
                  _radius_*2+LINE_WIDTH);
}

QPainterPath GViewItem::shape() const
{
//    QRect workingRect(-20.0,-40.0,40.0,80.0);
//    QPainterPath path;
//    QPolygon triangle;
//    triangle<<workingRect.bottomRight()
//           <<QPoint(workingRect.center().x(),0)
//          <<workingRect.bottomLeft();
//    path.addPolygon(triangle);
    QPainterPath path;
    path.addEllipse(-_radius_,-_radius_,_radius_*2,_radius_*2);
    return path;
}

void GViewItem::paint(QPainter* painter,
           const QStyleOptionGraphicsItem* option,
           QWidget* widget)
{
    Q_UNUSED(option) Q_UNUSED(widget)
    painter->save();
//    painter->setRenderHint(QPainter::Antialiasing,true);
//    QColor cur_color;
//    QPen cur_pen;
//    if(_is_hovered_)
//    {
//        cur_color.setRgbF(_color_.redF()+2.0,
//                          _color_.greenF()+2.0,
//                          _color_.blueF()+2.0);
//    }
//    else
//    {
//        cur_color = _color_;
//    }
//    painter->setBrush(cur_color);
//    if(_is_clicked_)
//    {
//        cur_pen.setColor(Qt::black);
//        cur_pen.setWidthF(LINE_WIDTH);
//    }
//    else
//    {
//        cur_pen.setColor(Qt::darkGray);
//        cur_pen.setWidthF((LINE_WIDTH>1.0)?LINE_WIDTH-1.0:LINE_WIDTH);
//    }
//    painter->setPen(cur_pen);
//    QRect workingRect(-20.0,-40.0,40.0,80.0);
//    QPolygon triangle;
//    triangle<<workingRect.bottomRight()
//           <<QPoint(workingRect.center().x(),0)
//          <<workingRect.bottomLeft();
//    painter->drawPolygon(triangle);

    painter->setRenderHint(QPainter::Antialiasing,true);
    //painter->drawEllipse(-7,-7,20,20);
    QColor cur_color;
    QPen cur_pen;
    if(_is_hovered_)
    {
        cur_color.setRgbF(_color_.redF()+2.0,
                          _color_.greenF()+2.0,
                          _color_.blueF()+2.0);
    }
    else
    {
        cur_color = _color_;
    }
    painter->setBrush(cur_color);
    if(_is_clicked_)
    {
        cur_pen.setColor(Qt::black);
        cur_pen.setWidthF(LINE_WIDTH);
    }
    else
    {
        cur_pen.setColor(Qt::darkGray);
        cur_pen.setWidthF((LINE_WIDTH>1.0)?LINE_WIDTH-1.0:LINE_WIDTH);
    }
    painter->setPen(cur_pen);
    painter->drawEllipse(-_radius_,-_radius_,_radius_*2,_radius_*2);
    painter->restore();
}

QVariant GViewItem::itemChange(GraphicsItemChange change, const QVariant& value)
{
    switch(change)
    {
    case ItemPositionHasChanged:
    {
        for(GViewEdge* edge:_edges_)
        {
            edge->recalculate();
        }
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

void GViewItem::setRadius(int radius)
{
    prepareGeometryChange();
    _radius_=radius;
    return;
}

void GViewItem::mousePressEvent(QGraphicsSceneMouseEvent * m_event)
{
    _is_clicked_ = true;
    update();
    QGraphicsItem::mousePressEvent(m_event);
}
void GViewItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * m_event)
{
    _is_clicked_ = false;
    update();
    QGraphicsItem::mouseReleaseEvent(m_event);
}
void GViewItem::hoverEnterEvent(QGraphicsSceneHoverEvent * h_event)
{
    _is_hovered_ = true;
    update();
    QGraphicsItem::hoverEnterEvent(h_event);
}
void GViewItem::hoverLeaveEvent(QGraphicsSceneHoverEvent * h_event)
{
    _is_hovered_ = false;
    update();
    QGraphicsItem::hoverLeaveEvent(h_event);
}
