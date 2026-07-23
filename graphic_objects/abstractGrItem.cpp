#include "abstractGrItem.h"
#include "graphic_objects/abstractgrconnection.h"
AbstractGrItem::AbstractGrItem(const item_id_t &id,
                               int radius,
                               QGraphicsItem *tata):
    AbstractGrInterface(id),
    QGraphicsObject(tata),
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

/* Метад вызначае й перамалёўвае іконку на аб'екце
   Залежыць ад наяўнасьці усталяванага відарыса й радыюса*/

void AbstractGrItem::keepInBorders()
{
    QRectF sceneRect = scene()->sceneRect();
    QPointF newPos = scenePos();
    newPos.setX(qMin(qMax(newPos.x(), sceneRect.left() + _radius_), sceneRect.right() - _radius_));
    newPos.setY(qMin(qMax(newPos.y(), sceneRect.top() + _radius_), sceneRect.bottom() - _radius_));
    setPos(newPos);
    return;
}

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

QRectF AbstractGrItem::boundingRect() const
{
    int select_inflate = isSelected()?SELECTED_ITEM_RISE:0.0;
    double borders = 0;
    if(_flags_&GV_Is_Clicked)
    {
        borders = LINE_ITEM_CLICKED_WIDTH;
    }
    else if(isSelected())
    {
        borders = LINE_ITEM_SELECT_WIDTH;
    }
    else
    {
        borders = LINE_ITEM_BASE_WIDTH;
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
        _communicator_->callEditWindow(this);
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

}

void AbstractGrItem::hoverEnterEvent(QGraphicsSceneHoverEvent * h_event)
{

}

void AbstractGrItem::hoverLeaveEvent(QGraphicsSceneHoverEvent * h_event)
{

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
