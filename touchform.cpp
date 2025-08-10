#include "touchform.h"
#include "ui_touchform.h"

TouchForm::TouchForm(QWidget *parent) :
    QWidget(parent),ui(new Ui::TouchForm),
    _paint_state_(0),cur_x(50),cur_y(50),cur_x_size(30),cur_y_size(30)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_AcceptTouchEvents, true);
    _color_list_<<Qt::cyan<<Qt::green<<Qt::blue<<Qt::black
               <<Qt::red<<Qt::magenta<<Qt::darkYellow
              <<Qt::gray<<Qt::darkCyan<<Qt::darkBlue;
    ui->spinBox->setMinimum(0);
    connect(ui->spinBox,&QSpinBox::valueChanged,this,&TouchForm::paintState);
    QPicture pic;
    QPainter painter;
    painter.begin(&pic);
    painter.setBrush(QBrush(Qt::white));
    painter.setPen(QPen(Qt::red));
    painter.drawLine(20,20,50,50);
    painter.end();
    if(!pic.save("myline.dat"))
    {
        qDebug()<<"Can't save the file";
    }

    QGridLayout* grid = new QGridLayout;
    grid->addWidget(generate_label(QPainter::CompositionMode_Source),0,0);
    grid->addWidget(new QLabel("<CENTER>Source</CENTER>"),1,0);

    grid->addWidget(generate_label(QPainter::CompositionMode_SourceOver),0,1);
    grid->addWidget(new QLabel("<CENTER>SourceOver</CENTER>"),1,1);

    grid->addWidget(generate_label(QPainter::CompositionMode_SourceIn),0,2);
    grid->addWidget(new QLabel("<CENTER>SourceIn</CENTER>"),1,2);

    grid->addWidget(generate_label(QPainter::CompositionMode_SourceOut),0,3);
    grid->addWidget(new QLabel("<CENTER>SourceOut</CENTER>"),1,3);

    grid->addWidget(generate_label(QPainter::CompositionMode_SourceAtop),0,4);
    grid->addWidget(new QLabel("<CENTER>SourceAtop</CENTER>"),1,4);

    grid->addWidget(generate_label(QPainter::CompositionMode_Clear),0,5);
    grid->addWidget(new QLabel("<CENTER>Clear</CENTER>"),1,5);

    grid->addWidget(generate_label(QPainter::CompositionMode_Destination),2,0);
    grid->addWidget(new QLabel("<CENTER>Destination</CENTER>"),3,0);

    grid->addWidget(generate_label(QPainter::CompositionMode_DestinationOver),2,1);
    grid->addWidget(new QLabel("<CENTER>DestinationOver</CENTER>"),3,1);

    grid->addWidget(generate_label(QPainter::CompositionMode_DestinationIn),2,2);
    grid->addWidget(new QLabel("<CENTER>DestinationIn</CENTER>"),3,2);

    grid->addWidget(generate_label(QPainter::CompositionMode_DestinationOut),2,3);
    grid->addWidget(new QLabel("<CENTER>DestinationOut</CENTER>"),3,3);

    grid->addWidget(generate_label(QPainter::CompositionMode_DestinationAtop),2,4);
    grid->addWidget(new QLabel("<CENTER>DestinationAtop</CENTER>"),3,4);

    grid->addWidget(generate_label(QPainter::CompositionMode_Xor),2,5);
    grid->addWidget(new QLabel("<CENTER>Xor</CENTER>"),3,5);
    wgt.setLayout(grid);

    QGraphicsBlurEffect* blur_eff = new QGraphicsBlurEffect;
    QGraphicsDropShadowEffect* shad_eff = new QGraphicsDropShadowEffect;
    QGraphicsColorizeEffect* col_eff = new QGraphicsColorizeEffect;
    QGraphicsOpacityEffect* op_eff = new QGraphicsOpacityEffect;

    QFormLayout* m_form = new QFormLayout;
    m_form->addRow("No Effects", generate_label2(nullptr));
    m_form->addRow("Blur",generate_label2(blur_eff));
    m_form->addRow("Drop Shadow",generate_label2(shad_eff));
    m_form->addRow("Colorize",generate_label2(col_eff));
    m_form->addRow("Opacity",generate_label2(op_eff));
    wgt2.setLayout(m_form);

}

void TouchForm::wgt_show()
{
    wgt.show();
    return;
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
        QPixmap pixm("wheel.png");
        QSize psze = pixm.size();
        painter1.setPen(QPen(Qt::red,8,Qt::DashDotLine,Qt::SquareCap,Qt::BevelJoin));
        psze.scale(300,150,Qt::IgnoreAspectRatio);
        pixm.scaled(psze,Qt::IgnoreAspectRatio);
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
    case 5:
    {
        int x = 10;
        int y = 10;
        QPainter painter9;
        painter9.begin(this);
        painter9.setRenderHint(QPainter::Antialiasing,true);
        painter9.setBrush(QBrush(Qt::red,Qt::DiagCrossPattern));
        painter9.setPen(QPen(Qt::blue,3,Qt::DashLine));
        painter9.drawRect(QRect(x,y,110,70));
        x+=120;
        //y+=80;
        painter9.end();
        QPainter painter10;
        painter10.begin(this);
        painter10.setRenderHint(QPainter::Antialiasing,true);
        painter10.setBrush(QBrush(Qt::green));
        painter10.setPen(QPen(Qt::black));
        painter10.drawRoundedRect(QRect(x,y,110,70),30,30);
        x+=120;
        //y+=80;
        painter10.end();
        QPainter painter11;
        painter11.begin(this);
        painter11.setRenderHint(QPainter::Antialiasing,true);
        painter11.setBrush(QBrush(Qt::green,Qt::CrossPattern));
        painter11.setPen(QPen(Qt::red,3,Qt::DotLine));
        painter11.drawEllipse(QRect(x,y,110,70));
        x=10;
        y+=80;
        painter11.end();

        QPainter painter12;
        painter12.begin(this);
        painter12.setRenderHint(QPainter::Antialiasing,true);
        painter12.setBrush(QBrush(Qt::yellow));
        painter12.setPen(QPen(Qt::blue));
        painter12.drawChord(QRect(x,y,110,70),45*16,200*16);
        x+=120;
        painter12.end();

        QPainter painter13;
        painter13.begin(this);
        painter13.setRenderHint(QPainter::Antialiasing,true);
        painter13.setBrush(QBrush(Qt::black,Qt::BDiagPattern));
        painter13.setPen(QPen(Qt::blue,4));
        painter13.drawPie(QRect(x,y,110,70),90*16,270*16);
        x+=120;
        painter13.end();

        QPainter painter14;
        painter14.begin(this);
        painter14.setRenderHint(QPainter::Antialiasing,true);
        painter14.setBrush(QBrush(Qt::lightGray));
        painter14.setPen(QPen(Qt::black));
        int n = 8;
        QPolygonF polygon;
        for(int i = 0; i<n; ++i)
        {
            qreal fAngle = qDegreesToRadians(360.0*i/n);
            qreal n_x = x+50 + cos(fAngle)*40;
            qreal n_y = y+50 + sin(fAngle)*40;
            polygon.append(QPointF(n_x,n_y));
        }
        painter14.drawPolygon(polygon);
        painter14.end();
    }
        break;
    case 6:
    {
        QPicture pic;
        if(!pic.load("myline.dat"))
        {
            qDebug()<<"can't load the file";
            break;
        }
        QPainter painter15;
        painter15.begin(this);
        painter15.drawPicture(QPoint(40,40),pic);
        painter15.end();
    }
        break;
    case 7:
    {
        QPainterPath path;
        QPointF pt1(width(),height()/2);
        QPointF pt2(width()/2,-height());
        QPointF pt3(width()/2,height()*2);
        QPointF pt4(0,height()/2);
        path.moveTo(pt1);
        path.cubicTo(pt2,pt3,pt4);
        QRect rect(width()/4,height()/4,width()/2,height()/2);
        path.addRect(rect);
        path.addEllipse(rect);
        QPainter painter16;
        painter16.begin(this);
        painter16.setRenderHint(QPainter::Antialiasing,true);
        painter16.setPen(QPen(Qt::blue,6));
        painter16.drawPath(path);
        painter16.end();
    }
        break;
    case 8:
    {
        QPainterPath path;
        path.addRect(20,20,60,60);
        path.moveTo(0,0);
        path.cubicTo(99,0,50,50,99,99);
        path.cubicTo(0,99,50,50,0,0);
        path.setFillRule(Qt::WindingFill);

        QPainter painter17;
        painter17.begin(this);
        painter17.fillRect(0,0,100,100,Qt::white);
        painter17.setPen(QPen(QColor(79,106,25),1,Qt::SolidLine,Qt::FlatCap,Qt::MiterJoin));
        painter17.setBrush(QColor(122,163,39));
        painter17.drawPath(path);
        painter17.end();
    }
        break;
    case 9:
    {
        QPainter painter18;
        painter18.begin(this);
        painter18.setRenderHint(QPainter::Antialiasing,true);
        painter18.setClipRect(0,0,100,100);
        painter18.setBrush(QBrush(Qt::green));
        painter18.setPen(QPen(Qt::black,2));
        painter18.drawEllipse(0,0,200,100);
        painter18.end();
    }
        break;
    case 10:
    {
        QPainter painter19;
        painter19.begin(this);
        QRegion region1(0,0,200,200);
        QRegion region2(20,20,180,160,QRegion::Ellipse);
        QRegion region3 = region1.subtracted(region2);
        painter19.setRenderHint(QPainter::Antialiasing,true);
        painter19.setClipRegion(region3);
        painter19.setBrush(QBrush(Qt::blue));
        painter19.setPen(QPen(Qt::black,3));
        painter19.drawEllipse(0,0,200,200);
        painter19.end();
    }
        break;
    case 11:
    {
        QPainter painter20;
        painter20.begin(this);
        painter20.setRenderHint(QPainter::Antialiasing,true);
        painter20.setBrush(QBrush(Qt::darkRed,Qt::Dense3Pattern));
        painter20.setPen(QPen(Qt::black,2,Qt::DotLine));
        painter20.drawEllipse(cur_x,cur_y,cur_x_size,cur_y_size);
        painter20.end();
    }
    default:
    {

    }
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
//        QTouchEvent * t_event = static_cast<QTouchEvent*>(reg_event);
//        _touchpoint_list_ = t_event->points();
//        this->update();
    }
        break;

    default:
    {
        return QWidget::event(reg_event);
    }
    }
    return  true;
}

void TouchForm::keyPressEvent(QKeyEvent* key_event)
{
    switch(key_event->key())
    {
    case Qt::Key_Up:
    {
        if(cur_y>0)
        {
            cur_y-=10;
            if(cur_y<0)
            {
                cur_y=0;
            }
            key_event->accept();
            repaint();
        }
    }
        break;
    case Qt::Key_Down:
    {
        if(cur_y+cur_y_size<height())
        {
            cur_y+=10;
            if(cur_y+cur_y_size>=height())
            {
                cur_y = height()-cur_y_size-1;
            }
            key_event->accept();
            repaint();
        }
    }
        break;
    case Qt::Key_Left:
    {
        if(cur_x>0)
        {
            cur_x-=10;
            if(cur_x<0)
            {
                cur_x = 0;
            }
            key_event->accept();
            repaint();
        }
    }
        break;
    case Qt::Key_Right:
    {
        if(cur_x+cur_x_size<width())
        {
            cur_x+=10;
            if(cur_x+cur_x_size>=width())
            {
                cur_x = width()-cur_x_size-1;
            }
            key_event->accept();
            repaint();
        }
    }
        break;
    case Qt::Key_Q:
    {
        if(key_event->modifiers()&Qt::ControlModifier)
        {
            wgt.show();
            key_event->accept();
        }
    }
        break;
    case Qt::Key_E:
    {
        if(key_event->modifiers()&Qt::ControlModifier)
        {
            wgt2.show();
            key_event->accept();
        }
    }
    }
    QWidget::keyPressEvent(key_event);
    return;
}

QLabel * TouchForm::generate_label(const QPainter::CompositionMode& mode)
{
    QLabel * n_label = new QLabel;
    n_label->setFixedSize(100,100);
    QRect rectangle(n_label->contentsRect());
    QPainter painter;
    QImage sourceImage(rectangle.size(),QImage::Format_ARGB32_Premultiplied);
    sourceImage.fill(QColor(0,0,0,0));
    painter.begin(&sourceImage);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.setBrush(QBrush(QColor(0,0,255)));
    painter.drawPolygon(
                QPolygon()<<
                rectangle.bottomLeft()<<
                QPoint(rectangle.center().x(),0)<<
                rectangle.bottomRight());
    painter.end();

    QImage resultImage(rectangle.size(),QImage::Format_ARGB32_Premultiplied);
    resultImage.fill(Qt::transparent);
    painter.begin(&resultImage);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.setPen(QPen(QColor(0,255,255),4));
    painter.setBrush(QBrush(QColor(255,0,0)));
//    painter.drawPolygon(QPolygon()<<
//                         rectangle.topLeft()<<
//                         QPoint(rectangle.center().x(),rectangle.bottom())<<
//                                rectangle.topRight());
    painter.drawEllipse(rectangle.center(),rectangle.width()/2,rectangle.height()/2);
    painter.setCompositionMode(mode);
    painter.drawImage(rectangle,sourceImage);
    painter.end();

    n_label->setPixmap(QPixmap::fromImage(resultImage));
    return n_label;
}
QLabel * TouchForm::generate_label2(QGraphicsEffect* graphic_effect)
{
    QLabel * n_label = new QLabel;
    QPixmap pixmap("yo-lee.png");
    n_label->setPixmap(pixmap.scaled(200,200,Qt::KeepAspectRatio));

    if(graphic_effect)
    {
        n_label->setGraphicsEffect(graphic_effect);
    }
    return n_label;
}


void TouchForm::closeEvent(QCloseEvent*  cl_event)
{
    wgt.close();
    wgt2.close();
    cl_event->accept();
    return;
}
