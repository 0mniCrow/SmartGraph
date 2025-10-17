#include "imagecropwindow.h"
#include "ui_imagecropwindow.h"

ImageCropWindow::ImageCropWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageCropWindow)
{
    ui->setupUi(this);
    connect(ui->button_imgFileDialog,&QPushButton::clicked,this,&ImageCropWindow::chooseFile);
    connect(ui->button_loadImage,&QPushButton::clicked,this,&ImageCropWindow::loadImage);
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
    ui->label->setFixedSize(bg.size());
    ui->label->setPixmap(QPixmap::fromImage(bg));
    return;
}


CropItem::CropItem(qreal radius):_radius_(radius)
{
    setFlags(ItemIsMovable|ItemSendsGeometryChanges);
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

//void CropItem::paint(QPainter* painter,
//           const QStyleOptionGraphicsItem* option,
//           QWidget* widget) override;
//void CropItem::mousePressEvent(QGraphicsSceneMouseEvent * m_event) override;
//void CropItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * m_event) override;
//void CropItem::mouseMoveEvent(QGraphicsSceneMouseEvent* m_event) override;
