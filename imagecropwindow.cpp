#include "imagecropwindow.h"
#include "ui_imagecropwindow.h"
#include "touchform.h"

ImageCropWindow::ImageCropWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageCropWindow)
{
    ui->setupUi(this);
    ui->spin_crop_width->setValue(DEF_WIDTH);
    connect(ui->button_imgFileDialog,&QPushButton::clicked,this,&ImageCropWindow::chooseFile);
    connect(ui->button_loadImage,&QPushButton::clicked,this,&ImageCropWindow::loadImage);
    connect(ui->button_Crop,&QPushButton::clicked,this,&ImageCropWindow::cropImage);
    connect(ui->spinBox_radius,&QSpinBox::valueChanged,this,&ImageCropWindow::radiusChanged);
    connect(ui->combo_form,&QComboBox::currentIndexChanged,this,&ImageCropWindow::geometryChange);
    connect(ui->spin_crop_width,&QSpinBox::valueChanged,this,&ImageCropWindow::thicknessChanged);
    _scene_ = new CropScene(ui->graphicsView);
    ui->graphicsView->setScene(_scene_);
    _item_ = nullptr;
    _r_item_ = nullptr;
}

ImageCropWindow::~ImageCropWindow()
{
    delete ui;
}

void ImageCropWindow::chooseFile()
{
    QString filename(QFileDialog::getOpenFileName(
                         this,"Open Image to crop",QDir::currentPath(),
                         "Images (*.jpg *.png *.bmp)",nullptr,QFileDialog::DontUseNativeDialog));
    ui->line_imgAddr->setText(filename);
}
QPixmap ImageCropWindow::getIMG()
{
    QString file_addr(ui->line_imgAddr->text());
    if(!QFileInfo::exists(file_addr))
        return QPixmap();
    QImage bg(file_addr);
    if(bg.isNull())
        return QPixmap();
    return QPixmap::fromImage(bg);
}

void ImageCropWindow::loadImage()
{
    if(_r_item_)
    {
        _scene_->removeItem(_r_item_);
        delete _r_item_;
    }
    if(_item_)
    {       
        _scene_->removeItem(_item_);
        delete _item_;
    }
    QPixmap bg(getIMG());
    if(bg.isNull())
    {
        return;
    }
    _scene_->loadPixmap(bg);
    _item_ = new CropItem();
    _r_item_ = new ResizeItem(_item_);
    _scene_->addItem(_item_);
    _scene_->addItem(_r_item_);
    QRectF scene_r(_scene_->sceneRect());
    _item_->setPos(scene_r.center());
    return;
}

void ImageCropWindow::cropImage()
{
    QPixmap bg(getIMG());
    if(bg.isNull())
    {
        return;
    }
    qreal radius = _item_->radius()-DEF_WIDTH;
    QPixmap crop(radius*2,radius*2);
    crop.fill(Qt::transparent);
    QPainter painter(&crop);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(Qt::white);
    painter.drawEllipse(0,0,radius*2,radius*2);
    QPointF center(_item_->sceneCenterPoint());
    QRectF target(center.x()-radius,center.y()-radius,radius*2.0,radius*2.0);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    QPixmap copy(bg.copy(target.toRect()));
    painter.drawPixmap(crop.rect(),copy);
    painter.end();
    LocWidget* widget = new LocWidget;
    widget->setAttribute(Qt::WA_TranslucentBackground);
    widget->setPixmap(crop);
    QPushButton* exit = new QPushButton("X");
    exit->setFixedSize(20,20);
    QObject::connect(exit,&QPushButton::clicked,widget,&QWidget::close);
    QObject::connect(this,&QObject::destroyed,widget,&QObject::deleteLater);
    QVBoxLayout* vert_layout = new QVBoxLayout;
    vert_layout->addWidget(exit);
    vert_layout->addStretch(1);
    widget->setLayout(vert_layout);
    widget->show();
    return;
}

void ImageCropWindow::radiusChanged(int radius)
{
    if(!_item_)
    {
        return;
    }
    _item_->setRadius(radius);
    return;
}

void ImageCropWindow::thicknessChanged(int val)
{
    if(!_item_)
    {
        return;
    }
    _item_->setThickness(val);
    return;
}

void ImageCropWindow::geometryChange(int geometry)
{
    if(!_item_)
    {
        return;
    }
    char new_geom = CropItem::Geometry::CI_INVALID;
    switch(geometry)
    {
    case 0:
    {
        new_geom = CropItem::Geometry::CI_CIRCLE;
    }
        break;
    case 1:
    {
        new_geom = CropItem::Geometry::CI_SQUARE;
    }
        break;
    case 2:
    {
        new_geom = CropItem::Geometry::CI_TRIANGLE;
    }
        break;
    }

    _item_->setGeometryType(new_geom);
    if(_r_item_)
    {
        _r_item_->update();
    }
    return;
}

CropItem::CropItem(qreal radius, qreal thickness):
    _radius_(radius),
    _thickness_(thickness),
    _geom_type_(CI_CIRCLE)
{
    setFlags(ItemIsMovable|ItemSendsGeometryChanges);
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    return;
}
void CropItem::setRadius(qreal radius)
{
    prepareGeometryChange();
    _radius_ = radius;
    auto child_items(childItems());
    if(!child_items.size())
    {
        return;
    }
    ResizeItem* res_item = dynamic_cast<ResizeItem*>(child_items.first());
    if(res_item)
    {
        res_item->update();
    }
    return;
}

void CropItem::setThickness(qreal val)
{
    _thickness_ = val;
    update();
    return;
}

void CropItem::moveRadius(qreal val)
{
    prepareGeometryChange();
    _radius_+=val;
    return;
}

void CropItem::setGeometryType(char g_type)
{
    _geom_type_ = g_type;
    update();
    return;
}

qreal CropItem::radius() const
{
    return _radius_;
}

QPointF CropItem::sceneCenterPoint() const
{
    return mapToScene(boundingRect().center());
}

QRectF CropItem::boundingRect() const
{
    QRectF rect;
    rect.setX(-_radius_-DEF_OUTLINE);
    rect.setY(-_radius_-DEF_OUTLINE);
    rect.setWidth(_radius_*2+DEF_OUTLINE);
    rect.setHeight(_radius_*2+DEF_OUTLINE);
    return rect;
}
QPainterPath CropItem::shape() const
{
    QPainterPath path;
    path.setFillRule(Qt::OddEvenFill);
    if(_geom_type_==CI_CIRCLE)
    {
        path.addEllipse(QPointF(0,0),
                    _radius_+DEF_OUTLINE,
                    _radius_+DEF_OUTLINE);
        path.addEllipse(QPointF(0,0),
                    _radius_-_thickness_,//DEF_WIDTH,
                    _radius_-_thickness_);//-DEF_WIDTH);
    }
    else if(_geom_type_==CI_SQUARE)
    {
        path.addRect(-_radius_-DEF_OUTLINE,
                     -_radius_-DEF_OUTLINE,
                     _radius_*2+DEF_OUTLINE,
                     _radius_*2+DEF_OUTLINE);
        path.addRect(-_radius_-DEF_OUTLINE+_thickness_,//DEF_WIDTH,
                     -_radius_-DEF_OUTLINE+_thickness_,//DEF_WIDTH,
                     (_radius_-_thickness_/*DEF_WIDTH*/)*2+DEF_OUTLINE,
                     (_radius_-_thickness_/*DEF_WIDTH*/)*2+DEF_OUTLINE);
    }
    else if(_geom_type_==CI_TRIANGLE)
    {
        QRectF rect(-_radius_-DEF_OUTLINE,
                    -_radius_-DEF_OUTLINE,
                    _radius_*2+DEF_OUTLINE,
                    _radius_*2+DEF_OUTLINE);
        QPolygonF polygon;
        polygon<<rect.bottomLeft()<<
                 QPointF(rect.center().x(),rect.topLeft().y())<<
                 rect.bottomRight()<<rect.bottomLeft();
        path.addPolygon(polygon);

        QPolygonF smol_polygon;
        for(int i = 0; i< polygon.size()-1;i++)
        {
            qreal dx = polygon.at(i).x();
            qreal dy = polygon.at(i).y();
            qreal len = std::hypot(dx,dy);
            if(len<=0.0)
            {
                smol_polygon<<polygon.at(i);
                continue;
            }
            qreal new_len = len - _thickness_;//DEF_WIDTH;
            if(new_len <=0.0)
            {
                new_len = len;
            }
            qreal ux = dx/len;
            qreal uy = dy/len;
            QPointF new_point(ux*new_len,uy*new_len);
            smol_polygon<<new_point;
        }
        if(smol_polygon.size())
        {
            smol_polygon.append(smol_polygon.at(0));
        }
        path.addPolygon(smol_polygon);
//        polygon.clear();
//        rect.setRect(-_radius_-DEF_OUTLINE+DEF_WIDTH,
//                     -_radius_-DEF_OUTLINE+DEF_WIDTH,
//                     (_radius_-DEF_WIDTH)*2+DEF_OUTLINE,
//                     (_radius_-DEF_WIDTH)*2+DEF_OUTLINE);
//        polygon<<rect.bottomLeft()<<
//                 QPointF(rect.center().x(),rect.topLeft().y())<<
//                 rect.bottomRight()<<rect.bottomLeft();
//        path.addPolygon(polygon);
    }
    return path;
}

void CropItem::paint(QPainter* painter,
           const QStyleOptionGraphicsItem* option,
           QWidget* widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    QPen outlinePen(Qt::black,DEF_OUTLINE);
    QColor t_color("indigo");
    t_color.setAlpha(120);
    QBrush t_brush(t_color);
    QPainterPath path;
    path.setFillRule(Qt::OddEvenFill);
    if(_geom_type_ == CI_CIRCLE)
    {
        path.addEllipse(QPointF(0,0),
                        _radius_,
                        _radius_);
        path.addEllipse(QPointF(0,0),
                        _radius_-_thickness_,//DEF_WIDTH,
                        _radius_-_thickness_);//DEF_WIDTH);
    }
    else if(_geom_type_==CI_SQUARE)
    {
        path.addRect(-_radius_,
                     -_radius_,
                     _radius_*2,
                     _radius_*2);
        path.addRect(-_radius_+_thickness_,//DEF_WIDTH,
                     -_radius_+_thickness_,//DEF_WIDTH,
                     (_radius_-_thickness_/*DEF_WIDTH*/)*2,
                     (_radius_-_thickness_/*DEF_WIDTH*/)*2);
    }
    else if(_geom_type_==CI_TRIANGLE)
    {
        QRectF rect(-_radius_,
                    -_radius_,
                    _radius_*2,
                    _radius_*2);
        QPolygonF polygon;
        polygon<<rect.bottomLeft()<<
                 QPointF(rect.center().x(),rect.topLeft().y())<<
                 rect.bottomRight()<<rect.bottomLeft();
        path.addPolygon(polygon);

        QPolygonF smol_polygon;
        for(int i = 0; i< polygon.size()-1;i++)
        {
            qreal dx = polygon.at(i).x();
            qreal dy = polygon.at(i).y();
            qreal len = std::hypot(dx,dy);
            if(len<=0.0)
            {
                smol_polygon<<polygon.at(i);
                continue;
            }
            qreal new_len = len - _thickness_;//DEF_WIDTH;
            if(new_len <=0.0)
            {
                new_len = len;
            }
            qreal ux = dx/len;
            qreal uy = dy/len;
            QPointF new_point(ux*new_len,uy*new_len);
            smol_polygon<<new_point;
        }
        if(smol_polygon.size())
        {
            smol_polygon.append(smol_polygon.at(0));
        }
        path.addPolygon(smol_polygon);
//        polygon.clear();
//        rect.setRect(-_radius_+DEF_WIDTH,
//                     -_radius_+DEF_WIDTH,
//                     (_radius_-DEF_WIDTH)*2,
//                     (_radius_-DEF_WIDTH)*2);
//        polygon<<rect.bottomLeft()<<
//                 QPointF(rect.center().x(),rect.topLeft().y())<<
//                 rect.bottomRight()<<rect.bottomLeft();
//        path.addPolygon(polygon);
    }
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing,true);
    painter->setPen(outlinePen);
    painter->setBrush(t_brush);
    painter->drawPath(path);
    painter->restore();
    return;
}

void CropItem::mousePressEvent(QGraphicsSceneMouseEvent * m_event)
{
    update();
    QGraphicsItem::mousePressEvent(m_event);
}

void CropItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * m_event)
{
    update();
    QGraphicsItem::mouseReleaseEvent(m_event);
}

void CropItem::mouseMoveEvent(QGraphicsSceneMouseEvent* m_event)
{
    QGraphicsItem::mouseMoveEvent(m_event);
}

CropScene::CropScene(QObject* tata):QGraphicsScene(tata)
{
    return;
}
void CropScene::loadPixmap(const QPixmap& bg)
{
    setSceneRect(bg.rect());
    _bg_ = bg;
    update();
    return;
}

void CropScene::drawBackground(QPainter* painter, const QRectF & rect)
{
    Q_UNUSED(rect);
    painter->save();
    painter->drawPixmap(rect,_bg_,rect);
    painter->restore();
    return;
}

ResizeItem::ResizeItem(CropItem* tata, qreal thickness):QGraphicsItem(tata),_thickness_(thickness)
{
    _main_item_ = tata;
    setFlags(ItemIsMovable|ItemIgnoresParentOpacity);
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    return;
}


QRectF ResizeItem::boundingRect() const
{
    qreal m_rad = _main_item_->radius()+DEF_OUTLINE;
    QRectF rect(-m_rad-_thickness_-DEF_CONTROL_OUTLINE,
                  -m_rad-_thickness_-DEF_CONTROL_OUTLINE,
                  (m_rad+_thickness_)*2+DEF_CONTROL_OUTLINE,
                  (m_rad+_thickness_)*2+DEF_CONTROL_OUTLINE);
    //Незразумела чаму, але не хапае 4 піксела каб хапала для малявання аб'екта.
    return rect;
}
QPainterPath ResizeItem::shape() const
{
    QPainterPath path;
    path.setFillRule(Qt::OddEvenFill);
    if(_main_item_->geometryType()==CropItem::Geometry::CI_CIRCLE)
    {
        qreal radius = _main_item_->radius()+DEF_OUTLINE;
        path.addEllipse(QPointF(0,0),
                        radius+_thickness_+DEF_CONTROL_OUTLINE,
                        radius+_thickness_+DEF_CONTROL_OUTLINE);
        path.addEllipse(QPointF(0,0),
                        radius,
                        radius);
    }
    else if(_main_item_->geometryType()==CropItem::Geometry::CI_SQUARE)
    {
        qreal length = _main_item_->radius();
        path.addRect(-length-DEF_OUTLINE-_thickness_-DEF_CONTROL_OUTLINE,
                     -length-DEF_OUTLINE-_thickness_-DEF_CONTROL_OUTLINE,
                     (length+DEF_OUTLINE+_thickness_)*2+DEF_CONTROL_OUTLINE,
                     (length+DEF_OUTLINE+_thickness_)*2+DEF_CONTROL_OUTLINE);
        path.addRect(-length-DEF_OUTLINE,
                     -length-DEF_OUTLINE,
                     length*2+DEF_OUTLINE,
                     length*2+DEF_OUTLINE);
    }
    else if(_main_item_->geometryType()==CropItem::Geometry::CI_TRIANGLE)
    {
        qreal side_len = _main_item_->radius();
        QRectF rect(-side_len-DEF_OUTLINE-_thickness_-DEF_CONTROL_OUTLINE,
                    -side_len-DEF_OUTLINE-_thickness_-DEF_CONTROL_OUTLINE,
                    (side_len+DEF_OUTLINE+_thickness_)*2+DEF_CONTROL_OUTLINE,
                    (side_len+DEF_OUTLINE+_thickness_)*2+DEF_CONTROL_OUTLINE);
        QPolygonF polygon;
        polygon<<rect.bottomLeft()<<
                 QPointF(rect.center().x(),rect.topLeft().y())<<
                 rect.bottomRight()<<rect.bottomLeft();
        path.addPolygon(polygon);

        polygon.clear();
        rect.setRect(   -side_len-DEF_OUTLINE,
                        -side_len-DEF_OUTLINE,
                        side_len*2+DEF_OUTLINE,
                        side_len*2+DEF_OUTLINE);
        polygon<<rect.bottomLeft()<<
                 QPointF(rect.center().x(),rect.topLeft().y())<<
                 rect.bottomRight()<<rect.bottomLeft();
        path.addPolygon(polygon);
    }
    return path;
}
void ResizeItem::paint(QPainter* painter,
           const QStyleOptionGraphicsItem* option,
           QWidget* widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    QPen outlinePen(Qt::black,DEF_CONTROL_OUTLINE);
    QColor color("seashell");
    color.setAlpha(120);
    QBrush brush(color);

    QPainterPath path;
    path.setFillRule(Qt::OddEvenFill);
    char m_geometry = _main_item_->geometryType();
    if(m_geometry == CropItem::Geometry::CI_CIRCLE)
    {
        qreal m_rad = _main_item_->radius()+DEF_OUTLINE;
        path.addEllipse(QPointF(0,0),
                        m_rad+_thickness_,
                        m_rad+_thickness_);
        path.addEllipse(QPointF(0,0),
                        m_rad,
                        m_rad);
    }
    else if(m_geometry==CropItem::Geometry::CI_SQUARE)
    {
        qreal m_len = _main_item_->radius();
        path.addRect(-m_len-DEF_OUTLINE-_thickness_,
                     -m_len-DEF_OUTLINE-_thickness_,
                     (m_len+DEF_OUTLINE+_thickness_)*2,
                     (m_len+DEF_OUTLINE+_thickness_)*2);
        path.addRect(-m_len-DEF_OUTLINE,
                     -m_len-DEF_OUTLINE,
                     m_len*2+DEF_OUTLINE,
                     m_len*2+DEF_OUTLINE);
    }
    else if(m_geometry==CropItem::Geometry::CI_TRIANGLE)
    {
        qreal m_side = _main_item_->radius();
        QRectF rect(-m_side-DEF_OUTLINE-_thickness_,
                    -m_side-DEF_OUTLINE-_thickness_,
                    (m_side+DEF_OUTLINE+_thickness_)*2,
                    (m_side+DEF_OUTLINE+_thickness_)*2);
        QPolygonF polygon;
        polygon<<rect.bottomLeft()<<
                 QPointF(rect.center().x(),rect.topLeft().y())<<
                 rect.bottomRight()<<rect.bottomLeft();
        path.addPolygon(polygon);
        polygon.clear();

        //QPointF delta(mapFromItem(_src_item_,0,0) - mapFromItem(_dest_item_,0,0));
        //qreal length = QLineF(mapFromItem(_src_item_,0,0),mapFromItem(_dest_item_,0,0)).length();
        //return std::hypot(delta.x(),delta.y());

        rect.setRect(   -m_side-DEF_OUTLINE,
                        -m_side-DEF_OUTLINE,
                        m_side*2+DEF_OUTLINE,
                        m_side*2+DEF_OUTLINE);
        polygon<<rect.bottomLeft()<<
                 QPointF(rect.center().x(),rect.topLeft().y())<<
                 rect.bottomRight()<<rect.bottomLeft();
        path.addPolygon(polygon);
    }
//    path.addEllipse(QPointF(0,0),
//                    m_rad+DEF_CONTROL_RADIUS,
//                    m_rad+DEF_CONTROL_RADIUS);
//    path.addEllipse(QPointF(0,0),
//                    m_rad,
//                    m_rad);
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing,true);
    painter->setPen(outlinePen);
    painter->setBrush(brush);
    painter->drawPath(path);
    painter->restore();
}

void ResizeItem::mouseMoveEvent(QGraphicsSceneMouseEvent* m_event)
{
    Q_UNUSED(m_event);
    QPointF l_point = m_event->lastScenePos();
    QPointF n_point = m_event->scenePos();
    QPointF main_center= _main_item_->sceneCenterPoint();
    QPointF new_delta(main_center-n_point);
    QPointF old_delta(main_center-l_point);
    qreal new_dist = std::hypot(new_delta.x(),new_delta.y());
    qreal old_dist = std::hypot(old_delta.x(),old_delta.y());
    qreal diff = new_dist - old_dist;
    _main_item_->moveRadius(diff);
    update();
    return;
}

void ResizeItem::setThickness(qreal val)
{
    prepareGeometryChange();
    _thickness_ = val;
    return;
}

void ResizeItem::setSize(qreal radius)
{
    _main_item_->setRadius(radius);
}
