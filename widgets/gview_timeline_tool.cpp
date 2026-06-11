#include "gview_timeline_tool.h"



GViewTimeTool::GViewTimeTool(int tick_number,QObject *parent)
    : GViewTimeInterface(parent),_slider_(nullptr),
      _tick_number_(tick_number)
{
    return;
}

GViewTimeTool::GViewTimeTool(gview_time min_time,
                             gview_time max_time,
                             gview_time cur_time,
                             QObject* parent) :
    GViewTimeInterface(min_time,max_time,cur_time<min_time?min_time:cur_time,parent)
{
    return;
}

GViewTimeTool::~GViewTimeTool()
{
    qDeleteAll(_time_units_);
    return;
}

bool GViewTimeTool::checkWorkState() const noexcept
{
    return !_slider_;                                   // Дадаць праверкі пазней
}

void GViewTimeTool::setCurTimeScale()
{

}

QSlider *GViewTimeTool::getTimelineWidget()
{
    if(_tick_number_<2)
    {
        return nullptr;
    }
    if(_slider_)
    {
        _slider_->deleteLater();
        _slider_=nullptr;
    }
    _slider_ = new TimeSlider();
    _slider_->setOrientation(Qt::Horizontal);
    _slider_->setMinimum(0);
    _slider_->setMaximum(_tick_number_-1);
//    _slider_->setTickPosition(QSlider::TicksAbove);
    if(_labels_.isEmpty())
    {
    QStringList list;
    for(int i = 0; i<_tick_number_;i++)
    {
        list.append(QString::number(i));
    }

    _slider_->loadTextLabels(list);
    }
    else
    {
        QStringList list = _labels_;
        _slider_->loadTextLabels(list);
    }
    return _slider_;
}

void TimeSlider::loadTextLabels(QStringList& list)
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
        if(_slider_)
        {
            QStringList list;
            for(int i = 0; i<tick_number;i++)
            {
                list.append(QString::number(i));
            }
            _slider_->loadTextLabels(list);
            _slider_->setMaximum(tick_number);
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
    if(_slider_)
    {
        _slider_->setMaximum(_tick_number_-1);
        QStringList list(values);
        _slider_->loadTextLabels(list);
        emit widgetRequireUpdate();
    }
    return;
}

void GViewTimeTool::moveForward()
{
    if((!_slider_)||
            (_slider_->maximum()==_slider_->value()))
    {
        return;
    }
    _slider_->setValue(_slider_->value()+1);
    return;
}

void GViewTimeTool::moveBack()
{
    if((!_slider_)||
            (_slider_->minimum()==_slider_->value()))
    {
        return;
    }
    _slider_->setValue(_slider_->value()-1);
    return;
}

void GViewTimeTool::jump(int new_state)
{
    if((!_slider_)||
            (new_state>_slider_->maximum())||
            (new_state<_slider_->minimum()))
    {
        return;
    }
    _slider_->setValue(new_state);
    return;
}

QStringList GViewTimeTool::scales() const
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

QString GViewTimeTool::currentUnitInstance() const
{
    return _cur_unit_;
}

bool GViewTimeTool::setCurrentUnitInstance(const QString &unit_name)
{
    GViewBaseTObject* obj = getUnitInstance(unit_name);
    if(!obj)
    {
        return false;
    }
    _cur_unit_ = unit_name;
    int val = obj->getUnitVal(currentTime());
    int modifyer = obj->curModifier(currentTime());
    ///!calculate min and max vals of current time unit
    return true;
}

bool GViewTimeTool::addTimeObject(GViewBaseTObject* object)
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

bool GViewTimeTool::deleteTimeObject(const QString& time_object)
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
        ///! Do object deletion for selected object
    }
    delete *it;
    _time_units_.erase(it);
    return true;
}

void GViewTimeTool::stepForward()
{

}

void GViewTimeTool::stepBack()
{

}

void GViewTimeTool::jumpForward(int step)
{

}

void GViewTimeTool::jumpBack(int step)
{

}

void GViewTimeTool::moveTo(int value)
{

}

void GViewTimeTool::play()
{

}

void GViewTimeTool::stop()
{

}

void GViewTimeTool::pause()
{

}

void GViewTimeTool::setScale(const QString& sc_name)
{

}
