#include "gview_timeline_tool.h"

void TimeSlider::paintEvent(QPaintEvent* p_event)
{
    p_event->accept();
    //QSlider::paintEvent(p_event);
    if((orientation()!=Qt::Horizontal)||!_text_.size())
    {
        return;
    }
    //QStylePainter * s_painter = new QStylePainter(this);
    QStyleOptionSlider s_option;
    initStyleOption(&s_option);
    QRect s_rect = rect();//.adjusted(0,0,0,-20);
    s_rect.setHeight(s_rect.height()-20);
    //s_rect.setWidth(s_rect.width()-20);
    QStyleOptionSlider rect_option(s_option);
    rect_option.rect=s_rect;
    rect_option.subControls = QStyle::SC_All;
    //s_painter->end();

    QPainter * painter = new QPainter(this);
    painter->setRenderHint(QPainter::Antialiasing,true);
    painter->save();
    style()->drawComplexControl(QStyle::CC_Slider,&rect_option,painter,this);
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
        //painter->drawRect(rt);
        painter->drawText(rt,_text_.at(i));
    }
    painter->end();
    delete painter;
    return;
    /*
    QRect s_rect = this->geometry();
    int t_count = _text_.size()-1;
    QFontMetrics font_metrics= this->fontMetrics();

    int font_h = font_metrics.height();
    for(int i = 0; i<_text_.size();i++)
    {
        //int value = minimum() + (i * (maximum()- minimum())) / _text_.size();
        //int pos = QStyle::sliderPositionFromValue(minimum(), maximum(), value, s_rect.width());
        int x_point = ((s_rect.width()/t_count?t_count:1)*i)-(font_metrics.horizontalAdvance(_text_.at(i))/2);
        int y_point = s_rect.height()-font_h;
        //QRect textRect(x_point - 50,y_point - font_metrics.ascent(), 100, font_metrics.height());
        painter->drawText(QPoint(x_point,y_point),_text_.at(i));
    }
    //painter->drawRect(s_rect);
    painter->end();
    delete painter;
    return;
    */
    /*
    QStyleOptionSlider opt;
    initStyleOption(&opt);

    QRect groove = style()->subControlRect(QStyle::CC_Slider, &opt,
                                           QStyle::SC_SliderGroove, this);

    if (groove.isEmpty()) groove = rect();

    int minV = minimum();
    int maxV = maximum();
    int count = qMax(1, _text_.size());
    QPainter p(this);
    p.setPen(palette().color(QPalette::WindowText));
    QFontMetrics fm = p.fontMetrics();

    for (int i = 0; i < count; ++i)
    {
        // compute value for tick i (e.g., evenly spaced)
        int value = minV + (i * (maxV - minV)) / qMax(1, count - 1);

        // Map value -> pixel position along groove
        int span = groove.width();
        int pos = QStyle::sliderPositionFromValue(minV, maxV, value, span, opt.upsideDown);
        int x = groove.left() + pos;

        // vertically place labels below groove
        int y = groove.bottom() + fm.height() + 4;

        QRect textRect(x - 50, y - fm.ascent(), 100, fm.height()); // width can be adjusted
        p.drawText(textRect, Qt::AlignHCenter | Qt::AlignTop, _text_.at(i));
    }
    */

}

QSize TimeSlider::sizeHint() const
{
    QSize size = QSlider::sizeHint();
    size.setHeight(size.height()+20);
    size.setWidth(size.width()+20);
    return size;
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
    QStringList list;
    for(int i = 0; i<_tick_number_;i++)
    {
        list.append(QString::number(i));
    }
    _slider_->loadTextLabels(list);
    return _slider_;
    /*
//    if(_grid_)
//    {
//        disconnect(_slider_,&QAbstractSlider::valueChanged,this,&GViewTimeTool::timeChanged);
//        _grid_->deleteLater();
//        _grid_ = nullptr;
//        _slider_ = nullptr;
//    }
//    _grid_ = new QGridLayout();
//    _grid_->setRowStretch(0,1);
//    _grid_->setRowStretch(1,0);

//    _slider_ = new QSlider(Qt::Horizontal);
//    _slider_->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
//    _grid_->setRowMinimumHeight(0,_slider_->sizeHint().height()+8);
//    _grid_->setVerticalSpacing(6);
//    _grid_->setHorizontalSpacing(0);
//    _grid_->setContentsMargins(0,0,0,0);
//    _slider_->setFocusPolicy(Qt::StrongFocus);
//    _slider_->setTickPosition(QSlider::TicksBelow);
//    _slider_->setObjectName("time_slider");
//    _slider_->setStyleSheet("QSlider#time_slider {"
//                            "  background: transparent;"
//                            "}"
////                            "QSlider#time_slider::groove:horizontal {"
////                            "  height: 8px;"
////                            "  margin: 0;            "
////                            "  border-radius: 2px;"
////                            "  background: transparent;"
////                            "}"
//                            "QSlider#time_slider::sub-page:horizontal {"
//                            "  background: blue;"
//                            "}"
//                            "QSlider#time_slider::handle:horizontal {"
//                            "  width: 14px;"
//                            "  height: 14px;"
//                            "  margin: -3px 0;      "
//                            "  border-radius: 8px;"
//                            "  background: #ffffff;"
//                            "  border: 1px solid #888;"
//                            "}"
//                            );
//    _slider_->setTickInterval(1);
//    _slider_->setSingleStep(1);
//    _slider_->setMinimum(0);
//    _slider_->setMaximum(_tick_number_-1);
//    connect(_slider_,&QAbstractSlider::valueChanged,this,&GViewTimeTool::timeChanged);
//    _grid_->addWidget(_slider_,0,0,0,0);//_tick_number_);
//    //QHBoxLayout* h_layout = new QHBoxLayout;
//    for(int i = 0; i<_tick_number_;i++)
//    {
//        QLabel* tick_text = new QLabel(QString::number(i));
//        //h_layout->addWidget(tick_text);
//        _grid_->addWidget(tick_text,1,i,Qt::AlignTop);
//        _grid_->setColumnStretch(i,1);
//    }
//    //_grid_->addLayout(h_layout,1,0);
//    return _grid_;
*/
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

