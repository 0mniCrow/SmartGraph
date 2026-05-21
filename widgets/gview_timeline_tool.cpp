#include "gview_timeline_tool.h"

void TimeSlider::paintEvent(QPaintEvent* p_event)
{
    p_event->accept();
    if((orientation()!=Qt::Horizontal)||!_text_.size())
    {
        return;
    }
    //QStylePainter * s_painter = new QStylePainter(this);
    QStyleOptionSlider s_option;
    initStyleOption(&s_option);
    QRect s_rect = rect();
    s_rect.setHeight(s_rect.height()-20);
    //QStyleOptionSlider rect_option(s_option);
    s_option.rect=s_rect;
    s_option.subControls = QStyle::SC_All;
    //s_painter->end();

    QPainter * painter = new QPainter(this);
    painter->setRenderHint(QPainter::Antialiasing,true);
    painter->save();
    painter->setPen(QPen(QBrush(Qt::black),1));
    QRect groove_rect = style()->subControlRect(QStyle::CC_Slider,&s_option,QStyle::SC_SliderGroove,this);
    int range_step = groove_rect.width()/(_text_.size()-1);
    for(int i = 0; i<_text_.size();i++)
    {
        if((!i)||(i==_text_.size()-1))
        {
            painter->drawLine(i*range_step,groove_rect.top()-20,
                              i*range_step,groove_rect.bottom()+10);
        }
        else
        {
            painter->drawLine(i*range_step,groove_rect.top()-10,
                              i*range_step,groove_rect.bottom()+5);
        }
    }
    painter->restore();
    painter->save();
    QRect handle_rect = style()->subControlRect(QStyle::CC_Slider,&s_option,QStyle::SC_SliderHandle,this);
    style()->drawComplexControl(QStyle::CC_Slider,&s_option,painter,this);
    int left_gr_side = handle_rect.center().x()-(handle_rect.width()/2);
    int groove_h = groove_rect.height()/4;
    QRect left_gr_rect = QRect(groove_rect.left(),groove_rect.top()+(groove_h*1.5),
                               left_gr_side-groove_rect.left(),groove_rect.height()-(groove_h*2.5));
    painter->setBrush(QColor(QColorConstants::Svg::orange));
    painter->setPen(Qt::NoPen);
    painter->drawRect(left_gr_rect);
    painter->restore();
    painter->setPen(QPen(Qt::black));
    painter->setBrush(QBrush(Qt::yellow));
    QRect rec = this->rect();
    int mid = rec.height()/2;
    int st = rec.width()/(_text_.size()-1);
    QFontMetrics fm = painter->fontMetrics();
    for(int i = 0; i<_text_.size();i++)
    {
        int t_pos = st*i;
        if(i==_text_.size()-1)
        {
            t_pos -=fm.horizontalAdvance(_text_.at(i));
        }
        else if(t_pos)
        {
            t_pos -=fm.horizontalAdvance(_text_.at(i))/2;
        }
        QRect rt(t_pos,mid,st,20);
        painter->drawText(rt,_text_.at(i));
    }
    painter->end();
    delete painter;
    return;
}

QSize TimeSlider::sizeHint() const
{
    QSize size = QSlider::sizeHint();
    size.setHeight(size.height()+20);
    //size.setWidth(size.width()+20);
    return size;
}

void TimeSlider::mousePressEvent(QMouseEvent* m_event)
{
    if(_text_.size()<1)
    {
        return QSlider::mousePressEvent(m_event);
    }
    QStyleOptionSlider sl_options;
    initStyleOption(&sl_options);
    QRect handle_rect = style()->subControlRect(QStyle::CC_Slider, &sl_options,
                                                   QStyle::SC_SliderHandle, this);
    if(handle_rect.contains(m_event->pos()))
    {
        return QSlider::mousePressEvent(m_event);
        setSliderDown(true);
        return;
    }
    int m_pos = m_event->pos().x();
    QRect loc_rect = this->rect();
    int rect_step = loc_rect.width()/(_text_.size()-1);
    int abs_val = INT_MAX;
    int closest_val = -1;
    for(int i = 0; i<_text_.size();i++)
    {
        if(std::abs(rect_step*i-m_pos)<abs_val)
        {
            closest_val = i;
            abs_val = std::abs(rect_step*i-m_pos);
        }
    }
    if(closest_val>=0)
    {
        setValue(closest_val);
        emit sliderPressed();
    }
    return;
}

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

