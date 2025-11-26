#include "nodeobjectinfo.h"

NodeObjectInfo::NodeObjectInfo()
{

}
[[nodiscard]] AbstractElement* NodeObjectInfo::createElement(const QString& element_name,
                                       const QVariant& value,
                                       char element_type)
{
    if(isExist(element_name))
    {
        return nullptr;
    }
    AbstractElement* element = nullptr;
    switch(element_type)
    {
    case NameElement::ET_NameElement:
    {
        element = new NameElement(element_name);
    }
        break;
    case ImageElement::ET_PictureElement:
    {
        element = new ImageElement(element_name);
    }
        break;
    case InfoElement::ET_RichTextElement:
    {
        element = new InfoElement(element_name);
    }
        break;
    case DateElement::ET_DateElement:
    {
        element = new DateElement(element_name);
    }
        break;
    default:
    {
        return nullptr;
    }
    }
    element->setValue(value,false);
    return element;
}

void NodeObjectInfo::destroyElement(AbstractElement* element)
{
    delete element;
    return;
}

[[nodiscard]] QWidget* NodeObjectInfo::createInfoWindow()
{
     InfoWidget * widget = new InfoWidget();
     for(AbstractElement* element: _elements_)
     {
         widget->addElement(element->generateWidget());
     }
     return widget;
}

void NodeObjectInfo::destroyInfoWindow()
{

}
