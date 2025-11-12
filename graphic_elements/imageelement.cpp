#include "imageelement.h"

ImageElement::ImageElement(const QString& element_name,
                           char element_type):
    AbstractElement(element_name,element_type)
{
    return;
}
ImageElement::ImageElement(const QString& element_name,
                           const QPixmap& value,
                           char element_type):
    AbstractElement(element_name,element_type),_value_(value)
{
    return;
}

ImageElement::ImageElement(const QString& element_name,
                           const QVariant& value,
                           char element_type):
    AbstractElement(element_name,element_type)
{
    if(value.canConvert<QPixmap>())
    {
        _value_ = value.value<QPixmap>();
    }
    return;
}

ImageElement::~ImageElement()
{
    return;
}

QVariant ImageElement::value() const
{
    return QVariant::fromValue(_value_);
}

void ImageElement::setValue(const QVariant& new_val, bool inform_signal)
{
    if(new_val.canConvert<QPixmap>())
    {
        _value_ = new_val.value<QPixmap>();
        if(inform_signal)
        {
            emit elementChanged(elementName(),QVariant::fromValue(_value_));
        }
    }
    return;
}

char ImageElement::elementType() const
{
    if(_element_type_==ET_Default)
    {
        return ET_PictureElement;
    }
    return _element_type_;
}
QWidget* ImageElement::generateWidget()
{
    QLabel* picture = new QLabel();
    picture->setPixmap(_value_);
    picture->setObjectName(_element_name_);
    return picture;
}

QWidget* ImageElement::generatePic(char picture_type)
{
    if(picture_type == PT_Default)
    {
        return generateWidget();
    }

    QLabel* picture = new QLabel();
    bool square = _value_.height()==_value_.width();
    QPixmap sourcePic;
    if(!square)
    {
        int scale_val = std::min(_value_.height(),_value_.width());
        sourcePic = _value_.scaled(scale_val,scale_val,Qt::IgnoreAspectRatio);
    }
    else
    {
        sourcePic = _value_;
    }

    if(picture_type == PT_RoundIcon||
            picture_type == PT_SquareIcon ||
            picture_type == PT_TriangleIcon)
    {
        picture->setFixedSize(ICON_SIZE,ICON_SIZE);
        sourcePic = sourcePic.scaled(ICON_SIZE,ICON_SIZE,Qt::KeepAspectRatio);
    }
    QImage resultPic(sourcePic.size(),QImage::Format_ARGB32_Premultiplied);
    resultPic.fill(Qt::transparent);
    QPainter painter(&resultPic);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(QColor(255,0,0)));
    QRect rect = sourcePic.rect();
    switch(picture_type)
    {
    case PT_RoundIcon:
    case PT_RoundPic:
    {
        painter.drawEllipse(rect.center(),rect.width()/2,rect.height()/2);
    }
        break;
    case PT_SquareIcon:
    case PT_SquarePic:
    {
        painter.drawRect(rect);
    }
        break;
    case PT_TriangleIcon:
    case PT_TrianglePic:
    {
        painter.drawPolygon(
                        QPolygon()<<
                        rect.bottomLeft()<<
                        QPoint(rect.center().x(),0)<<
                        rect.bottomRight());
    }
        break;
    default:
    {

    }
    }
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.drawPixmap(0,0,sourcePic);
    painter.end();
    picture->setPixmap(QPixmap::fromImage(resultPic));
    picture->setObjectName(_element_name_);
    return picture;
}

void ImageElement::setImage(const QImage& image, const QRect& rect,bool inform_signal)
{
    QPixmap pixmap(QPixmap::fromImage(image));
    if(!rect.isNull())
    {
        _value_ = pixmap.copy(rect);
    }
    else
    {
        _value_ = std::move(pixmap);
    }
    if(inform_signal)
    {
        emit elementChanged(elementName(),QVariant::fromValue(_value_));
    }
    return;
}

void ImageElement::setImage(const QPixmap& pixmap, int side_size, bool keep_aspect_ratio, bool inform_signal)
{
    if(pixmap.height()==side_size && pixmap.width()==side_size)
    {
        _value_ = pixmap;
        if(inform_signal)
        {
            emit elementChanged(elementName(),QVariant::fromValue(_value_));
        }
        return;
    }

    _value_ = pixmap.scaled(side_size,side_size,
                            keep_aspect_ratio? Qt::KeepAspectRatio:
                                              Qt::IgnoreAspectRatio,
                            Qt::SmoothTransformation);
    if(inform_signal)
    {
        emit elementChanged(elementName(),QVariant::fromValue(_value_));
    }
    return;
}

void ImageElement::changeElement(QVariant new_val, bool inform_signal)
{
    setValue(new_val,inform_signal);
    return;
}

void ImageElement::setEditable(bool state)
{
    Q_UNUSED(state);
    return;
}
