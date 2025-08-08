#include "touchform.h"
#include "ui_touchform.h"

TouchForm::TouchForm(QWidget *parent) :
    QWidget(parent),ui(new Ui::TouchForm),
    _paint_state_(0)
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

void TouchForm::paintState(int state)
{
    _paint_state_ = state;
    repaint();
    return;
}

void TouchForm::paintEvent(QPaintEvent* p_event)
{
    Q_UNUSED(p_event);
    switch(_paint_state_)
    {
    case 0:
    {
        QPainter painter1;
        painter1.begin(this);
        QPixmap pixm("C:/Users/Somny/Pictures/0087_001.png");
        QSize psze = pixm.size();
        painter1.setPen(QPen(Qt::red,8,Qt::DashDotLine,Qt::SquareCap,Qt::BevelJoin));
        psze.scale(300,150,Qt::KeepAspectRatio);
        pixm.scaled(psze,Qt::KeepAspectRatio);
        painter1.setBrush(QBrush(Qt::black,pixm));
        painter1.drawEllipse(0,0,500,330);
        painter1.end();
    }
        break;
    case 1:
    {
        int sepWidth = width()/3;
        //int sepHeight = height()/3;

        QPainter painter2;
        painter2.begin(this);
        QLinearGradient gradient(0,0,sepWidth,height());
        gradient.setColorAt(0,Qt::darkRed);
        gradient.setColorAt(0.5,Qt::blue);
        gradient.setColorAt(1,Qt::red);
        painter2.setBrush(gradient);
        painter2.drawRect(0,0,sepWidth,height());
        painter2.end();

        QPainter painter3;
        painter3.begin(this);
        QConicalGradient con_gradient(width()/2,height()/2,0);
        con_gradient.setColorAt(0,Qt::darkGreen);
        con_gradient.setColorAt(0.4,Qt::white);
        con_gradient.setColorAt(0.8,Qt::red);
        con_gradient.setColorAt(1,Qt::white);
        painter3.setBrush(con_gradient);
        painter3.drawRect(sepWidth,0,sepWidth*2,height());
        painter3.end();

        QPainter painter4;
        painter4.begin(this);
        QPointF fuuu(rect().center());
        QRadialGradient rad_grad(fuuu,width()/2,fuuu);
        rad_grad.setColorAt(0,Qt::white);
        rad_grad.setColorAt(0.5,Qt::red);
        rad_grad.setColorAt(1,Qt::white);
        painter4.setBrush(rad_grad);
        painter4.drawRect(sepWidth*2,0,width(),height());
        painter4.end();
    }
        break;
    case 2:
    {
        QPainter painter5;
        painter5.begin(this);
        painter5.setPen(QPen(Qt::black,5));
        int n = 60;
        for(int i = 0; i<n; ++i)
        {
            qreal rad_Angle = qDegreesToRadians(360.0*i/n);
            //Адзін радыян гэта вугал насупраць аркі акружнасьці даўжынёй з радыюс.
            //R=Ark 360 degree = 2Pi rad, 180 = 1Pi rad
            QPointF cent(rect().center());
            qreal x = cent.x()+ cos(rad_Angle)*(cent.x()/2);
            qreal y = cent.y()+ sin(rad_Angle)*(cent.y()/2);
            painter5.drawPoint(QPointF(x,y));
            //Усё не вельмі складана. Гіпацінуза акружнасьці вядома, як і вугал паміж ёй
            //і смежным бокам. Каб дазьведацца каардынаты Х, нам трэба памножыць косінус
            //на даўжыню гіпацінузы, бо (cos() = adj_side/hypot) ->(adj_side = cos()*hypot)
            //таксама сінус дазваляе вылічыць каардынату У,
            //бо (sin() = oppos_side/hypot)->(oppos_side = sin*hypot)
        }
        painter5.end();
    }
        break;
    case 3:
    {
        QPainter painter6;
        painter6.begin(this);
        painter6.setPen(QPen(Qt::red,8,Qt::DashDotLine,Qt::SquareCap,Qt::BevelJoin));
        painter6.setRenderHint(QPainter::Antialiasing,true);
        painter6.drawLine(QPointF(10,10),QPointF(400,40));
        painter6.end();

        QPainter painter7;
        painter7.begin(this);
        painter7.setRenderHint(QPainter::Antialiasing,true);
        painter7.setPen(QPen(Qt::darkBlue,4,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin));
        const int n = 10;
        QPointF a[n];
        QPointF centr(rect().center());
        for(int i =0; i<n;++i)
        {
            qreal fAngle = qDegreesToRadians(360.0*i/n);

            qreal x = centr.x()+cos(fAngle)*100;
            qreal y = centr.y()+sin(fAngle)*100;
            a[i] = QPointF(x,y);
        }
        painter7.drawPolyline(a,n);
        painter7.end();
    }
        break;
    case 4:
    {
        QPainter painter8;
        painter8.begin(this);
        QBrush brush(Qt::green,Qt::Dense1Pattern);
        painter8.fillRect(50,50,400,400,brush);
        painter8.eraseRect(100,100,300,300);
        painter8.end();
    }
        break;
    }
//    painter.setRenderHint(QPainter::Antialiasing,true);
//    int colour_count = _color_list_.count();
//    foreach(QTouchEvent::TouchPoint tp, _touchpoint_list_)
//    {
//        if(tp.state() == QEventPoint::Stationary)
//        {
//            continue;
//        }
//        QColor n_color(_color_list_.at(tp.id()%colour_count));
//        painter.setPen(n_color);
//        painter.setBrush(n_color);

//        QRectF r1(tp.position(),QSize(20,20));
//        QRectF r2(tp.pressPosition(),QSize(20,20));
//        painter.drawEllipse(r1.translated(-10,-10));
//        painter.drawEllipse(r2.translated(-10,-10));
//        painter.drawLine(tp.position(),tp.pressPosition());
//    }
}

bool TouchForm::event (QEvent * reg_event)
{
    //qDebug()<<"Event type: " << reg_event->type();
    switch(reg_event->type())
    {
    case QEvent::TouchBegin:
    case QEvent::TouchUpdate:
    case QEvent::TouchEnd:
    {
        QTouchEvent * t_event = static_cast<QTouchEvent*>(reg_event);
        _touchpoint_list_ = t_event->points();
//        this->update();
    }
    default:
    {
        return QWidget::event(reg_event);
    }
    }
    return  true;
}
