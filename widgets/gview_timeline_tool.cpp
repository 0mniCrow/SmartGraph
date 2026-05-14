#include "gview_timeline_tool.h"

void TimeSlider::paintEvent(QPaintEvent* p_event)
{
    QSlider::paintEvent(p_event);

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

