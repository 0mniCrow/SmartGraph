#include "gviewitem.h"

GViewItem::GViewItem(const QString &info,
                     const QColor &color):
    _info_(info),_color_(color),
    _is_hovered_(false),_is_clicked_(false)
{
    setFlags(ItemIsMovable);
    return;
}

GViewItem::GViewItem(const QColor& color):_color_(color),
    _is_hovered_(false),_is_clicked_(false)
{
    setFlags(ItemIsMovable);
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


QRectF GViewItem::boundingRect() const
{
    return QRectF(-20-LINE_WIDTH,-40-LINE_WIDTH,40+LINE_WIDTH,80+LINE_WIDTH );
}

QPainterPath GViewItem::shape() const
{
    QRect workingRect(-20.0,-40.0,40.0,80.0);
    QPainterPath path;
    QPolygon triangle;
    triangle<<workingRect.bottomRight()
           <<QPoint(workingRect.center().x(),0)
          <<workingRect.bottomLeft();
    path.addPolygon(triangle);
    return path;
}

void GViewItem::paint(QPainter* painter,
           const QStyleOptionGraphicsItem* option,
           QWidget* widget)
{
    Q_UNUSED(option) Q_UNUSED(widget)
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing,true);
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
    QRect workingRect(-20.0,-40.0,40.0,80.0);
    QPolygon triangle;
    triangle<<workingRect.bottomRight()
           <<QPoint(workingRect.center().x(),0)
          <<workingRect.bottomLeft();
    painter->drawPolygon(triangle);
    painter->restore();
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
