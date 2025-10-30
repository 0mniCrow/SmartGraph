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
    void setThickness(qreal val);
private:
    CropItem* _main_item_;
    qreal _thickness_;
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
    qreal _thickness_;
    char _geom_type_;
public:
    enum Geometry{CI_INVALID = 0, CI_CIRCLE = 1,
                  CI_TRIANGLE = 2, CI_SQUARE = 3};
    CropItem(qreal radius = DEF_RADIUS,qreal thickness = DEF_WIDTH);
    void setRadius(qreal radius);
    void moveRadius(qreal val);
    char geometryType() const {return _geom_type_;}
    void setGeometryType(char g_type);
    void setThickness(qreal val);
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
    QPixmap getIMG();
private slots:
    void chooseFile();
    void loadImage();
    void cropImage();
    void radiusChanged(int radius);
    void thicknessChanged(int val);
    void geometryChange(int geometry);
};

#endif // IMAGECROPWINDOW_H
