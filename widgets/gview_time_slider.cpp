#include "gview_time_slider.h"
void TimeSlider::paintEvent(QPaintEvent* p_event)
{
    if((orientation()!=Qt::Horizontal)||!_text_.size())
    {
        return QSlider::paintEvent(p_event);
    }
    p_event->accept();
    QStyleOptionSlider s_option;
    initStyleOption(&s_option);
    QRect s_rect = rect();
    s_rect.setHeight(s_rect.height()-20);
    s_option.rect=s_rect;
    s_option.subControls = QStyle::SC_All;
    QPainter * painter = new QPainter(this);
    painter->setRenderHint(QPainter::Antialiasing,true);
    QRect groove_rect = style()->subControlRect(QStyle::CC_Slider,&s_option,QStyle::SC_SliderGroove,this);
    QRect handle_rect = style()->subControlRect(QStyle::CC_Slider,&s_option,QStyle::SC_SliderHandle,this);
    recalculateLabelPos();
    drawLabels(painter,groove_rect);
    drawSlider(painter,s_option,groove_rect,handle_rect);

    //alternativeCalculation(painter,groove_rect,handle_rect,s_option);
    painter->end();
    delete painter;
    return;
}

void TimeSlider::recalculateLabelPos()
{
    if(!_pos_recalc_needed_)
    {
        return;
    }
    QStyleOptionSlider s_option;
    initStyleOption(&s_option);
    QRect groove_rect = style()->subControlRect(QStyle::CC_Slider,&s_option,QStyle::SC_SliderGroove,this);
    QRect handle_rect = style()->subControlRect(QStyle::CC_Slider,&s_option,QStyle::SC_SliderHandle,this);

    int slider_len = handle_rect.width();
    int slider_min = groove_rect.x();
    int slider_max = groove_rect.right()- slider_len+1;
    for(int i = 0; i< _text_.size();i++)
    {
        int text_pos = QStyle::sliderPositionFromValue(minimum(),maximum(),i,
                                                       slider_max - slider_min,
                                                       s_option.upsideDown);
        if(!i)
        {
            _positions_[i] = text_pos;
        }
        else if(i==_text_.size()-1)
        {
            _positions_[i] = text_pos+slider_min+slider_len;
        }
        else
        {
            _positions_[i] = text_pos+slider_min+slider_len/2;
        }
    }
    QFontMetrics fm = fontMetrics();
    _adj_labels_ = adjustLabels(this->rect().width()/(_text_.size()-1),fm);
    _pos_recalc_needed_=false;
    return;
}

void TimeSlider::drawLabels(QPainter* painter, const QRect &groove_rect)
{
    painter->save();
    painter->setPen(QPen(QBrush(Qt::black),1));
    qreal label_y_pos = static_cast<qreal>(this->rect().height())/2;
    QFontMetrics fm = painter->fontMetrics();
    qreal range_step = static_cast<qreal>(groove_rect.width())/static_cast<qreal>(_text_.size()-1);
    QPointF r_st;
    QPointF r_fn;
    for(int i = 0; i< _text_.size();i++)
    {
        qreal t_pos = static_cast<qreal>(_positions_[i]);
        r_st.setX(t_pos);
        r_fn.setX(t_pos);
        if((!i)||(i==_text_.size()-1))
        {
            r_st.setY(groove_rect.top()-20);
            r_fn.setY(groove_rect.bottom()+10);
        }
        else
        {
            r_st.setY(groove_rect.top()-10);
            r_fn.setY(groove_rect.bottom()+5);
        }

        if(i==_adj_labels_.size()-1)
        {
            t_pos -=static_cast<qreal>(fm.horizontalAdvance(_adj_labels_.at(i)));
        }
        else if(t_pos)
        {
            t_pos -=static_cast<qreal>(fm.horizontalAdvance(_adj_labels_.at(i)))/2;
        }
        QRectF text_rect(t_pos,label_y_pos,range_step,20.0);
        painter->drawText(text_rect,_adj_labels_.at(i));
        painter->drawLine(r_st,r_fn);
    }
    painter->restore();
    return;
}

void TimeSlider::drawSlider(QPainter* painter, QStyleOptionSlider &s_option,
                            const QRect &groove_rect, const QRect &handle_rect)
{
    painter->save();
    style()->drawComplexControl(QStyle::CC_Slider,&s_option,painter,this);
    int left_gr_side = handle_rect.center().x()-(handle_rect.width()/2);
    int groove_h = groove_rect.height()/4;
    QRect left_gr_rect = QRect(groove_rect.left(),groove_rect.top()+(groove_h*1.5),
                               left_gr_side-groove_rect.left(),groove_rect.height()-(groove_h*2.5));
    painter->setBrush(QColor(QColorConstants::Svg::orange));
    painter->setPen(Qt::NoPen);
    painter->drawRect(left_gr_rect);
    painter->restore();
    return;
}

void TimeSlider::alternativeCalculation(QPainter * painter,const QRect& groove_rect, const QRect &handle_rect, QStyleOptionSlider& s_option)
{
    painter->save();
    qreal label_y_pos = static_cast<qreal>(this->rect().height())/2;
    QFontMetrics fm = painter->fontMetrics();
    qreal range_step = static_cast<qreal>(groove_rect.width())/static_cast<qreal>(_text_.size()-1);
    QPointF r_st;
    QPointF r_fn;
    for(int i = 0; i< _text_.size();i++)
    {
        qreal t_pos = static_cast<qreal>(_positions_[i]);
        r_st.setX(t_pos);
        r_fn.setX(t_pos);
        if((!i)||(i==_text_.size()-1))
        {
            r_st.setY(groove_rect.top()-20);
            r_fn.setY(groove_rect.bottom()+10);
        }
        else
        {
            r_st.setY(groove_rect.top()-10);
            r_fn.setY(groove_rect.bottom()+5);
        }

        if(i==_adj_labels_.size()-1)
        {
            t_pos -=static_cast<qreal>(fm.horizontalAdvance(_adj_labels_.at(i)));
        }
        else if(t_pos)
        {
            t_pos -=static_cast<qreal>(fm.horizontalAdvance(_adj_labels_.at(i)))/2;
        }
        QRectF text_rect(t_pos,label_y_pos,range_step,20.0);
        painter->drawText(text_rect,_adj_labels_.at(i));
        painter->drawLine(r_st,r_fn);
    }
    painter->restore();
    painter->save();
    style()->drawComplexControl(QStyle::CC_Slider,&s_option,painter,this);
    int left_gr_side = handle_rect.center().x()-(handle_rect.width()/2);
    int groove_h = groove_rect.height()/4;
    QRect left_gr_rect = QRect(groove_rect.left(),groove_rect.top()+(groove_h*1.5),
                               left_gr_side-groove_rect.left(),groove_rect.height()-(groove_h*2.5));
    painter->setBrush(QColor(QColorConstants::Svg::orange));
    painter->setPen(Qt::NoPen);
    painter->drawRect(left_gr_rect);
    painter->restore();
    return;
}


void TimeSlider::collectPositions(QList<QPair<int,int>>& container, const QStringList &list,
                                  int step_x, const QFontMetrics &f_metrix) const
{
    container.clear();
    for(int i = 0; i<list.size();i++)
    {
        int s_pos = step_x*i;
        if(i==list.size()-1)
        {
            s_pos -=f_metrix.horizontalAdvance(list.at(i));
        }
        else if(s_pos)
        {
            s_pos -=f_metrix.horizontalAdvance(list.at(i))/2;
        }
        int e_pos = s_pos+f_metrix.horizontalAdvance(list.at(i));
        container.append(std::make_pair(s_pos,e_pos));
    }
    return;
}

int TimeSlider::areLabelsAdjusted(const QList<QPair<int,int>>& text_metrix) const
{
    int num = -1;
    for(int i = 0; i<text_metrix.size();i++)
    {
        if(i==text_metrix.size()-1)
        {
            continue;
        }
        if(text_metrix.at(i).second>=text_metrix.at(i+1).first)
        {
            num = i;
            break;
        }
    }
    return num;
}

QStringList TimeSlider::adjustLabels(int step_x, const QFontMetrics &f_metrix) const
{
    QList<QPair<int,int>> text_metrix;
    QStringList final_list(_text_);
    final_list.detach();
    while(true)
    {
        collectPositions(text_metrix,final_list,step_x,f_metrix);
        int pos = areLabelsAdjusted(text_metrix);
        if((pos<0)||(final_list.at(pos).size()<=3))
        {
            break;
        }
        int adj_size = final_list.at(pos).size()-2;
        for(QString& label:final_list)
        {
            label.truncate(adj_size);
            label.append('.');
        }
    }
    return final_list;
}

QSize TimeSlider::sizeHint() const
{
    QSize size = QSlider::sizeHint();
    size.setHeight(size.height()+20);
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
    int abs_val = INT_MAX;
    int closest_val = -1;
    for(int i = 0; i<_positions_.size();i++)
    {
        if(std::abs(_positions_.at(i)-m_pos)<abs_val)
        {
            closest_val = i;
            abs_val = std::abs(_positions_.at(i)-m_pos);
        }
    }
    if(closest_val>=0)
    {
        setValue(closest_val);
        emit sliderPressed();
    }
    return;
}

void TimeSlider::wheelEvent(QWheelEvent* w_event)
{
    if(_text_.size()<1)
    {
        return QSlider::wheelEvent(w_event);
    }
    w_event->accept();
    int step = w_event->angleDelta().y()/120;
    if(step<0)
    {
        int new_val = value() - std::abs(step)*WHEEL_STEP;
        if(new_val<minimum())
        {
            setValue(minimum());
        }
        else
        {
            setValue(new_val);
        }
    }
    else if (step>0)
    {
        int new_val = value()+step*WHEEL_STEP;
        if(new_val>maximum())
        {
            setValue(maximum());
        }
        else
        {
            setValue(new_val);
        }
    }
    return;
}

void TimeSlider::loadTextLabels(const QStringList& list)
{
    if(!_text_.isEmpty())
    {
        _text_.clear();
    }
    _text_ = list;
    _positions_.resize(_text_.size());
    _positions_.fill(0);
    _pos_recalc_needed_ = true;
    recalculateLabelPos();
    repaint();
}

void TimeSlider::changeEvent(QEvent* c_event)
{
    QSlider::changeEvent(c_event);
    switch(c_event->type())
    {
    case QEvent::StyleChange:
        [[fallthrough]];
    case QEvent::FontChange:
        [[fallthrough]];
    case QEvent::LayoutDirectionChange:
    {
        invalidateLabelPos();
    }
        break;
    default:
    {

    }
    }
    return;
}

void TimeSlider::resizeEvent(QResizeEvent* r_event)
{
    QSlider::resizeEvent(r_event);
    invalidateLabelPos();
    return;
}
