#include "gview_timeline_tool.h"

GViewTimeTool::GViewTimeTool(int tick_number,QObject *parent)
    : QObject{parent},_grid_(nullptr),_slider_(nullptr),
      _tick_number_(tick_number)
{

}

QGridLayout* GViewTimeTool::getTimelineWidget()
{
    if(_tick_number_<2)
    {
        return nullptr;
    }
    if(_grid_)
    {
        disconnect(_slider_,&QAbstractSlider::valueChanged,this,&GViewTimeTool::timeChanged);
        _grid_->deleteLater();
        _grid_ = nullptr;
        _slider_ = nullptr;
    }
    _grid_ = new QGridLayout();
    _slider_ = new QSlider(Qt::Horizontal);
    _slider_->setFocusPolicy(Qt::StrongFocus);
    _slider_->setTickPosition(QSlider::TicksBelow);
    _slider_->setStyleSheet("QSlider::sub-page:horizontal { background: blue; }"
                            "QSlider::add-page:horizontal { background: red; }");
    _slider_->setTickInterval(1);
    _slider_->setSingleStep(1);
    _slider_->setMinimum(0);
    _slider_->setMaximum(_tick_number_);
    connect(_slider_,&QAbstractSlider::valueChanged,this,&GViewTimeTool::timeChanged);
    _grid_->addWidget(_slider_,0,0,0,_tick_number_);
//    for(int i = 0; i<_tick_number_;i++)
//    {
//        QLabel* tick_text = new QLabel(QString::number(i));
//        _grid_->addWidget(tick_text,1,i);
//    }
    return _grid_;
}

void GViewTimeTool::setTickNumber(int tick_number)
{
    if(tick_number>0)
    {
        _tick_number_=tick_number;
        if(_slider_)
        {
            //_slider_->setMaximum(tick_number);
            emit widgetRequireUpdate();
        }
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

