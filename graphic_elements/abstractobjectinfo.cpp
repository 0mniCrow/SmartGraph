#include "abstractobjectinfo.h"

AbstractObjectInfo::AbstractObjectInfo()
{
    return;
}

AbstractObjectInfo::AbstractObjectInfo(std::initializer_list<AbstractElement*> list):_elements_(list)
{
    return;
}

//AbstractObjectInfo::~AbstractObjectInfo()
//{
//    clear();
//    return;
//}

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

int AbstractObjectInfo::size() const
{
    return _elements_.size();
}

//void AbstractObjectInfo::setSize(int size)
//{
//    if(size<=0)
//    {
//        return;
//    }
//    if(_elements_.size()<size)
//    {
//        _elements_.insert(_elements_.cend(),size-_elements_.size(),nullptr);
//    }
//    else if(_elements_.size()>size)
//    {
//        while(_elements_.size()>size)
//        {
//            delete _elements_.takeLast();
//            //AbstractElement* last_el = _elements_.last();
//            //delete last_el;
//        }
//    }
//    return;
//}

bool AbstractObjectInfo::isExist(const QString& element_name) const
{
    return findElement(element_name);
}

QVariant AbstractObjectInfo::at(int num) const
{
    if(num<0 || num>=_elements_.size())
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
    if(num<0 || num>=_elements_.size())
    {
        return QString();
    }
    return _elements_.at(num)->elementName();
}

AbstractElement* AbstractObjectInfo::elementAt(int num) const
{
    if(num<0 || num>=_elements_.size())
    {
        return nullptr;
    }
    return _elements_.at(num);
}

AbstractElement* AbstractObjectInfo::elementAt(const QString& element_name) const
{
    return findElement(element_name);
}

bool AbstractObjectInfo::append(const QString& element_name, const QVariant& value, char element_type)
{
    if(findElement(element_name))
    {
        return false;
    }
    AbstractElement * new_element(createElement(element_name,value,element_type));
    _elements_.append(new_element);
    return true;
}

bool AbstractObjectInfo::destroyAt(int num)
{
    if(num<0 || num>=_elements_.size())
    {
        return false;
    }
    AbstractElement* elem = _elements_.at(num);
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
    if(num<0 || num>=_elements_.size())
    {
        return false;
    }
    _elements_.at(num)->setValue(new_val);
    return true;
}

QVariant& AbstractObjectInfo::operator[](int num)
{
    return _elements_.at(num)->rvalue();
    //Небяспечны метад - выкарыстоўваць з увагай
}

QVariant& AbstractObjectInfo::operator()(const QString& element_name)
{
    return findElement(element_name)->rvalue();
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
    if(num<0 || num>=_elements_.size())
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
