#include "abstractobjectinfo.h"

AbstractObjectInfo::AbstractObjectInfo():
    _active_element_(EMPTY_ACTIVE_ELEMENT),
    _info_widget_(nullptr)
{
    return;
}

AbstractObjectInfo::AbstractObjectInfo(std::initializer_list<AbstractElement*> list):
    _elements_(list),_active_element_(_elements_.size()?0:EMPTY_ACTIVE_ELEMENT),
    _info_widget_(nullptr)
{
    return;
}

void AbstractObjectInfo::clear()
{
    if(_elements_.size())
    {
        for(AbstractElement* elem:_elements_)
        {
             destroyElement(elem);
        }
        _elements_.clear();
    }
    return;
}

AbstractElement* AbstractObjectInfo::findElement(const QString& el_name)const
{
    auto it = std::find_if(_elements_.cbegin(),
                           _elements_.cend(),
                           [el_name](AbstractElement* element)
    {return element->elementName()==el_name;});
    if(it==_elements_.cend())
    {
        return nullptr;
    }
    return *it;
}

int AbstractObjectInfo::findElementNum(const QString& el_name) const
{
    for(int i = 0; i<_elements_.size();i++)
    {
        if(_elements_.at(i)->elementName()==el_name)
        {
            return i;
        }
    }
    return -1;
}

bool AbstractObjectInfo::inSize(int num) const
{
    return !(num<0 || num>=_elements_.size());
}

int AbstractObjectInfo::size() const
{
    return _elements_.size();
}

bool AbstractObjectInfo::isExist(const QString& element_name) const
{
    return findElement(element_name);
}

QVariant AbstractObjectInfo::at(int num) const
{
    if(!inSize(num))
    {
        return QVariant();
    }
    return _elements_.at(num)->value();
}

QVariant AbstractObjectInfo::at(const QString& element_name) const
{
    AbstractElement* elem = findElement(element_name);
    if(!elem)
    {
        return QVariant();
    }
    return elem->value();
}

QString AbstractObjectInfo::nameAt(int num) const
{
    if(!inSize(num))
    {
        return QString();
    }
    return _elements_.at(num)->elementName();
}

AbstractElement* AbstractObjectInfo::elementAt(int num) const
{
    if(!inSize(num))
    {
        return nullptr;
    }
    return _elements_.at(num);
}

AbstractElement* AbstractObjectInfo::elementAt(const QString& element_name) const
{
    return findElement(element_name);
}

bool AbstractObjectInfo::append(const QString& element_name,
                                const QVariant& value,
                                char element_type)
{
    if(findElement(element_name))
    {
        return false;
    }
    AbstractElement * new_element(createElement(element_name,value,element_type));
    if(new_element)
    {
        if(_active_element_<0)
        {
            _active_element_=0;
        }
        _elements_.append(new_element);
        return true;
    }
    return false;
}

bool AbstractObjectInfo::appendAt(int num,
                 const QString& element_name,
                 const QVariant& value,
                 char element_type)
{
    if(!inSize(num))
    {
        return false;
    }
    if(findElement(element_name))
    {
        return false;
    }
    AbstractElement * new_element(createElement(element_name,value,element_type));
    if(new_element)
    {
        _elements_.insert(num,new_element);
        return true;
    }
    return false;
}
bool AbstractObjectInfo::appendAfter(const QString& element_name,
                 const QString& new_element_name,
                 const QVariant& value,
                 char element_type)
{
    if(findElement(new_element_name))
    {
        return false;
    }
    int elem = findElementNum(element_name);
    if(elem<0)
    {
        return false;
    }
    if((elem+1)==size())
    {
        return append(new_element_name,value,element_type);
    }
    AbstractElement * new_element(createElement(element_name,value,element_type));
    if(new_element)
    {
        _elements_.insert(elem+1,new_element);
        return true;
    }
    return false;
}

bool AbstractObjectInfo::destroyAt(int num)
{
    if(!inSize(num))
    {
        return false;
    }
    AbstractElement* elem = _elements_.at(num);
    if(size()==1)
    {
        _active_element_ = EMPTY_ACTIVE_ELEMENT;
    }
    else if(num<=_active_element_)
    {
        --_active_element_;
    }
    delete elem;
    _elements_.removeAt(num);
    return true;
}
bool AbstractObjectInfo::destroy(const QString& element_name)
{
    auto it = std::find_if(_elements_.cbegin(),
                           _elements_.cend(),
                           [&element_name](AbstractElement* elem)
    {
            return elem->elementName()==element_name;});

    if(it==_elements_.cend())
    {
        return false;
    }
    int index = _elements_.indexOf(*it);
    if(size()==1)
    {
        _active_element_ = EMPTY_ACTIVE_ELEMENT;
    }
    else if(index<=_active_element_)
    {
        --_active_element_;
    }
    delete *it;
    _elements_.erase(it);
    return true;
}

bool AbstractObjectInfo::setValue(const QString& element_name, const QVariant& new_val)
{
    AbstractElement * elem = findElement(element_name);
    if(!elem)
    {
        return false;
    }
    elem->setValue(new_val);
    return true;
}

bool AbstractObjectInfo::setValue(int num, const QVariant& new_val)
{
    if(!inSize(num))
    {
        return false;
    }
    _elements_.at(num)->setValue(new_val);
    return true;
}

QVariant AbstractObjectInfo::operator[](int num)
{
    return _elements_.at(num)->value();
    //Небяспечны метад - выкарыстоўваць з увагай
}

QVariant AbstractObjectInfo::operator ()(const QString& element_name)
{
    return findElement(element_name)->value();
    //Небяспечны метад - выкарыстоўваць з вельмі ўважлівай увагай
}

QMap<int,QString> AbstractObjectInfo::getNames() const
{
    QMap<int,QString> answer;
    if(!size())
    {
        return answer;
    }
    for(int i = 0; i<_elements_.size();i++)
    {
        answer.insert(i,_elements_.at(i)->elementName());
    }
    return answer;
}

bool AbstractObjectInfo::changeName(const QString& cur_name, const QString& new_name)
{
    if(findElement(new_name))
    {
        return false;
    }
    AbstractElement* elem = findElement(cur_name);
    if(!elem)
    {
        return false;
    }
    elem->setElementName(new_name);
    return true;
}

bool AbstractObjectInfo::changeName(int num, const QString& new_name)
{
    if(!inSize(num))
    {
        return false;
    }
    if(findElement(new_name))
    {
        return false;
    }
    _elements_.at(num)->setElementName(new_name);
    return true;
}

char AbstractObjectInfo::widgetType(const QString& element_name) const
{
    AbstractElement* elem = findElement(element_name);
    if(!elem)
    {
        return AbstractElement::ET_Error;
    }
    return elem->elementType();
}

char AbstractObjectInfo::widgetType(int num) const
{
    if(!inSize(num))
    {
        return AbstractElement::ET_Error;
    }
    return _elements_.at(num)->elementType();
}

bool AbstractObjectInfo::setWidgetType(const QString& element_name, char element_type)
{
    AbstractElement* elem = findElement(element_name);
    if(!elem)
    {
        return false;
    }
    elem->setElementType(element_type);
    return true;
}

bool AbstractObjectInfo::setWidgetType(int num, char element_type)
{
    if(!inSize(num))
    {
        return false;
    }
    _elements_[num]->setElementType(element_type);
    return true;
}

int AbstractObjectInfo::curElementNum() const
{
    return _active_element_;
}

QVariant AbstractObjectInfo::curValue() const
{
    if(!inSize(_active_element_))
    {
        return QVariant();
    }
    return _elements_.at(_active_element_)->value();
}

QString AbstractObjectInfo::curElementName() const
{
    if(!inSize(_active_element_))
    {
        return QString();
    }
    return _elements_.at(_active_element_)->elementName();
}

char AbstractObjectInfo::curElementType() const
{
    if(!inSize(_active_element_))
    {
        return AbstractElement::ET_Error;
    }
    return _elements_.at(_active_element_)->elementType();
}

AbstractElement* AbstractObjectInfo::curElement() const
{
    if(!inSize(_active_element_))
    {
        return nullptr;
    }
    return _elements_.at(_active_element_);
}

AbstractObjectInfo& AbstractObjectInfo::operator++()
{
    if((_active_element_+1)<size())
    {
        ++_active_element_;
    }
    return *this;
}

AbstractObjectInfo& AbstractObjectInfo::operator--()
{
    if((_active_element_-1)>=0)
    {
        --_active_element_;
    }
    return *this;
}

AbstractObjectInfo& AbstractObjectInfo::operator+(int len)
{
    if((_active_element_+len)<size())
    {
        _active_element_+=len;
    }
    else
    {
        _active_element_=size()?size()-1:-1;
    }
    return *this;
}

AbstractObjectInfo& AbstractObjectInfo::operator-(int len)
{
    if((_active_element_+len)>=0)
    {
        _active_element_-=len;
    }
    else
    {
        _active_element_=size()?0:-1;
    }
    return *this;
}

bool AbstractObjectInfo::swapElements(const QString& first_name, const QString& sec_name)
{
    if(first_name == sec_name)
    {
        return false;
    }
    int elem_1 = findElementNum(first_name);
    if(elem_1<0)
    {
        return false;
    }
    int elem_2 = findElementNum(sec_name);
    if(elem_2<0)
    {
        return false;
    }
    _elements_.swapItemsAt(elem_1,elem_2);
    return true;
}

bool AbstractObjectInfo::swapElements(int first_num, int sec_num)
{
    if(first_num == sec_num)
    {
        return false;
    }
    if((!inSize(first_num)) || (!inSize(sec_num)))
    {
        return false;
    }
    _elements_.swapItemsAt(first_num,sec_num);
    return true;
}

QWidget *AbstractObjectInfo::getInfoWidget() const
{
    return _info_widget_;
}

bool AbstractObjectInfo::eventFilter(QObject* obj, QEvent *event)
{
    AbstractElement* elem = findElement(obj->objectName());
    if(elem)
    {
       switch(elem->elementType())
       {
            //Трэба перавызначыць гэты метад для атожылкавых класаў
       }
       return true;
    }
    return QObject::eventFilter(obj,event);
}
