#include "imagecropwindow.h"
#include "ui_imagecropwindow.h"
#include "touchform.h"

ImageCropWindow::ImageCropWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageCropWindow)
{
    ui->setupUi(this);
    connect(ui->button_imgFileDialog,&QPushButton::clicked,this,&ImageCropWindow::chooseFile);
    connect(ui->button_loadImage,&QPushButton::clicked,this,&ImageCropWindow::loadImage);
    connect(ui->button_Crop,&QPushButton::clicked,this,&ImageCropWindow::cropImage);
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
//    QString file_addr(ui->line_imgAddr->text());
//    if(!QFileInfo::exists(file_addr))
//        return;
//    QImage bg(file_addr);
//    if(bg.isNull())
//        return;

    if(_item_)
    {
        _scene_->removeItem(_r_item_);
        _scene_->removeItem(_item_);
        delete _r_item_;
        delete _item_;
    }
    QPixmap bg(getIMG());
    if(bg.isNull())
    {
        return;
    }
    _scene_->loadPixmap(bg);
//    _scene_->loadPixmap(QPixmap::fromImage(bg));
    _item_ = new CropItem();
    _r_item_ = new ResizeItem(_item_);
    _scene_->addItem(_item_);
    _scene_->addItem(_r_item_);
    QRectF scene_r(_scene_->sceneRect());
    _item_->setPos(scene_r.center());
    //_r_item_->setPos(scene_r.center());

//    ui->label->setFixedSize(bg.size());
//    ui->label->setPixmap(QPixmap::fromImage(bg));
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
    painter.drawPixmap(-target.topLeft(),bg.copy(target.toRect()));
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

CropItem::CropItem(qreal radius):_radius_(radius)
{
    setFlags(ItemIsMovable|ItemSendsGeometryChanges);
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    return;
}
void CropItem::setRadius(qreal radius)
{
    prepareGeometryChange();
    _radius_ = radius;
    return;
}

void CropItem::moveRadius(qreal val)
{
    prepareGeometryChange();
    _radius_+=val;
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
    QRectF b_rect(-_radius_-DEF_OUTLINE,
                  -_radius_-DEF_OUTLINE,
                  _radius_*2+DEF_OUTLINE,
                  _radius_*2+DEF_OUTLINE);
    return b_rect;
}
QPainterPath CropItem::shape() const
{
    QPainterPath path;
    path.setFillRule(Qt::OddEvenFill);
    path.addEllipse(QPointF(0,0),
                    _radius_+DEF_OUTLINE,
                    _radius_+DEF_OUTLINE);
    path.addEllipse(QPointF(0,0),
                    _radius_-DEF_WIDTH,
                    _radius_-DEF_WIDTH);
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
    path.addEllipse(QPointF(0,0),_radius_,_radius_);
    path.addEllipse(QPointF(0,0),_radius_-DEF_WIDTH,_radius_-DEF_WIDTH);
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

ResizeItem::ResizeItem(CropItem* tata):QGraphicsItem(tata)
{
    _main_item_ = tata;
    setFlags(ItemIsMovable|ItemIgnoresParentOpacity);
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    return;
}


QRectF ResizeItem::boundingRect() const
{
    qreal m_rad = _main_item_->radius();
    QRectF rect(-m_rad-DEF_CONTROL_RADIUS-DEF_CONTROL_OUTLINE,
                  -m_rad-DEF_CONTROL_RADIUS-DEF_CONTROL_OUTLINE,
                  m_rad*2+DEF_CONTROL_RADIUS+DEF_OUTLINE*2,
                  m_rad*2+DEF_CONTROL_RADIUS+DEF_OUTLINE*2);
    return rect;
}
QPainterPath ResizeItem::shape() const
{
    QPainterPath path;
    qreal m_rad = _main_item_->radius();
    path.setFillRule(Qt::OddEvenFill);
    path.addEllipse(QPointF(0,0),
                    m_rad+DEF_CONTROL_RADIUS+DEF_CONTROL_OUTLINE,
                    m_rad+DEF_CONTROL_RADIUS+DEF_CONTROL_OUTLINE);
    path.addEllipse(QPointF(0,0),
                    m_rad,
                    m_rad);
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
    qreal m_rad = _main_item_->radius();
    QPainterPath path;
    path.setFillRule(Qt::OddEvenFill);
    path.addEllipse(QPointF(0,0),
                    m_rad+DEF_CONTROL_RADIUS,
                    m_rad+DEF_CONTROL_RADIUS);
    path.addEllipse(QPointF(0,0),
                    m_rad,
                    m_rad);
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

void ResizeItem::setSize(qreal radius)
{
    _main_item_->setRadius(radius);
}
