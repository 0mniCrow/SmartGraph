#ifndef IMAGECROPWINDOW_H
#define IMAGECROPWINDOW_H
#define DEF_RADIUS 40.0
#define DEF_WIDTH 10.0
#define DEF_CONTROL_RADIUS 5
#define DEF_OUTLINE 2
#define DEF_CONTROL_OUTLINE 1
#include <QWidget>
#include <QFileDialog>
#include <QDir>
#include <QImage>
#include <QPainter>
#include <QFileInfo>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

class CropItem;

class ResizeItem:public QGraphicsItem
{
public:
    ResizeItem(CropItem* tata);
    void setSize(qreal radius);
private:
    CropItem* _main_item_;
protected:
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* m_event) override;
};

class CropItem:public QGraphicsItem
{
private:
    qreal _radius_;
public:
    CropItem(qreal radius = DEF_RADIUS);
    void setRadius(qreal radius);
    void moveRadius(qreal val);
    qreal radius() const;
    QPointF sceneCenterPoint() const;
protected:
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget) override;
    void mousePressEvent(QGraphicsSceneMouseEvent * m_event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * m_event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* m_event) override;
};

class CropScene: public QGraphicsScene
{
public:
    CropScene(QObject* tata=nullptr);
    void loadPixmap(const QPixmap& bg);
protected:
    void drawBackground(QPainter* painter, const QRectF & rect) override;
private:
    QPixmap _bg_;
};

namespace Ui {
class ImageCropWindow;
}

class ImageCropWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ImageCropWindow(QWidget *parent = nullptr);
    ~ImageCropWindow();

private:
    Ui::ImageCropWindow *ui;
    CropScene* _scene_;
    CropItem* _item_;
    ResizeItem* _r_item_;
private slots:
    void chooseFile();
    void loadImage();
};

#endif // IMAGECROPWINDOW_H
