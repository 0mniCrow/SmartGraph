#include "abstractelement.h"

AbstractElement::AbstractElement(
        const QString &element_name,
        ElementType element_type):
    _element_name_(element_name),
    _element_type_(element_type)
{
    return;
}
AbstractElement::~AbstractElement()
{
    emit elementDestroyed(_element_name_);
    return;
}

QString AbstractElement::elementName() const
{
    return _element_name_;
}

void AbstractElement::setElementName(const QString& new_name)
{
    _element_name_ = new_name;
    return;
}

AbstractElement::ElementType AbstractElement::elementType() const
{
    return _element_type_;
}

void AbstractElement::setElementType(ElementType type)
{
    _element_type_ = type;
    return;
}
