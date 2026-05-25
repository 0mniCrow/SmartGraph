#include "gview_timeline_tool.h"



GViewTimeTool::GViewTimeTool(int tick_number,QObject *parent)
    : QObject{parent},_slider_(nullptr),//_grid_(nullptr),
      _tick_number_(tick_number)
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

