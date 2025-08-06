#include "touchform.h"
#include "ui_touchform.h"

TouchForm::TouchForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TouchForm)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_AcceptTouchEvents, true);
    _color_list_<<Qt::cyan<<Qt::green<<Qt::blue<<Qt::black
               <<Qt::red<<Qt::magenta<<Qt::darkYellow
              <<Qt::gray<<Qt::darkCyan<<Qt::darkBlue;
}

TouchForm::~TouchForm()
{
    delete ui;
}


void TouchForm::paintEvent(QPaintEvent* p_event)
{
    Q_UNUSED(p_event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    int colour_count = _color_list_.count();
    foreach(QTouchEvent::TouchPoint tp, _touchpoint_list_)
    {
        if(tp.state() == QEventPoint::Stationary)
        {
            continue;
        }
        QColor n_color(_color_list_.at(tp.id()%colour_count));
        painter.setPen(n_color);
        painter.setBrush(n_color);

        QRectF r1(tp.position(),QSize(20,20));
        QRectF r2(tp.pressPosition(),QSize(20,20));
        painter.drawEllipse(r1.translated(-10,-10));
        painter.drawEllipse(r2.translated(-10,-10));
        painter.drawLine(tp.position(),tp.pressPosition());
    }
}

bool TouchForm::event (QEvent * reg_event)
{
    qDebug()<<"Event type: " << reg_event->type();
    switch(reg_event->type())
    {
    case QEvent::TouchBegin:
    case QEvent::TouchUpdate:
    case QEvent::TouchEnd:
    {
        QTouchEvent * t_event = static_cast<QTouchEvent*>(reg_event);
        _touchpoint_list_ = t_event->points();
        this->update();
    }
    default:
    {
        return QWidget::event(reg_event);
    }
    }
    return  true;
}
