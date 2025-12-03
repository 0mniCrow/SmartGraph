#include "infoelement.h"

InfoElement::InfoElement(const QString &element_name,
                         char element_type):
    AbstractElement(element_name,element_type),
    _useHTML_(false),
    _readOnly_(false)
{
    return;
}

InfoElement::InfoElement(const QString& element_name,
                         const QString& text, bool formatHTML,
                         char element_type):
    AbstractElement(element_name,element_type),
    _value_(text),
    _useHTML_(formatHTML),
    _readOnly_(false)
{
    return;
}
InfoElement::InfoElement(const QString& element_name,
                         const QVariant& text, bool formatHTML,
                         char element_type):
    AbstractElement(element_name,element_type),
    _useHTML_(formatHTML),
    _readOnly_(false)
{
    if(text.canConvert<QString>())
    {
        _value_ = text.value<QString>();
    }
    return;
}

InfoElement::~InfoElement()
{
    return;
}

QVariant InfoElement::value() const
{
    return QVariant::fromValue(_value_);
}

void InfoElement::setValue(const QVariant& new_val, bool inform_signal)
{
    if(new_val.canConvert<QString>())
    {
        _value_ = new_val.value<QString>();
        if(inform_signal)
            emit elementChanged(elementName(),_value_);
    }
    return;
}

char InfoElement::elementType() const
{
    return ET_RichTextElement;
}

QString InfoElement::internalDataType() const
{
    return QString("QString");
}

QWidget* InfoElement::generateWidget()
{
    QTextEdit* textEditor;
    if(_useHTML_)
    {
        textEditor = new QTextEdit();
        textEditor->setHtml(_value_);
    }
    else
    {
        textEditor = new QTextEdit(_value_);
    }

    textEditor->setReadOnly(_readOnly_);
    textEditor->setObjectName(elementName());
    return textEditor;
}

void InfoElement::changeElement(QVariant new_val, bool inform_signal)
{
    setValue(new_val,inform_signal);
    return;
}

void InfoElement::setEditable(bool state)
{
    _readOnly_ = !state;
}
