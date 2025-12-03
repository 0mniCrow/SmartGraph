#include "nodeobjectinfo.h"

NodeObjectInfo::NodeObjectInfo():_widget_(nullptr)
{

}

NodeObjectInfo::~NodeObjectInfo()
{
    clearWidget();
}

void NodeObjectInfo::clearWidget()
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
    if(_widget_)
    {
        _widget_->close();
        _widget_->deleteElement(element->elementName());
    }
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

[[nodiscard]] QWidget * NodeObjectInfo::getInfoWidget()
{
    if(_widget_)
    {
        return _widget_;
    }
    return createInfoWindow();
}

QList<QVariant> NodeObjectInfo::shortInfo() const
{
    ImageElement* e_icon = nullptr;
    NameElement* f_name = nullptr;
    NameElement* l_name = nullptr;
    for(AbstractElement* element:_elements_)
    {
        if(element->elementType() == ImageElement::ET_PictureElement)
        {
            if(!e_icon)
            {
                e_icon = transElem<ImageElement*>(element);
            }
        }
        else if(element->elementType()==NameElement::ET_NameElement)
        {
            if(!f_name)
            {
                f_name = transElem<NameElement*>(element);
            }
            else if(!l_name)
            {
                l_name = transElem<NameElement*>(element);
            }
        }
    }
    QList<QVariant> answer;
    if(e_icon)
    {
        answer.append(e_icon->value()); //!TODO дадаць метад вяртаючы непасрэдна піскельную мапу
    }
    if(f_name)
    {
        answer.append(f_name->value());
    }
    if(l_name)
    {
        answer.append(l_name->value());
    }
    return answer;
}

QList<QVariant> NodeObjectInfo::fullInfo() const
{
    QList<QVariant> answer;
    for(const AbstractElement* element:_elements_)
    {
        answer.append(element->value());
    }
    return answer;
}

void NodeObjectInfo::resetWidget()
{
    clearWidget();
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
    clearWidget();
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
