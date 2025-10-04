#include "gviewitem.h"
#include "gviewedge.h"

GViewItem::GViewItem(int radius, const QString &info,
                     const QColor &color):_radius_(radius),
    _info_(info),_color_(color),
    _flags_(GV_None)
{
    setFlags(ItemSendsGeometryChanges|ItemIsMovable|ItemIsSelectable);
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    setAcceptHoverEvents(true);
    return;
}

GViewItem::GViewItem(int radius, const QColor& color):
    _radius_(radius),_color_(color),
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
    if(_flags_&GV_Is_Clicked)
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
            cur_color = Qt::yellow;
        }
        else if(isSelected())
        {
            cur_pen.setColor(QColorConstants::Svg::darkolivegreen);
            cur_pen.setWidthF(LINE_SELECT_WIDTH);
            cur_color = QColorConstants::Svg::palegoldenrod;
        }
        else
        {
            cur_pen.setColor(QColorConstants::Svg::black);
            cur_pen.setWidthF(LINE_BASE_WIDTH);
            cur_color = QColorConstants::Svg::slategray;
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

    painter->drawEllipse(-_radius_,-_radius_,_radius_*2,_radius_*2);
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

void GViewItem::contextMenuEvent(QGraphicsSceneContextMenuEvent* c_event)
{
    QMenu* menu = new QMenu;
    QAction* action_pin = menu->addAction(QIcon::fromTheme(
                          (flags()&ItemIsMovable)?"call-start":"call-stop"),
                          (flags()&ItemIsMovable)?"Прычапіць":"Адчапіць");
    action_pin->setCheckable(true);
    action_pin->setChecked(!(flags()&ItemIsMovable));

    QAction* selectedAction = menu->exec(c_event->screenPos());
    if(selectedAction)
    {
        if(!selectedAction->isChecked())
        {
            selectedAction->setChecked(false);
            setFlag(ItemIsMovable,true);
            selectedAction->setText("Прычапіць");
            selectedAction->setIcon(QIcon::fromTheme("call-start"));
        }
        else
        {
            selectedAction->setChecked(true);
            setFlag(ItemIsMovable,false);
            selectedAction->setText("Адчапіць");
            selectedAction->setIcon(QIcon::fromTheme("call-stop"));
        }
    }
    menu->deleteLater();
    QGraphicsItem::contextMenuEvent(c_event);
    return;
}
