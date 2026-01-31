#include "gviewitem.h"
#include "gviewedge.h"
#include "gviewport.h"

GViewItem::GViewItem(int radius,
                     #ifdef INFO_COMPLEX_OBJECT
                     const NodeObjectInfo& info,
                     #else
                     const QString &info,
                     #endif
                     const QColor &color):_info_(info),
    _color_(color),_radius_(radius),
    _flags_(GV_None)
{
    setFlags(ItemSendsGeometryChanges|ItemIsMovable|ItemIsSelectable);
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    setAcceptHoverEvents(true);
    return;
}

#ifdef INFO_COMPLEX_OBJECT
    GViewItem::GViewItem(int radius,
              NodeObjectInfo&& info,
              const QColor& color):_info_(std::move(info)),
        _color_(color),_radius_(radius),
        _flags_(GV_None)
    {
        setFlags(ItemSendsGeometryChanges|ItemIsMovable|ItemIsSelectable);
        setCacheMode(QGraphicsItem::DeviceCoordinateCache);
        setAcceptHoverEvents(true);
        return;
    }
#endif

GViewItem::GViewItem(int radius, const QColor& color):
    _color_(color),_radius_(radius),
    _flags_(GV_None)
{
    setFlags(ItemSendsGeometryChanges|ItemIsMovable|ItemIsSelectable);
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    setAcceptHoverEvents(true);
    return;
}

GViewItem::~GViewItem()
{

}


void GViewItem::checkBorders()
{
    QRectF sceneRect = scene()->sceneRect();
    QPointF newPos = scenePos();
    newPos.setX(qMin(qMax(newPos.x(), sceneRect.left() + _radius_), sceneRect.right() - _radius_));
    newPos.setY(qMin(qMax(newPos.y(), sceneRect.top() + _radius_), sceneRect.bottom() - _radius_));
    setPos(newPos);
    return;
}

void GViewItem::calcForce()
{
    if(!scene() || scene()->mouseGrabberItem() == this||
            !(_flags_&GV_Is_Forced))
    {
        _adv_pos_ = pos();
        return;
    }

    qreal vel_x = 0.0;
    qreal vel_y = 0.0;
    //Пошук зоны каля вяршыні
    QRectF sceneRect = scene()->sceneRect();
    QPointF cur_pos = scenePos();
    QPointF zone_TL,zone_BR;
    double zone_width = _radius_*5;
    zone_TL.setX(qMax(cur_pos.x()-zone_width,sceneRect.left()));
    zone_TL.setY(qMax(cur_pos.y()-zone_width,sceneRect.top()));
    zone_BR.setX(qMin(mapToScene(boundingRect().bottomRight()).x()
                      +zone_width,sceneRect.right()));
    zone_BR.setY(qMin(mapToScene(boundingRect().bottomRight()).y()
                      +zone_width,sceneRect.bottom()));
    QRectF zone_rect(zone_TL,zone_BR);

    //Пошук графічных аб'ектаў у зоне каля вяршыні
    const QList<QGraphicsItem*> items(scene()->items(zone_rect));
    for(QGraphicsItem* item:items)
    {
        GViewItem* g_item = qgraphicsitem_cast<GViewItem*>(item);
        if(!g_item)
        {
            continue;
        }
        QPointF vect = mapToItem(g_item,0.0,0.0);
        qreal dx = vect.x();
        qreal dy = vect.y();
        double len = 2.0 * (std::pow(dx,2.0)+std::pow(dy,2.0));
        if(len>0)
        {
            vel_x+= (dx*150.0)/len;
            vel_y+= (dy*150.0)/len;
        }
    }
    double weight = (_edges_.size()+1) * 10;
    for(const GViewEdge* edge: std::as_const(_edges_))
    {
        QPointF vect;
        if(edge->source()==this)
        {
            vect = mapToItem(edge->destination(),0,0);
        }
        else
        {
            vect = mapToItem(edge->source(),0,0);
        }
        //Тут трэба вымяраць даўжыню рэбра і калі яно даўжэй, дадаваць значэнне
        QPointF delta(mapFromItem(edge->source(),0,0) - mapFromItem(edge->destination(),0,0));
        qreal dist = std::hypot(delta.x(),delta.y());
        //qreal distance = std::sqrt(std::pow(difference.x(), 2) + std::pow(difference.y(), 2));
        if(dist>=edge->edgeWeight())
        {
            vel_x -= vect.x()/weight;
            vel_y -= vect.y()/weight;
        }
    }
    if(qAbs(vel_x)<0.1 && qAbs(vel_y)<0.1)
    {
        vel_x = 0.0;
        vel_y = 0.0;
    }
    _adv_pos_ = pos()+QPointF(vel_x,vel_y);
    _adv_pos_.setX(qMin(qMax(_adv_pos_.x(), sceneRect.left() + _radius_), sceneRect.right() - _radius_));
    _adv_pos_.setY(qMin(qMax(_adv_pos_.y(), sceneRect.top() + _radius_), sceneRect.bottom() - _radius_));
    return;
}

bool GViewItem::advPosition()
{
    if(_adv_pos_ == pos())
    {
        return false;
    }
    setPos(_adv_pos_);
    return true;
}

void GViewItem::setColor(const QColor& color)
{
    _color_=color;
    return;
}

#ifdef INFO_COMPLEX_OBJECT
void GViewItem::setInfo(NodeObjectInfo&& info)
{
    _info_=std::move(info);
    return;
}

const NodeObjectInfo& GViewItem::info()const
{
    return _info_;
}

#else

void GViewItem::setInfo(const QString& info)
{
    _info_=info;
    return;
}

QString GViewItem::info()const
{
    return _info_;
}
#endif







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
    int select_inflate = isSelected()?SELECTED_RISE:0;
    int borders = 0;
    if(_flags_&GV_Is_Clicked)
    {
        borders = LINE_CLICKED_WIDTH;
    }
    else if(isSelected())
    {
        borders = LINE_SELECT_WIDTH;
    }
    else
    {
        borders = LINE_BASE_WIDTH;
    }
    return QRectF(-_radius_ - borders - select_inflate,
                  -_radius_ - borders - select_inflate,
                  _radius_*2 + borders + select_inflate,
                  _radius_*2 + borders+ select_inflate);
            /*QRectF(-_radius_-LINE_WIDTH-select_inflate,
                  -_radius_-LINE_WIDTH-select_inflate,
                  _radius_*2+LINE_WIDTH+select_inflate,
                  _radius_*2+LINE_WIDTH+select_inflate);*/
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
    //int select_inflate = isSelected()?SELECTED_RISE:0;
    QPainterPath path;
    path.addEllipse(/*-_radius_-select_inflate,
                    -_radius_-select_inflate,
                    _radius_*2+select_inflate,
                    _radius_*2+select_inflate*/boundingRect());
    return path;
}

void GViewItem::paint(QPainter* painter,
           const QStyleOptionGraphicsItem* option,
           QWidget* widget)
{
    Q_UNUSED(option) Q_UNUSED(widget)
    painter->save();
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
    if(flags()&ItemIsMovable && _flags_&GV_Is_Clicked)
    {
        cur_pen.setColor(QColorConstants::Svg::darkslateblue);
        cur_pen.setWidthF(LINE_CLICKED_WIDTH);
        cur_color = QColorConstants::Svg::orange;
    }
    else
    {
        if(isUnderMouse())
        {
            cur_pen.setColor(QColorConstants::Svg::yellowgreen);
            cur_pen.setWidthF(LINE_BASE_WIDTH);
            cur_color = (flags()&ItemIsMovable)?
                        Qt::yellow:
                        QColorConstants::Svg::lightcyan;
        }
        else if(isSelected())
        {
            cur_pen.setColor(QColorConstants::Svg::darkolivegreen);
            cur_pen.setWidthF(LINE_SELECT_WIDTH);
            cur_color = (flags()&ItemIsMovable)?
                        QColorConstants::Svg::palegoldenrod:
                        QColorConstants::Svg::lightskyblue;
        }
        else
        {
            cur_pen.setColor(QColorConstants::Svg::black);
            cur_pen.setWidthF(LINE_BASE_WIDTH);
            cur_color = (flags()&ItemIsMovable)?
                        QColorConstants::Svg::slategray:
                        QColorConstants::Svg::powderblue;
        }
    }
    painter->setBrush(cur_color);
    painter->setPen(cur_pen);

//        cur_color.setRed(_color_.red()<235?_color_.red()+20:_color_.red()-20);
//        cur_color.setGreen(_color_.green()<235?_color_.green()+20:_color_.green()-20);
//        cur_color.setBlue(_color_.blue()<235?_color_.blue()+20:_color_.blue()-20);
//        cur_color.setRgbF(_color_.redF()+2.0,
//                          _color_.greenF()+2.0,
//                          _color_.blueF()+2.0);
    QRectF ellipse_rect(-_radius_,-_radius_,_radius_*2,_radius_*2);
    painter->drawEllipse(ellipse_rect/*-_radius_,-_radius_,_radius_*2,_radius_*2*/);
    if(!(flags()&ItemIsMovable))
    {
        QPolygonF pin_needle;
        qreal pin_stem_rad = 220*M_PI/180;
        QPointF pin_stem_end(ellipse_rect.center().x()+_radius_*sin(pin_stem_rad),
                             ellipse_rect.center().y()+_radius_*cos(pin_stem_rad));
        QLineF pin_stem_line(ellipse_rect.center(),pin_stem_end);
        qreal ux = pin_stem_line.dx()/pin_stem_line.length();
        qreal uy = pin_stem_line.dy()/pin_stem_line.length();
        qreal vx = -uy;
        qreal vy = ux;
        QPointF pin_needle_point1(pin_stem_end.x()+PIN_HEAD_RADIUS*vx,
                                  pin_stem_end.y()+PIN_HEAD_RADIUS*vy);
        QPointF pin_needle_point2(pin_stem_end.x()-PIN_HEAD_RADIUS*vx,
                                  pin_stem_end.y()-PIN_HEAD_RADIUS*vy);
        pin_needle<<pin_stem_line.pointAt(0.2)<<pin_needle_point1<<pin_needle_point2;
        painter->setBrush(QBrush(QColorConstants::Svg::gainsboro));
        painter->setPen(QPen(Qt::black,1));
        painter->drawPolygon(pin_needle);
        painter->setBrush(QBrush(Qt::red));
        painter->drawEllipse(pin_stem_end,PIN_HEAD_RADIUS+1,PIN_HEAD_RADIUS+1);
    }
    painter->restore();
}

QVariant GViewItem::itemChange(GraphicsItemChange change, const QVariant& value)
{
    switch(change)
    {
    case ItemPositionHasChanged:
    {
        checkBorders();
        for(GViewEdge* edge:_edges_)
        {
            edge->recalculate();
        }
        if(_flags_&GV_Is_Forced)
        {
            GViewPort* port = dynamic_cast<GViewPort*>(scene()->views().first());
            if(port)
            {
                port->itemMoved();
            }
        }
    }
        break;
    case ItemSelectedHasChanged:
    {
        update();
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


//#ifdef Q_OS_WIN
//    QPoint topLeft     = scene()->views().first()->viewport()->mapToGlobal(QPoint(0,0));
//    QPoint bottomRight = scene()->views().first()->viewport()->mapToGlobal(
//                             QPoint(scene()->views().first()->viewport()->width(),
//                              scene()->views().first()->viewport()->height()));
//    RECT clipRect = {
//        topLeft.x(),
//        topLeft.y(),
//        bottomRight.x(),
//        bottomRight.y()
//    };
//    ::ClipCursor(&clipRect);
//#endif
    if(flags()&ItemIsMovable)
    {
        setCursor(Qt::BlankCursor);
        setGVFlag(GV_Is_Clicked, true);
    }
    update();

    QGraphicsItem::mousePressEvent(m_event);
}
void GViewItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * m_event)
{
    if(flags()&ItemIsMovable)
    {
        if(scene() && !scene()->views().isEmpty())
        {
            QPoint viewPos = scene()->views().first()->mapFromScene(pos());
            QPoint gl_pos = scene()->views().first()->viewport()->mapToGlobal(viewPos);
            QCursor::setPos(gl_pos);
        }
        unsetCursor();
        setGVFlag(GV_Is_Clicked,false);
    }
    update();

//#ifdef Q_OS_WIN
//    ::ClipCursor(nullptr);
//#endif

    QGraphicsItem::mouseReleaseEvent(m_event);
}

void GViewItem::mouseMoveEvent(QGraphicsSceneMouseEvent* m_event)
{
    if(!(flags()&ItemIsMovable))
    {
        m_event->ignore();
        return;
    }
    if(_flags_&GV_Ignore_Next_Move)
    {
        setGVFlag(GV_Ignore_Next_Move,false);
        m_event->ignore();
        return;
    }
    QPointF delta = (m_event->scenePos()-m_event->lastScenePos()) * MOUSE_SENSE_DECR;
    QPointF new_pos(pos()+delta);

    QRect vpRect = scene()->views().first()->viewport()->rect();
    QPoint topLeft     = scene()->views().first()->viewport()->mapToGlobal(vpRect.topLeft());
    QPoint bottomRight = scene()->views().first()->viewport()->mapToGlobal(vpRect.bottomRight());
    QPoint globalPos(QCursor::pos());
    bool wrapped = false;
    if (globalPos.x() <= topLeft.x())
    {
        wrapped = true;
    }
    else if (globalPos.x() >= bottomRight.x())
    {
        wrapped = true;
    }
    if (globalPos.y() <= topLeft.y())
    {
        wrapped = true;
    }
    else if (globalPos.y() >= bottomRight.y())
    {
        wrapped = true;
    }

    if (wrapped)
    {
        QPoint viewPos = scene()->views().first()->mapFromScene(pos());
        QPoint gl_pos = scene()->views().first()->viewport()->mapToGlobal(viewPos);
        QCursor::setPos(gl_pos);
        setGVFlag(GV_Ignore_Next_Move,true);
    }
    setPos(new_pos);

    //QPoint viewPoint = scene()->views().first()->mapFromScene(new_pos);
    //QPoint gl_Point = scene()->views().first()->viewport()->mapToGlobal(viewPoint);
    //QCursor::setPos(gl_Point);
    //setPos(pos()+delta*MOUSE_SENSE_DECR);
    m_event->accept();
    //QGraphicsItem::mouseMoveEvent(m_event);
}

void GViewItem::hoverEnterEvent(QGraphicsSceneHoverEvent * h_event)
{
    update();
    QGraphicsItem::hoverEnterEvent(h_event);
}
void GViewItem::hoverLeaveEvent(QGraphicsSceneHoverEvent * h_event)
{
    update();
    QGraphicsItem::hoverLeaveEvent(h_event);
}

