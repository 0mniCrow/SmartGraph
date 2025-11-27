#include "nodeobjectinfo.h"

NodeObjectInfo::NodeObjectInfo():_widget_(nullptr)
{

}

NodeObjectInfo::~NodeObjectInfo()
{
    destroyInfoWindow();
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

void NodeObjectInfo::setReadOnly(bool mode)
{
    if(_widget_)
    {
        _widget_->setReadOnly(mode);
    }

    for(AbstractElement* element: _elements_)
    {
        element->setEditable(!mode);
    }
    return;
}

[[nodiscard]] QWidget* NodeObjectInfo::createInfoWindow()
{
     if(_widget_)
     {
        destroyInfoWindow();
     }
     _widget_ = new InfoWidget();
     connect(_widget_,&InfoWidget::closeRequest,this,&NodeObjectInfo::closeRequest);
     connect(_widget_,&InfoWidget::saveRequest,this,&NodeObjectInfo::saveRequest);
     connect(_widget_,&InfoWidget::elementValueChanged,this,&NodeObjectInfo::widgetValueChanged);
     connect(this,&NodeObjectInfo::elementValueChanged,_widget_,&InfoWidget::catchExternalChange);
     for(AbstractElement* element: _elements_)
     {
         _widget_->addElement(element->generateWidget());
     }
     return _widget_;
}

void NodeObjectInfo::destroyInfoWindow()
{
    disconnect(_widget_,&InfoWidget::closeRequest,this,&NodeObjectInfo::closeRequest);
    disconnect(_widget_,&InfoWidget::saveRequest,this,&NodeObjectInfo::saveRequest);
    disconnect(_widget_,&InfoWidget::elementValueChanged,this,&NodeObjectInfo::widgetValueChanged);
    disconnect(this,&NodeObjectInfo::elementValueChanged,_widget_,&InfoWidget::catchExternalChange);
    _widget_->close();
    delete _widget_;
    _widget_ = nullptr;
    return;
}

void NodeObjectInfo::widgetValueChanged(const QString& element_name,
                                 const QVariant& value)
{
    setValue(element_name,value);
    return;
}

void NodeObjectInfo::externalValueChanged(const QString& element_name,
                                  const QVariant& value)
{
    setValue(element_name,value);
    if(_widget_)
    {
        _widget_->setValue(element_name,value);
    }
    return;
}

void NodeObjectInfo::saveRequest()
{
    QMap<QString,QVariant> elements(_widget_->getValues(true));
    QMap<QString,QVariant>::iterator it = elements.begin();
    while(it!=elements.end())
    {
        setValue(it.key(),it.value());
        ++it;
    }
    return;
}
void NodeObjectInfo::closeRequest()
{
    if(_widget_)
    {
        _widget_->close();
    }
    return;
}
