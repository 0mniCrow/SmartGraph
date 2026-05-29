#include "gview_time_objects.h"

GViewBaseTObject::GViewBaseTObject(const QString& name,
                                   const gview_time_t& modifier,
                                   GViewBaseTObject* greater_unit,
                                   GViewBaseTObject* lesser_unit):
    _modifier_(modifier),_name_(name),
    _greater_unit_(greater_unit),
    _lesser_unit_(lesser_unit)
{
    return;
}

//gview_time_t GViewBaseTObject::getScalingFactor()
//{
//    if(!_modifier_ || !_lesser_unit_)
//    {
//        return TO_GVIEW_TIME(1);
//    }
//    return _modifier_ * _lesser_unit_->getScalingFactor();
//}

//gview_time_t GViewBaseTObject::getScalingShorting(const gview_time_t& time)
//{
//    if(!_greater_unit_)
//    {
//        return _modifier_;
//    }
//    return _greater_unit_->getScalingShorting(time)*_modifier_;
//}

gview_time_t GViewBaseTObject::accumulateUpperVal(const gview_time_t& time)
{
    if(!_greater_unit_)
    {
        return getUnitVal(time);
    }
    return _greater_unit_->getUnitVal(time)+getUnitVal(time);
}

gview_time_t GViewBaseTObject::accumulateLowerVal(const gview_time_t& time)
{
    if(!_lesser_unit_)
    {
        return getUnitVal(time);
    }
    return _lesser_unit_->getUnitVal(time)+getUnitVal(time);
}

gview_time_t GViewBaseTObject::getUpperReminder(const gview_time_t& time)
{
    if(!_greater_unit_)
    {
        return time;
    }
    return time%_greater_unit_->modifier();
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

void GViewBaseTObject::setName(const QString& name)
{
    _name_ = name;
    return;
}

GViewBaseTObject* GViewBaseTObject::greaterUnit() const
{
    return _greater_unit_;
}

GViewBaseTObject* GViewBaseTObject::lesserUnit() const
{
    return _lesser_unit_;
}

void GViewBaseTObject::setGreaterUnit(GViewBaseTObject* greater_unit)
{
    _greater_unit_ = greater_unit;
    return;
}

void GViewBaseTObject::setLesserUnit(GViewBaseTObject* lesser_unit)
{
    _lesser_unit_ = lesser_unit;
    return;
}

bool GViewBaseTObject::isBasicUnit() const
{
    return !_modifier_ && !_lesser_unit_;
}

FixedTObject::FixedTObject(const QString& name,
                      const gview_time_t& modifier,
                      GViewBaseTObject* greater_unit,
                      GViewBaseTObject* lesser_unit):
    GViewBaseTObject(name,modifier,greater_unit,lesser_unit)
{
    return;
}

//bool FixedTObject::addContainingUnit(GViewBaseTObject* containing_unit)
//{
//    if(!containing_unit)
//    {
//        return false;
//    }
//    if(_contained_in_.contains(containing_unit))
//    {
//        return false;
//    }
//    QSet<GViewBaseTObject*>::ConstIterator it = _contained_in_.cbegin();
//    while(it!=_contained_in_.cend())
//    {
//        if((*it)->name()==containing_unit->name())
//        {
//            return false;
//        }
//        ++it;
//    }
//    _contained_in_.insert(containing_unit);
//    return true;
//}

bool FixedTObject::setTextLabels(const QStringList& text_labels)
{
    if(text_labels.size()!=static_cast<int>(modifier()))
    {
        return false;
    }
    _text_labels_ = text_labels;
    return true;
}

QStringList FixedTObject::getScaleLabels() const
{
    if(_text_labels_.isEmpty())
    {
        QStringList text_labels;
        for(int i = 0; i<static_cast<int>(modifier());++i)
        {
            text_labels.append(QString::number(i+1));
        }
        return text_labels;
    }
    return _text_labels_;
}

gview_time_t FixedTObject::scaleUnitToTime(int val, const gview_time_t &time) const
{

}
int FixedTObject::scaleTimeToVal(const gview_time_t &time) const
{

}

VariantTObject::VariantTObject(const QString& name,
                        const gview_time_t& modifier,
                        FixedTObject* leap_unit,
                        int leap_cycle, int leap_length,
                        GViewBaseTObject* greater_unit,
                        GViewBaseTObject* lesser_unit
                        ):
    GViewBaseTObject(name,modifier,greater_unit,lesser_unit),
    _leap_cycle_(leap_cycle),_leap_length_(leap_length),
    _leap_unit_(leap_unit)

{
    return;
}
