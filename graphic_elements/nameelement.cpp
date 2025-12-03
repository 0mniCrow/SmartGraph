#include "nameelement.h"

NameElement::NameElement(const QString& element_name,
                         char element_type):
    AbstractElement(element_name,element_type),_is_editable_(true)
{
    return;
}

NameElement::NameElement(const QString& element_name,
                         const QString& value,
                         char element_type):
    AbstractElement(element_name,element_type),
    _value_(value), _is_editable_(true)
{
    return;
}

NameElement::NameElement(const QString& element_name,
                         const QVariant& value,
                         char element_type):
    AbstractElement(element_name,element_type),
    _value_(value.toString()),_is_editable_(true)
{
    return;
}

NameElement::~NameElement()
{
    return;
}

QVariant NameElement::value() const
{
    return QVariant(_value_);
}

void NameElement::setValue(const QVariant& new_val,
                           bool inform_signal)
{
    if(new_val.canConvert<QString>())
    {
        _value_ = new_val.toString();
        if(inform_signal)
        {
            emit elementChanged(elementName(),QVariant(_value_));
        }
    }
    return;
}

char NameElement::elementType() const
{
    if(_element_type_==ET_Default)
    {
        return ET_NameElement;
    }
    return _element_type_;
}

QWidget* NameElement::generateWidget()
{
    QWidget* widget;
    if(_is_editable_)
    {
        QLineEdit* l_edit = new QLineEdit(_value_);
        l_edit->setMaxLength(NAME_ELEMENT_MAX_LEN);
        widget = l_edit;
    }
    else
    {
        widget = new QLabel(_value_);
    }
    widget->setObjectName(_element_name_);
    return widget;
}

QString NameElement::internalDataType() const
{
    return QString("QString");
}

void NameElement::changeElement(QVariant new_val, bool inform_signal)
{
    setValue(new_val,inform_signal);
    return;
}

void NameElement::setEditable(bool state)
{
    _is_editable_ = state;
}
