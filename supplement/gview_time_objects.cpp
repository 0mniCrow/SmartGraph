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

gview_time_t GViewBaseTObject::accumulateUpperVal(const gview_time_t& time) const
{
//    if(!_greater_unit_)
//    {
//        return getUnitVal(time);
//    }
//    return _greater_unit_->accumulateUpperVal(time)+getUnitVal(time);
    //магчыма тут падыйдзе проста getInteger.
    return getInteger(time);
}

gview_time_t GViewBaseTObject::accumulateLowerVal(const gview_time_t& time) const
{
    if(!_lesser_unit_)
    {
        return getUnitTime(time);
    }
    return _lesser_unit_->accumulateLowerVal(time)+getUnitTime(time);
}

gview_time_t GViewBaseTObject::getUpperVal(const gview_time_t& time) const
{
    if(!_greater_unit_)
    {
        return TO_GVIEW_TIME(0);
    }
    return _greater_unit_->accumulateUpperVal(time);
}

gview_time_t GViewBaseTObject::getLowerVal(const gview_time_t& time) const
{
    if(!_lesser_unit_)
    {
        return TO_GVIEW_TIME(0);
    }
    return _lesser_unit_->accumulateLowerVal(time);
}

gview_time_t GViewBaseTObject::getUpperReminder(const gview_time_t& time) const
{
    if(!_greater_unit_)
    {
        return time;
    }
    return _greater_unit_->getReminder(time);
}

QString GViewBaseTObject::name() const noexcept
{
    return _name_;
}

gview_time_t GViewBaseTObject::modifier() const noexcept
{
    return _modifier_;
}

void GViewBaseTObject::setModifier(const gview_time_t& modifier) noexcept
{
    _modifier_ = modifier;
    return;
}

void GViewBaseTObject::setName(const QString& name) noexcept
{
    _name_ = name;
    return;
}

GViewBaseTObject* GViewBaseTObject::greaterUnit() const noexcept
{
    return _greater_unit_;
}

GViewBaseTObject* GViewBaseTObject::lesserUnit() const noexcept
{
    return _lesser_unit_;
}

void GViewBaseTObject::setGreaterUnit(GViewBaseTObject* greater_unit) noexcept
{
    _greater_unit_ = greater_unit;
    return;
}

void GViewBaseTObject::setLesserUnit(GViewBaseTObject* lesser_unit) noexcept
{
    _lesser_unit_ = lesser_unit;
    return;
}

bool GViewBaseTObject::isBasicUnit() const noexcept
{
    return !_lesser_unit_;
}

bool GViewBaseTObject::isTopUnit() const noexcept
{
    return !_greater_unit_;
}

FixedTObject::FixedTObject(const QString& name,
                      const gview_time_t& modifier,
                      GViewBaseTObject* greater_unit,
                      GViewBaseTObject* lesser_unit):
    GViewBaseTObject(name,modifier,greater_unit,lesser_unit)
{
    return;
}

gview_time_t FixedTObject::getInteger(const gview_time_t& time) const
{
      return time-getReminder(time);
}

gview_time_t FixedTObject::getReminder(const gview_time_t& time) const
{
    return  time % modifier();
}

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
    gview_time_t cur_unit_time = static_cast<gview_time_t>(val) * modifier();
    gview_time_t modified_time = getUpperVal(time) + cur_unit_time + getLowerVal(time);
    return modified_time;
}

int FixedTObject::scaleTimeToUnit(const gview_time_t &time) const
{
    return static_cast<int>(getUnitVal(time));
}

gview_time_t FixedTObject::curModifier(const gview_time_t& time) const
{
    Q_UNUSED(time)
    return modifier();
}

gview_time_t FixedTObject::getUnitVal(const gview_time_t& time) const
{
    gview_time_t upper_reminder = getUpperReminder(time);
    return upper_reminder/modifier();
}

gview_time_t FixedTObject::getUnitTime(const gview_time_t& time) const
{
    //gview_time_t upper_reminder = getUpperReminder(time);
    gview_time_t unit_num = getUnitVal(time);//upper_reminder/modifier();
    gview_time_t unit_time = unit_num * modifier();
    return unit_time;
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


bool VariantTObject::setTextLabels(const QStringList& text_labels)
{
    if(text_labels.size()!=static_cast<int>(modifier()))
    {
        return false;
    }
    _text_labels_ = text_labels;
    return true;
}

void VariantTObject::setLeapCycle(int leap_cycle) noexcept
{
    if(leap_cycle<1)
    {
        leap_cycle = 1;
    }
    _leap_cycle_=leap_cycle;
    return;
}

int VariantTObject::leapCycle() const noexcept
{
    return _leap_cycle_;
}

void VariantTObject::setLeapLength(int leap_length) noexcept
{
    if(leap_length<1)
    {
        leap_length = 1;
    }
    _leap_length_ = leap_length;
    return;
}

int VariantTObject::leapLength() const noexcept
{
    return _leap_length_;
}

void VariantTObject::setLeapUnit(FixedTObject* leap_unit) noexcept
{
    if(!leap_unit)
    {
        return;
    }
    _leap_unit_ = leap_unit;
    return;
}

FixedTObject* VariantTObject::leapUnit() const noexcept
{
    return _leap_unit_;
}

QStringList VariantTObject::getScaleLabels() const
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

gview_time_t VariantTObject::scaleUnitToTime(int val, const gview_time_t& time) const
{
    gview_time_t cur_unit_time = TO_GVIEW_TIME(0);
    int cycle_v = 0;
    for(int i = 0;i<val;i++)
    {
        ++cycle_v;
        if(cycle_v == _leap_cycle_)
        {
            cycle_v = 0;
            cur_unit_time +=  modifier() + _leap_unit_->modifier()*static_cast<gview_time_t>(_leap_length_);
        }
        else
        {
            cur_unit_time += modifier();
        }
    }
    gview_time_t modified_time = getUpperVal(time) + cur_unit_time + getLowerVal(time);
    return modified_time;
}

int VariantTObject::scaleTimeToUnit(const gview_time_t& time) const
{
    return static_cast<int>(getUnitVal(time));
}

gview_time_t VariantTObject::curModifier(const gview_time_t& time) const
{
    if(!_leap_unit_)
    {
        return modifier();
    }
    gview_time_t accumulator = TO_GVIEW_TIME(0);
    int cycle_t = 0;
    while(true)
    {
        ++cycle_t;
        gview_time_t cur_modifier = TO_GVIEW_TIME(0);
        if(cycle_t == _leap_cycle_)
        {
            cycle_t = 0;
            cur_modifier = modifier() + (_leap_unit_->modifier()*static_cast<gview_time_t>(_leap_length_));
        }
        else
        {
            cur_modifier =modifier();
        }
        if((accumulator+cur_modifier)>time)
        {
            return cur_modifier;
        }
        accumulator+=cur_modifier;
    }
}

gview_time_t VariantTObject::getInteger(const gview_time_t& time) const
{
    int num = 0;
    bool leap = false;
    return variantInteger(time,num,leap);//accumulator;
}

gview_time_t VariantTObject::variantInteger(const gview_time_t& time, int& number, bool &is_leap_time) const
{
    if(!_leap_unit_)
    {
        return modifier();
    }
    number = 0;
    gview_time_t accumulator = TO_GVIEW_TIME(0);
    int cycle_t = 0;
    while(true)
    {
        ++cycle_t;
        gview_time_t cur_modifier = TO_GVIEW_TIME(0);
        if(cycle_t == _leap_cycle_)
        {
            cycle_t = 0;
            cur_modifier = modifier() + (_leap_unit_->modifier()*static_cast<gview_time_t>(_leap_length_));
        }
        else
        {
            cur_modifier =modifier();
        }
        if((accumulator+cur_modifier)>=time)
        {
            if(!cycle_t)
            {
                is_leap_time=true;
            }
            else
            {
                is_leap_time=false;
            }
            break;
        }
        ++number;
        accumulator+=cur_modifier;
    }
    return accumulator;
}

gview_time_t VariantTObject::getReminder(const gview_time_t& time) const
{
    return time-getInteger(time);
}

gview_time_t VariantTObject::getVariantReminder(const gview_time_t& time, bool& is_leap_time) const
{
    int num = 0;
    return variantInteger(time,num,is_leap_time);
}

gview_time_t VariantTObject::getUnitVal(const gview_time_t& time) const
{
    gview_time_t upper_reminder = getUpperReminder(time);
    int unit_val = 0;
    bool leap = false;
    variantInteger(upper_reminder,unit_val,leap);
    return static_cast<gview_time_t>(unit_val);
}

gview_time_t VariantTObject::getUnitTime(const gview_time_t& time) const
{
    gview_time_t upper_reminder = getUpperReminder(time);
    int unit_val = 0;
    bool leap = false;
    return variantInteger(upper_reminder,unit_val,leap);
}


 MonthTObject::MonthTObject(const QString& name,
                        const gview_time_t& modifier,
                        FixedTObject* leap_unit,
                        const QList<QPair<QString,int>>& months,
                        const QString& leap_month,
                        int leap_cycle, int leap_length,
                        GViewBaseTObject* greater_unit,
                        GViewBaseTObject* lesser_unit
                        ):VariantTObject(name,modifier,leap_unit,
                                         leap_cycle,leap_length,
                                         greater_unit,lesser_unit),_leap_month_(leap_month)
 {
    gview_time_t full_mod = TO_GVIEW_TIME(0);
    for(const QPair<QString,int>& p:months)
    {
        gview_time_t cur_mod = static_cast<gview_time_t>(p.second)*leapUnit()->modifier();
        full_mod+=cur_mod;
        _months_.append(std::make_pair(p.first,cur_mod));
    }
    if(modifier!=full_mod)
    {
        setModifier(full_mod);
    }
    return;
 }

MonthTObject::~MonthTObject()
{

}

int MonthTObject::getMonthNum(const gview_time_t& time, bool is_leap_time) const
{
    gview_time_t base_mod = lesserUnit()->modifier();
    gview_time_t accumulator = TO_GVIEW_TIME(0);
    int i = 0;
    for(; i<_months_.size();++i)
    {
        gview_time_t month_size = _months_.at(i).second;
        gview_time_t month_accum = TO_GVIEW_TIME(0);
        if(is_leap_time && (_leap_month_==_months_.at(i).first))
        {
            month_size += leapUnit()->modifier()*static_cast<gview_time_t>(leapLength());
        }
        while(true)
        {
            if(accumulator+base_mod>time)
            {
                return i;
            }
            accumulator+=base_mod;
            month_accum+=base_mod;
            if(month_accum>month_size)
            {
                break;
            }
        }
    }
    return i;
}

QString  MonthTObject::getMonth(const gview_time_t& time) const
{
    bool leap = false;
    gview_time_t month_time = getVariantReminder(time,leap);
    return _months_.at(getMonthNum(month_time,leap)).first;

}
gview_time_t  MonthTObject::getMonthLength(const QString& month, const gview_time_t& time) const
{
    bool leap = false;
    getVariantReminder(time,leap);
    gview_time_t len;
    auto it_month = std::find_if(_months_.cbegin(),
                                    _months_.cend(),
                                    [&month](const QPair<QString,gview_time_t>& cur_month)
                {return cur_month.first==month;});
    len = it_month->second;
    if(leap&&(it_month->first==_leap_month_))
    {
        len+=leapUnit()->modifier()*static_cast<gview_time_t>(leapLength());
    }
    return len;
}

