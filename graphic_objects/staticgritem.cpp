#include "staticgritem.h"

const QString StaticGrItem::_object_name_ = "StaticGrItem";

StaticGrItem::StaticGrItem(const item_id_t& id,
                           int radius,
                           QGraphicsObject* tata):
    AbstractGrQtItem(id,radius,tata)
{
    iconUpdate();
    return;
}

/* Метад вызначае й перамалёўвае іконку на аб'екце
   Залежыць ад наяўнасьці усталяванага відарыса й радыюса*/
void StaticGrItem::iconUpdate()
{
    auto communicator = getItemCommunicator();
    if(_orig_pixmap_.isNull() &&
            (!communicator||
             communicator->getDefImage().isNull()))
    {
        return;
    }
    int diameter = getRadius()*2;
    QImage temp_icon(diameter,diameter,QImage::Format_ARGB32_Premultiplied);
    temp_icon.fill(Qt::transparent);
    QPainter painter;
    painter.begin(&temp_icon);
    painter.setRenderHint(QPainter::Antialiasing,true);
    //Карэктыроўка асноўнага відарыса, абгрунтаванага радыюсам
    QPixmap scaled_pxm;
    if(_orig_pixmap_.isNull())
    {
        scaled_pxm = communicator->getDefImage().scaled(QSize(diameter,diameter),Qt::IgnoreAspectRatio);
        setGrItemFlag(GV_Def_Icon,true);
    }
    else
    {
        scaled_pxm = _orig_pixmap_.scaled(QSize(diameter,diameter),Qt::IgnoreAspectRatio);
        setGrItemFlag(GV_Def_Icon,false);
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
void StaticGrItem::drawVertexCircle(QPainter* painter)
{
    auto gr_radius = getRadius();
    QRectF ellipse_rect(-gr_radius,-gr_radius,gr_radius*2,gr_radius*2);
    QColor cur_color;
    QPen cur_pen;
    if(flags()&ItemIsMovable && getGrItemFlags()&GV_Is_Clicked)
    {
        cur_pen.setColor(QColorConstants::Svg::darkslateblue);
        cur_pen.setWidthF(LINE_ITEM_CLICKED_WIDTH);
        cur_color = QColorConstants::Svg::orange;
    }
    else
    {
        if(isUnderMouse())
        {
            cur_pen.setColor(QColorConstants::Svg::yellowgreen);
            cur_pen.setWidthF(LINE_ITEM_BASE_WIDTH);
            cur_color = (flags()&ItemIsMovable)?
                        Qt::yellow:
                        QColorConstants::Svg::lightcyan;
        }
        else if(isSelected())
        {
            cur_pen.setColor(QColorConstants::Svg::darkolivegreen);
            cur_pen.setWidthF(LINE_ITEM_SELECT_WIDTH);
            cur_color = (flags()&ItemIsMovable)?
                        QColorConstants::Svg::palegoldenrod:
                        QColorConstants::Svg::lightskyblue;
        }
        else
        {
            cur_pen.setColor(QColorConstants::Svg::white);
            cur_pen.setWidthF(LINE_ITEM_BASE_WIDTH);
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


void StaticGrItem::drawVertexIcon(QPainter* painter)
{
    auto gr_radius = getRadius();
    auto gr_flags = getGrItemFlags();
    QRectF ellipse_rect(-gr_radius,-gr_radius,gr_radius*2,gr_radius*2);
    QColor mask_color;
    QPen cur_pen;
    if(flags()&ItemIsMovable && gr_flags&GV_Is_Clicked)
    {
        cur_pen.setColor(QColorConstants::Svg::orangered);
        cur_pen.setWidthF(LINE_ITEM_CLICKED_WIDTH);
        mask_color = QColorConstants::Svg::cyan;
        if(!(gr_flags&GV_Def_Icon))
        {
            mask_color.setAlpha(40);
        }
    }
    else
    {
        if(isUnderMouse())
        {
            cur_pen.setColor(QColorConstants::Svg::yellow);
            cur_pen.setWidthF(LINE_ITEM_BASE_WIDTH);
            mask_color = (flags()&ItemIsMovable)?
                        QColorConstants::Svg::wheat:
                        QColorConstants::Svg::tomato;
            if(!(gr_flags&GV_Def_Icon))
            {
                mask_color.setAlpha(50);
            }
        }
        else if(isSelected())
        {
            cur_pen.setColor(QColorConstants::Svg::orange);
            cur_pen.setWidthF(LINE_ITEM_SELECT_WIDTH);
            mask_color = (flags()&ItemIsMovable)?
                        QColorConstants::Svg::gold:
                        QColorConstants::Svg::cornsilk;
            if(!(gr_flags&GV_Def_Icon))
            {
                mask_color.setAlpha(30);
            }
        }
        else
        {
            cur_pen.setColor(QColorConstants::Svg::lightslategrey);
            cur_pen.setWidthF(LINE_ITEM_BASE_WIDTH);
            mask_color = Qt::gray;
            if(!(gr_flags&GV_Def_Icon))
            {
                mask_color.setAlpha(75);
            }
        }
    }
    if(gr_flags&GV_Def_Icon)
    {
        painter->setBrush(mask_color);
        painter->drawEllipse(ellipse_rect);
        painter->drawPixmap(-gr_radius,-gr_radius,_icon_);
        painter->setBrush(Qt::NoBrush);
        painter->setPen(cur_pen);
        painter->drawEllipse(ellipse_rect);
    }
    else
    {
        painter->drawPixmap(-gr_radius,-gr_radius,_icon_);
        painter->setBrush(mask_color);
        painter->setPen(cur_pen);
        painter->drawEllipse(ellipse_rect);
    }
}

void StaticGrItem::drawPinNeedle(QPainter* painter)
{
    auto gr_radius = getRadius();
    QRectF ellipse_rect(-gr_radius,-gr_radius,gr_radius*2,gr_radius*2);
    QPolygonF pin_needle;
    qreal pin_stem_rad = 220*M_PI/180;
    QPointF pin_stem_end(ellipse_rect.center().x()+gr_radius*sin(pin_stem_rad),
                         ellipse_rect.center().y()+gr_radius*cos(pin_stem_rad));
    QLineF pin_stem_line(ellipse_rect.center(),pin_stem_end);
    qreal ux = pin_stem_line.dx()/pin_stem_line.length();
    qreal uy = pin_stem_line.dy()/pin_stem_line.length();
    qreal vx = -uy;
    qreal vy = ux;
    QPointF pin_needle_point1(pin_stem_end.x()+PIN_HEAD_ITEM_RADIUS*vx,
                              pin_stem_end.y()+PIN_HEAD_ITEM_RADIUS*vy);
    QPointF pin_needle_point2(pin_stem_end.x()-PIN_HEAD_ITEM_RADIUS*vx,
                              pin_stem_end.y()-PIN_HEAD_ITEM_RADIUS*vy);
    pin_needle<<pin_stem_line.pointAt(0.2)<<pin_needle_point1<<pin_needle_point2;
    painter->setBrush(QBrush(QColorConstants::Svg::gainsboro));
    painter->setPen(QPen(Qt::black,1));
    painter->drawPolygon(pin_needle);
    painter->setBrush(QBrush(Qt::red));
    painter->drawEllipse(pin_stem_end,PIN_HEAD_ITEM_RADIUS+1,PIN_HEAD_ITEM_RADIUS+1);
    return;
}

void StaticGrItem::paint(QPainter* painter,
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

QRectF StaticGrItem::boundingRect() const
{
    auto gr_radius = getRadius();
    auto gr_flags = getGrItemFlags();
    int select_inflate = isSelected()?SELECTED_ITEM_RISE:0.0;
    double borders = 0;
    if(gr_flags&GV_Is_Clicked)
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
    return QRectF(-gr_radius - borders - select_inflate,
                  -gr_radius - borders - select_inflate,
                  gr_radius*2 + borders + select_inflate,
                  gr_radius*2 + borders+ select_inflate);
}

QPainterPath StaticGrItem::shape() const
{
    QPainterPath path;
    path.addEllipse(boundingRect());
    return path;
}

void StaticGrItem::setImage(const QPixmap& image)
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

void StaticGrItem::setRadius(int radius)
{
    AbstractGrQtItem::setRadius(radius);
    iconUpdate();
}

void StaticGrItem::drawGrObject()
{
    update();
    return;
}

void StaticGrItem::setGrData(const item_data_type& data, dataChangeType gr_type)
{
    QStringItemData::setGrData(data,gr_type);
    emit grItemDataChanged(getGrID());
    return;
}

char StaticGrItem::grObjectType() const noexcept
{
    return StaticItem;
}
QStringView StaticGrItem::getObjectName() const
{
    return _object_name_;
}
