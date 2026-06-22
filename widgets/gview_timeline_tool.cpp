#include "gview_timeline_tool.h"



GViewTimeTool::GViewTimeTool(int tick_number,QObject *parent)
    : GViewTimeInterface(parent),_time_slider_(nullptr),
      _tick_number_(tick_number)
{
    FixedTObject* secs = new FixedTObject("Секунды");
    FixedTObject* min = new FixedTObject("Хвіліны",60,nullptr,secs);
    secs->setGreaterUnit(min);
    FixedTObject* hour = new FixedTObject("Гадзіны",60*min->modifier(),nullptr,min);
    min->setGreaterUnit(hour);
    FixedTObject* day = new FixedTObject("Дні",24*hour->modifier(),nullptr,hour);
    hour->setGreaterUnit(day);
    addTimeUnit(secs);
    addTimeUnit(min);
    addTimeUnit(hour);
    addTimeUnit(day);
    return;
}

GViewTimeTool::GViewTimeTool(gview_time min_time,
                             gview_time max_time,
                             gview_time cur_time,
                             int timer_interval,
                             QObject* parent) :
    GViewTimeInterface(min_time,max_time,cur_time<min_time?min_time:cur_time,timer_interval,parent)
{
    return;
}

GViewTimeTool::~GViewTimeTool()
{
    qDeleteAll(_time_units_);
    return;
}

bool GViewTimeTool::isReadyForWork() const noexcept
{
    return _time_slider_ &&
            !_time_units_.isEmpty()&&
            !_cur_unit_.isEmpty();             // Дадаць праверкі
}

void GViewTimeTool::setBordersForSlider()
{
    if(!isReadyForWork())
    {
        return;
    }
    GViewBaseTObject* cur_obj = getUnitInstance(_cur_unit_);
    if(!cur_obj)
    {
        return;
    }
    int num_of_units = cur_obj->isTopUnit()?
                cur_obj->getUnitCount(currentTime()):
                cur_obj->getUpperUnit()->getLowerUnitCount(currentTime());
    _time_slider_->setMinimum(1);
    _time_slider_->setMaximum(num_of_units);
    return;
}

void GViewTimeTool::setNewTime(int new_val)
{
    if(_cur_unit_.isEmpty())
    {
        return;
    }
    GViewBaseTObject* cur_obj = getUnitInstance(_cur_unit_);
    if(!cur_obj)
    {
        return;
    }
    if(cur_obj->isBasicUnit())
    {
        setCurrentTime(cur_obj->scaleUnitToTime(new_val,currentTime()));
    }
    else
    {
        setCurrentTime(cur_obj->getLowerUnit()->scaleUnitToTime(new_val,currentTime()));
    }
    return;
}

void GViewTimeTool::updateSliderValue()
{
    if(!isReadyForWork())
    {
        return;
    }
    GViewBaseTObject* cur_obj = getUnitInstance(_cur_unit_);
    if(!cur_obj)
    {
        return;
    }
    int cur_val = cur_obj->scaleTimeToUnit(currentTime());
    if(cur_val>=_time_slider_->minimum() &&
            cur_val<=_time_slider_->maximum())
    {
        _time_slider_->setValue(cur_val);
    }
    return;
}

int GViewTimeTool::getCurrentTime()
{
    if(_cur_unit_.isEmpty())
    {
        return -1;
    }
    GViewBaseTObject* cur_obj = getUnitInstance(_cur_unit_);
    if(!cur_obj)
    {
        return -1;
    }
    if(cur_obj->isBasicUnit())
    {
        return cur_obj->scaleTimeToUnit(currentTime());
    }
    return cur_obj->getLowerUnit()->scaleTimeToUnit(currentTime());

}

bool GViewTimeTool::generateTimeSlider()
{
    if(_time_units_.isEmpty()||
         _cur_unit_.isEmpty()||
            _time_slider_)
    {
        return false;
    }
    auto unit = getUnitInstance(_cur_unit_);
    if(!unit)
    {
        return false;
    }
    _time_slider_ = new TimeSlider();
    setBordersForSlider();
    _time_slider_->loadTextLabels(unit->getScaleLabels());
    return true;
}

QSlider *GViewTimeTool::getTimelineWidget()
{
    /*
    if(_tick_number_<2)
    {
        return nullptr;
    }
    if(_time_slider_)
    {
        _time_slider_->deleteLater();
        _time_slider_=nullptr;
    }
    _time_slider_ = new TimeSlider();
    _time_slider_->setOrientation(Qt::Horizontal);
    _time_slider_->setMinimum(0);
    _time_slider_->setMaximum(_tick_number_-1);
//    _time_slider_->setTickPosition(QSlider::TicksAbove);
    if(_labels_.isEmpty())
    {
    QStringList list;
    for(int i = 0; i<_tick_number_;i++)
    {
        list.append(QString::number(i));
    }

    _time_slider_->loadTextLabels(list);
    }
    else
    {
        QStringList list = _labels_;
        _time_slider_->loadTextLabels(list);
    }
    */
    if(!_time_slider_)
    {
        if(!generateTimeSlider())
        {
            return nullptr;
        }
    }
    return _time_slider_;
}

void TimeSlider::loadTextLabels(const QStringList& list)
{
    if(!_text_.isEmpty())
    {
        _text_.clear();
    }
    _text_ = list;
    repaint();
}

void GViewTimeTool::setTickNumber(int tick_number)
{
    if(tick_number>0)
    {
        _tick_number_=tick_number;
        if(_time_slider_)
        {
            QStringList list;
            for(int i = 0; i<tick_number;i++)
            {
                list.append(QString::number(i));
            }
            _time_slider_->loadTextLabels(list);
            _time_slider_->setMaximum(tick_number);
            emit widgetRequireUpdate();
        }
    }
    return;
}

void GViewTimeTool::loadValues(const QStringList& values)
{
    if(!values.size())
    {
        return;
    }
    _tick_number_=values.size();
    _labels_=values;
    if(_time_slider_)
    {
        _time_slider_->setMaximum(_tick_number_-1);
        QStringList list(values);
        _time_slider_->loadTextLabels(list);
        emit widgetRequireUpdate();
    }
    return;
}

void GViewTimeTool::moveForward()
{
    if((!_time_slider_)||
            (_time_slider_->maximum()==_time_slider_->value()))
    {
        return;
    }
    _time_slider_->setValue(_time_slider_->value()+1);
    return;
}

void GViewTimeTool::moveBack()
{
    if((!_time_slider_)||
            (_time_slider_->minimum()==_time_slider_->value()))
    {
        return;
    }
    _time_slider_->setValue(_time_slider_->value()-1);
    return;
}

void GViewTimeTool::jump(int new_state)
{
    if((!_time_slider_)||
            (new_state>_time_slider_->maximum())||
            (new_state<_time_slider_->minimum()))
    {
        return;
    }
    _time_slider_->setValue(new_state);
    return;
}

QStringList GViewTimeTool::timeUnitNames() const
{
    QStringList unit_names;
    for(const GViewBaseTObject* obj:_time_units_)
    {
        unit_names.append(obj->name());
    }
    return unit_names;
}

GViewBaseTObject* GViewTimeTool::getUnitInstance(const QString& obj_name) const
{
    if(_time_units_.empty()||obj_name.isEmpty())
    {
        return nullptr;
    }
    auto it = std::find_if(_time_units_.cbegin(),
                           _time_units_.cend(),
                           [&obj_name](const GViewBaseTObject* obj)
    {
        return obj->name()==obj_name;
    });
    if(it==_time_units_.cend())
    {
        return nullptr;
    }
    return *it;
}

QString GViewTimeTool::currentUnit() const
{
    return _cur_unit_;
}

bool GViewTimeTool::setCurrentUnit(const QString &unit_name)
{
    GViewBaseTObject* obj = getUnitInstance(unit_name);
    if(!obj)
    {
        return false;
    }
    _cur_unit_ = unit_name;
    setBordersForSlider();
    return true;
}

bool GViewTimeTool::getTime(QMap<QString,int>& container) const
{
    container.clear();
    if(!isReadyForWork())
    {
        return false;
    }
    for(const GViewBaseTObject* obj:_time_units_)
    {
        container.insert(obj->name(),obj->scaleTimeToUnit(currentTime()));
    }
    return true;
}

bool GViewTimeTool::addTimeUnit(GViewBaseTObject* object)
{
    if(!object)
    {
        return false;
    }
    auto it = std::find_if(_time_units_.cbegin(),
                           _time_units_.cend(),
                           [object](const GViewBaseTObject* obj)
    {return object->name()==obj->name();});
    if(it!=_time_units_.cend())
    {
        return false;
    }
    _time_units_.append(object);
    return true;
}

bool GViewTimeTool::deleteTimeUnit(const QString& time_object)
{
    if(time_object.isEmpty())
    {
        return false;
    }
    auto it = std::find_if(_time_units_.cbegin(),
                           _time_units_.cend(),
                           [time_object](const GViewBaseTObject* obj)
    {return time_object==obj->name();});
    if(it==_time_units_.constEnd())
    {
        return false;
    }
    for(GViewBaseTObject* obj:_time_units_)
    {
        obj->removeLinksToUnit(*it);
    }
    delete *it;
    _time_units_.erase(it);
    return true;
}

void GViewTimeTool::stepForward()
{
    if(!isReadyForWork())
    {
        return;
    }
    if(_time_slider_->value()==_time_slider_->maximum())
    {
        return;
    }
    _time_slider_->setValue(_time_slider_->value()+1);
    setNewTime(_time_slider_->value());
    return;
}

void GViewTimeTool::stepBack()
{
    if(!isReadyForWork())
    {
        return;
    }
    if(_time_slider_->value()==_time_slider_->minimum())
    {
        return;
    }
    _time_slider_->setValue(_time_slider_->value()-1);
    setNewTime(_time_slider_->value());
    return;
}

void GViewTimeTool::jumpForward(int step)
{
    if(!isReadyForWork())
    {
        return;
    }
    if(_time_slider_->value()+step>_time_slider_->maximum())
    {
        _time_slider_->setValue(_time_slider_->maximum());
    }
    else
    {
        _time_slider_->setValue(_time_slider_->value()+step);
    }
    setNewTime(_time_slider_->value());
    return;
}

void GViewTimeTool::jumpBack(int step)
{
    if(!isReadyForWork())
    {
        return;
    }
    if(_time_slider_->value()-step<_time_slider_->minimum())
    {
        _time_slider_->setValue(_time_slider_->minimum());
    }
    else
    {
        _time_slider_->setValue(_time_slider_->value()-step);
    }
    setNewTime(_time_slider_->value());
    return;
}

void GViewTimeTool::moveTo(int value)
{
    if(!isReadyForWork())
    {
        return;
    }
    if(value<_time_slider_->minimum())
    {
        value = _time_slider_->minimum();
    }
    else if(value>_time_slider_->maximum())
    {
        value = _time_slider_->maximum();
    }
    _time_slider_->setValue(value);
    setNewTime(value);
    return;
}

void GViewTimeTool::play()
{
    if(!isReadyForWork())
    {
        return;
    }
    if(_play_timer_.isActive())
    {
        _play_timer_.stop();
    }
    _play_timer_.setInterval(timerInterval());
    _play_timer_.start();
    return;
}

void GViewTimeTool::stop()
{
    if(_play_timer_.isActive())
    {
        _play_timer_.stop();
    }
    return;
}

void GViewTimeTool::pause()
{
    stop();
    return;
}

void GViewTimeTool::setScale(const QString& sc_name)
{
    setCurrentUnit(sc_name);
    return;
}

void GViewTimeTool::setTime(const QMap<QString,int>& unitname_val)
{
    if(!isReadyForWork())
    {
        return;
    }
    gview_time new_time = TO_GVIEW_TIME(0);
    for(const GViewBaseTObject* obj:_time_units_)
    {
        auto it = unitname_val.find(obj->name());
        if(it!=unitname_val.cend())
        {
            new_time+=obj->scaleUnitToTime(it.value(),TO_GVIEW_TIME(0));
        }
    }
    setTime(new_time);
    return;
}

void GViewTimeTool::setTime(const gview_time& time)
{
    if(!isReadyForWork())
    {
        return;
    }
    gview_time t_time = time;
    if(time<minTime())
    {
        t_time=minTime();
    }
    else if(time>maxTime())
    {
        t_time = maxTime();
    }
    setCurrentTime(t_time);
    updateSliderValue();
}

void GViewTimeTool::nextStep()
{
    if(!_time_slider_|| (_time_slider_->value()>=_time_slider_->maximum()))
    {
        _play_timer_.stop();
    }
    stepForward();
    return;
}
