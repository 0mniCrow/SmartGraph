#include "imagecropwindow.h"
#include "ui_imagecropwindow.h"

ImageCropWindow::ImageCropWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageCropWindow)
{
    ui->setupUi(this);
    connect(ui->button_imgFileDialog,&QPushButton::clicked,this,&ImageCropWindow::chooseFile);
    connect(ui->button_loadImage,&QPushButton::clicked,this,&ImageCropWindow::loadImage);
    _scene_ = new CropScene(ui->graphicsView);
    ui->graphicsView->setScene(_scene_);
    _item_ = nullptr;
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

void ImageCropWindow::loadImage()
{
    QString file_addr(ui->line_imgAddr->text());
    if(!QFileInfo::exists(file_addr))
        return;
    QImage bg(file_addr);
    if(bg.isNull())
        return;
    if(_item_)
    {
        _scene_->removeItem(_item_);
        delete _item_;
    }
    _scene_->loadPixmap(QPixmap::fromImage(bg));
    _item_ = new CropItem();
    _scene_->addItem(_item_);
    _item_->setPos(_scene_->sceneRect().center());
//    ui->label->setFixedSize(bg.size());
//    ui->label->setPixmap(QPixmap::fromImage(bg));
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
    _radius_ = radius;
    return;
}
qreal CropItem::radius() const
{
    return _radius_;
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
    _radius_ = tata->radius();
    _resz_caller_ = false;
    setFlags(ItemIsMovable|ItemSendsGeometryChanges);
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    return;
}


QRectF ResizeItem::boundingRect() const
{
    QRectF res_rect(-DEF_CONTROL_RADIUS-DEF_CONTROL_OUTLINE,
                    -DEF_CONTROL_RADIUS-DEF_CONTROL_OUTLINE,
                    DEF_CONTROL_RADIUS+DEF_CONTROL_OUTLINE,
                    DEF_CONTROL_RADIUS+DEF_CONTROL_OUTLINE);
    return res_rect;
}
QPainterPath ResizeItem::shape() const
{
    QPainterPath res_path;
    res_path.addEllipse(QPointF(0,0),
                    DEF_CONTROL_RADIUS+DEF_CONTROL_OUTLINE,
                    DEF_CONTROL_RADIUS+DEF_CONTROL_OUTLINE);
    return res_path;
}
void ResizeItem::paint(QPainter* painter,
           const QStyleOptionGraphicsItem* option,
           QWidget* widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    painter->save();
    painter->setPen(QPen(Qt::black,DEF_CONTROL_OUTLINE));
    painter->setBrush(QBrush(Qt::white));
    painter->drawEllipse(QPointF(0,0),
                         DEF_CONTROL_RADIUS,
                         DEF_CONTROL_RADIUS);
    painter->restore();
}

void ResizeItem::mouseMoveEvent(QGraphicsSceneMouseEvent* m_event)
{
    Q_UNUSED(m_event);
//    QPointF tata_center = _main_item_->mapToScene(QPointF(0,0));
//    QPointF dzicja_center = mapToScene(QPoint(0,0));
//    QPointF l_point = m_event->lastScenePos();
//    QPointF n_point = m_event->scenePos();

    QPointF main_center = mapFromItem(_main_item_,QPointF(0,0));
    QPointF delta(QPointF(0,0)-main_center);
    qreal dist = std::hypot(delta.x(),delta.y());
    setSize(dist);
    m_event->accept();
    return;
}

void ResizeItem::setSize(qreal radius)
{
   _resz_caller_=true;
   _main_item_->setRadius(radius);
   _resz_caller_= false;
}
