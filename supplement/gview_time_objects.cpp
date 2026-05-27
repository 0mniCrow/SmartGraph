#include "gview_time_objects.h"

GViewBaseTObject::GViewBaseTObject(const QString& name,
                                   const gview_time_t& modifier):
    _modifier_(modifier),_name_(name)
{
    return;
}

GViewBaseTObject::GViewBaseTObject(const GViewBaseTObject& obj)
{
    _name_ = obj._name_;
    _modifier_ = obj._modifier_;
    return;
}

GViewBaseTObject::GViewBaseTObject(GViewBaseTObject&& obj)
{
    std::swap(_name_,obj._name_);
    std::swap(_modifier_,obj._modifier_);
    return;
}

gview_time_t GViewBaseTObject::extractCurrentValue()
{
    if(!_modifier_)
    {
        return TO_GVIEW_TIME(1);
    }
    if(!subTObjCount())
    {
        return _modifier_;
    }
    return _modifier_ * getSupTObj()->extractCurrentValue();
}

QString GViewBaseTObject::name() const
{
    return _name_;
}

gview_time_t GViewBaseTObject::modifier() const
{
    return _modifier_;
}

void GViewBaseTObject::setModifier(const gview_time_t& modifier)
{
    _modifier_ = modifier;
    return;
}

GViewBaseTObject& GViewBaseTObject::operator=(const GViewBaseTObject& obj)
{
    _name_ = obj._name_;
    _modifier_ = obj._modifier_;
    return *this;
}

GViewBaseTObject& GViewBaseTObject::operator=(GViewBaseTObject&& obj)
{
    std::swap(_name_,obj._name_);
    std::swap(_modifier_,obj._modifier_);
    return *this;
}
