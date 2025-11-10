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
    //!Зрабіць: Скарыстацца класам imagecropwindow каб стварыць спецыяльныя абсячэнні.
    switch(picture_type)
    {
    case PT_Default:
    {

    }
        break;
    default:
    {

    }
    }
    return nullptr;
}
void ImageElement::setImage(const QImage& image, const QRect& rect)
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
    return;
}
void ImageElement::setImage(const QPixmap& pixmap, qreal radius, const QPointF& central_point)
{
    //!TODO:
    return;
}

void ImageElement::changeElement(QVariant new_val, bool inform_signal)
{
    //!TODO:
    return;
}

void ImageElement::setEditable(bool state)
{
    Q_UNUSED(state);
    return;
}
