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
class ShadowItem;

class ResizeItem:public QGraphicsItem
{
public:
    ResizeItem(CropItem* tata, qreal thickness = DEF_CONTROL_RADIUS);
    void setSize(qreal radius);
    void setShadowItem(ShadowItem* sh_item);
    void setThickness(qreal val);
    qreal radius() const;
    char mainGeomType() const;
private:
    CropItem* _main_item_;
    ShadowItem* _shadow_;
    qreal _thickness_;
protected:
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* m_event) override;
};

class ShadowItem:public QGraphicsItem
{
private:
    ResizeItem* _res_item_;
public:
    ShadowItem(ResizeItem* resize_item);
protected:
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget) override;

};

class CropItem:public QGraphicsObject
{
    Q_OBJECT
private:
    qreal _radius_;
    qreal _thickness_;
    char _geom_type_;
    void checkBorders();
    bool checkNewRadius(qreal n_radius);
public:
    enum Geometry{CI_INVALID = 0, CI_CIRCLE = 1,
                  CI_TRIANGLE = 2, CI_SQUARE = 3};
    CropItem(qreal radius = DEF_RADIUS,qreal thickness = DEF_WIDTH);
    void setRadius(qreal radius, bool manual_set=false);
    void moveRadius(qreal val);
    char geometryType() const {return _geom_type_;}
    void setGeometryType(char g_type);
    void setThickness(qreal val);
    qreal thickness()const{return _thickness_;}
    qreal radius() const;
    QPointF sceneCenterPoint() const;
signals:
    void sendNewRadius(qreal new_radius);
protected:
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;
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
    ShadowItem* _s_item_;
    QPixmap getIMG();
private slots:
    void chooseFile();
    void loadImage();
    void cropImage();
    void radiusChanged(int radius);
    void nonManualRadChanged(qreal radius);
    void thicknessChanged(int val);
    void resThicknessChanged(int val);
    void geometryChange(int geometry);
};

#endif // IMAGECROPWINDOW_H
