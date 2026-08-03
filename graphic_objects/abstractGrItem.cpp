#include "abstractGrItem.h"
#include "graphic_objects/abstractgrconnection.h"
AbstractGrItem::AbstractGrItem(const item_id_t &id,
                               int radius,
                               QGraphicsObject *tata):
    QGraphicsObject(tata),
    AbstractGrInterface(id),
    _radius_(radius)
{
    setFlags(ItemSendsGeometryChanges|ItemIsMovable|ItemIsSelectable);
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    setAcceptHoverEvents(true);
    iconUpdate();
}

AbstractGrItem::~AbstractGrItem()
{

}

void AbstractGrItem::keepInBorders()
{
    QRectF sceneRect = scene()->sceneRect();
    QPointF newPos = scenePos();
    newPos.setX(qMin(qMax(newPos.x(), sceneRect.left() + _radius_), sceneRect.right() - _radius_));
    newPos.setY(qMin(qMax(newPos.y(), sceneRect.top() + _radius_), sceneRect.bottom() - _radius_));
    setPos(newPos);
    return;
}

/* Метад вызначае й перамалёўвае іконку на аб'екце
   Залежыць ад наяўнасьці усталяванага відарыса й радыюса*/
void AbstractGrItem::iconUpdate()
{
    if(_orig_pixmap_.isNull() &&
            (!_communicator_||
             _communicator_->getDefImage().isNull()))
    {
        return;
    }
    int diameter = _radius_*2;
    QImage temp_icon(diameter,diameter,QImage::Format_ARGB32_Premultiplied);
    temp_icon.fill(Qt::transparent);
    QPainter painter;
    painter.begin(&temp_icon);
    painter.setRenderHint(QPainter::Antialiasing,true);
    //Карэктыроўка асноўнага відарыса, абгрунтаванага радыюсам
    QPixmap scaled_pxm;
    if(_orig_pixmap_.isNull())
    {
        scaled_pxm = _communicator_->getDefImage().scaled(QSize(diameter,diameter),Qt::IgnoreAspectRatio);
        setGrFlag(GV_Def_Icon,true);
    }
    else
    {
        scaled_pxm = _orig_pixmap_.scaled(QSize(diameter,diameter),Qt::IgnoreAspectRatio);
        setGrFlag(GV_Def_Icon,false);
    }
    painter.drawPixmap(0,0,scaled_pxm);
    painter.end();
    //Ствараецца маска для адсячэння акружнасці
    QImage result_icon(diameter,diameter,QImage::Format_ARGB32_Premultiplied);
    result_icon.fill(Qt::transparent);
    painter.begin(&result_icon);
    QPainterPath cut_mask;
    cut_mask.addEllipse(0,0,diameter,diameter);
    painter.setClipPath(cut_mask);
    painter.drawImage(temp_icon.rect(),temp_icon);
    painter.end();
    _icon_ = QPixmap::fromImage(result_icon);
    return;
}

void AbstractGrItem::drawVertexCircle(QPainter* painter)
{
    QRectF ellipse_rect(-_radius_,-_radius_,_radius_*2,_radius_*2);
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
            cur_pen.setColor(QColorConstants::Svg::white);
            cur_pen.setWidthF(LINE_BASE_WIDTH);
            cur_color = (flags()&ItemIsMovable)?
                        QColorConstants::Svg::slategray:
                        QColorConstants::Svg::powderblue;
        }
    }
    painter->setBrush(cur_color);
    painter->setPen(cur_pen);

    painter->drawEllipse(ellipse_rect);
    return;
}

void AbstractGrItem::drawVertexIcon(QPainter* painter)
{
    QRectF ellipse_rect(-_radius_,-_radius_,_radius_*2,_radius_*2);
    QColor mask_color;
    QPen cur_pen;
    if(flags()&ItemIsMovable && _flags_&GV_Is_Clicked)
    {
        cur_pen.setColor(QColorConstants::Svg::orangered);
        cur_pen.setWidthF(LINE_CLICKED_WIDTH);
        mask_color = QColorConstants::Svg::cyan;
        if(!(_flags_&GV_Def_Icon))
        {
            mask_color.setAlpha(40);
        }
    }
    else
    {
        if(isUnderMouse())
        {
            cur_pen.setColor(QColorConstants::Svg::yellow);
            cur_pen.setWidthF(LINE_BASE_WIDTH);
            mask_color = (flags()&ItemIsMovable)?
                        QColorConstants::Svg::wheat:
                        QColorConstants::Svg::tomato;
            if(!(_flags_&GV_Def_Icon))
            {
                mask_color.setAlpha(50);
            }
        }
        else if(isSelected())
        {
            cur_pen.setColor(QColorConstants::Svg::orange);
            cur_pen.setWidthF(LINE_SELECT_WIDTH);
            mask_color = (flags()&ItemIsMovable)?
                        QColorConstants::Svg::gold:
                        QColorConstants::Svg::cornsilk;
            if(!(_flags_&GV_Def_Icon))
            {
                mask_color.setAlpha(30);
            }
        }
        else
        {
            cur_pen.setColor(QColorConstants::Svg::lightslategrey);
            cur_pen.setWidthF(LINE_BASE_WIDTH);
            mask_color = Qt::gray;
            if(!(_flags_&GV_Def_Icon))
            {
                mask_color.setAlpha(75);
            }
        }
    }
    if(_flags_&GV_Def_Icon)
    {
        painter->setBrush(mask_color);
        painter->drawEllipse(ellipse_rect);
        painter->drawPixmap(-_radius_,-_radius_,_icon_);
        painter->setBrush(Qt::NoBrush);
        painter->setPen(cur_pen);
        painter->drawEllipse(ellipse_rect);
    }
    else
    {
        painter->drawPixmap(-_radius_,-_radius_,_icon_);
        painter->setBrush(mask_color);
        painter->setPen(cur_pen);
        painter->drawEllipse(ellipse_rect);
    }
}

void AbstractGrItem::drawPinNeedle(QPainter* painter)
{
    QRectF ellipse_rect(-_radius_,-_radius_,_radius_*2,_radius_*2);
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
    return;
}

/*Метад вызначае новую пазіцыю аб'екта з улікам
 * штучнага запавольвання (калі карыстальнік націсквае
 * на аб'ект і перасоўвае яго). У выпадку, калі рэальны (нябачны)
 * курсор выйшаў за мяжы віджэта (wrapped), метад пераносіць курсор
 * назад да аб'екта.*/
void AbstractGrItem::calculateObjectPosition(const QPointF &event_pos, const QPointF &prev_pos)
{
    QPointF delta = (event_pos-prev_pos) * MOUSE_SENSE_ITEM_DECR;
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
        setGrFlag(GV_Ignore_Next_Move,true);
    }
    setPos(new_pos);
    return;
}

QRectF AbstractGrItem::boundingRect() const
{
    int select_inflate = isSelected()?SELECTED_ITEM_RISE:0.0;
    double borders = 0;
    if(_flags_&GV_Is_Clicked)
    {
        borders = LINE_ITEM_CLICKED_WIDTH;              //Памер для націснутага элемента
    }
    else if(isSelected())
    {
        borders = LINE_ITEM_SELECT_WIDTH;               //Памер для вызначанага элемента
    }
    else
    {
        borders = LINE_ITEM_BASE_WIDTH;                 //Стандартны памер
    }
    return QRectF(-_radius_ - borders - select_inflate,
                  -_radius_ - borders - select_inflate,
                  _radius_*2 + borders + select_inflate,
                  _radius_*2 + borders+ select_inflate);
}

QPainterPath AbstractGrItem::shape() const
{
    QPainterPath path;
    path.addEllipse(boundingRect());
    return path;
}

QVariant AbstractGrItem::itemChange(GraphicsItemChange change, const QVariant& value)
{
    switch(change)
    {
    case ItemPositionHasChanged:
    {
        keepInBorders();
        for(AbstractGrConnection* edge:_edges_)
        {
            edge->recalculate();
        }
        if(_flags_&GV_Is_Forced)
        {
            if(_communicator_)
            {
                _communicator_->itemIsMoved();
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

void AbstractGrItem::paint(QPainter* painter,
           const QStyleOptionGraphicsItem* option,
           QWidget* widget)
{
    Q_UNUSED(option) Q_UNUSED(widget)
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing,true);
    if(_icon_.isNull())
    {
        drawVertexCircle(painter);
    }
    else
    {
        drawVertexIcon(painter);
    }
    if(!(flags()&ItemIsMovable))
    {
        drawPinNeedle(painter);
    }
    painter->restore();
    return;
}

void AbstractGrItem::mousePressEvent(QGraphicsSceneMouseEvent * m_event)
{
    if(flags()&ItemIsMovable)
    {
        setCursor(Qt::BlankCursor);
        setGrFlag(GV_Is_Clicked, true);
        _last_screen_pos_ = QPoint();
        if(_communicator_)
        {
            _communicator_->stopToolTipTimer();
        }
    }
    update();

    QGraphicsItem::mousePressEvent(m_event);
}

void AbstractGrItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* m_event)
{
    if(_communicator_)
    {
        _communicator_->callEditWindow(this, m_event->screenPos());
    //callEditWindow(m_event);
    }
    m_event->accept();
    return;
}

void AbstractGrItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * m_event)
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
        setGrFlag(GV_Is_Clicked,false);
    }
    update();
    QGraphicsItem::mouseReleaseEvent(m_event);
}

void AbstractGrItem::mouseMoveEvent(QGraphicsSceneMouseEvent* m_event)
{
    if(!(flags()&ItemIsMovable))
    {
        m_event->ignore();
        return;
    }
    if(_flags_&GV_Ignore_Next_Move)
    {
        setGrFlag(GV_Ignore_Next_Move,false);
        m_event->ignore();
        return;
    }
    calculateObjectPosition(m_event->scenePos(),m_event->lastScenePos());
    /*
    QPointF delta = (m_event->scenePos()-m_event->lastScenePos()) * MOUSE_SENSE_ITEM_DECR;
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
        setGrFlag(GV_Ignore_Next_Move,true);
    }
    setPos(new_pos);
    */
    m_event->accept();
    return;
}

void AbstractGrItem::hoverEnterEvent(QGraphicsSceneHoverEvent * h_event)
{
    _last_screen_pos_ = h_event->screenPos();
    if(_communicator_)
    {
        _communicator_->startToolTipTimer(this,h_event->screenPos());
    }
    //startTipTimer();
    update();
    return QGraphicsItem::hoverEnterEvent(h_event);
}

void AbstractGrItem::hoverLeaveEvent(QGraphicsSceneHoverEvent * h_event)
{
    _last_screen_pos_ = QPoint();
    if(_communicator_)
    {
        _communicator_->stopToolTipTimer();
    }
    //breakTipTimer();
    update();
    return QGraphicsItem::hoverLeaveEvent(h_event);
}

void AbstractGrItem::setItemCommunicator(ItemCommunicator* communicator)
{
    if(!communicator)
    {
        return;
    }
    _communicator_ = communicator;
    return;
}

void AbstractGrItem::setRadius(int radius)
{
    if(radius==_radius_)
    {
        return;
    }
    prepareGeometryChange();
    if(radius>=MIN_ITEM_RADIUS)
    {
        _radius_=radius;
    }
    else
    {
        _radius_=MIN_ITEM_RADIUS;
    }
    iconUpdate();
}

void AbstractGrItem::setGrFlag(char flag, bool state)
{
    _flags_ = state? _flags_|flag : _flags_&~flag;
    return;
}

void AbstractGrItem::setGrFlags(char flags)
{
    _flags_ = flags;
    return;
}

void AbstractGrItem::addEdge(AbstractGrConnection* edge)
{
    _edges_.append(edge);
    edge->recalculate();
    return;
}

void AbstractGrItem::delEdge(AbstractGrConnection* edge)
{
    if(_edges_.count(edge))
    {
        _edges_.erase(std::find(_edges_.cbegin(),_edges_.cend(),edge));
    }
    return;
}

QList<AbstractGrConnection*> AbstractGrItem::getEdges() const
{
    QList<AbstractGrConnection*> list(_edges_);
    list.detach();
    return list;
}

void AbstractGrItem::setImage(const QPixmap& image)
{
    if(image.isNull())
    {
        return;
    }
    _orig_pixmap_=image;
    iconUpdate();
    update();
    return;
}

void AbstractGrItem::setGrX(coord_real x)
{
//    QPointF sc_pos(mapToScene(pos()));
//    sc_pos.setX(x);
//    setPos(mapFromScene(sc_pos));
    setX(x);
    return;
}

void AbstractGrItem::setGrY(coord_real y)
{
//    QPointF sc_pos(mapToScene(pos()));
//    sc_pos.setY(y);
//    setPos(mapFromScene(sc_pos));
    setY(y);
    return;
}

coord_real AbstractGrItem::getGrX() const
{
    //QPointF sc_pos(mapToScene(pos()));
    return x();//sc_pos.x();
}

coord_real AbstractGrItem::getGrY() const
{
    //QPointF sc_pos(mapToScene(pos()));
    return y();//sc_pos.y();
}

void AbstractGrItem::setGrWidth(coord_real width)
{
    setRadius(static_cast<int>(width/2));
    return;
}

void AbstractGrItem::setGrHeight(coord_real height)
{
    setRadius(static_cast<int>(height/2));
    return;
}

coord_real AbstractGrItem::getGrWidth() const
{
    return boundingRect().width();
}

coord_real AbstractGrItem::getGrHeight() const
{
    return boundingRect().height();
}

void AbstractGrItem::moveGr(coord_real x, coord_real y)
{
    setPos(x,y);
    return;
}

void AbstractGrItem::drawGr()
{
    update();
    return;
}

void AbstractGrItem::calcForce()
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
    for(const AbstractGrConnection* edge: std::as_const(_edges_))
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
        if(dist>=edge->grWeight())
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

bool AbstractGrItem::advPosition()
{
    if(_adv_pos_ == pos())
    {
        return false;
    }
    setPos(_adv_pos_);
    return true;
}
