#ifndef TOUCHFORM_H
#define TOUCHFORM_H

#include <QWidget>
#include <QList>
#include <QColor>
#include <QTouchEvent>
#include <QPainter>
#include <QPicture>
#include <QPainterPath>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QKeyEvent>
#include <QGraphicsBlurEffect>
#include <QGraphicsDropShadowEffect>
#include <QGraphicsColorizeEffect>
#include <QGraphicsOpacityEffect>
#include <QFormLayout>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QMovie>
#include <QTimer>
//#include "graphwidget.h"

namespace Ui {
class TouchForm;
}
/*
 * Адсякаючыя рэчы павінны рухацца ў залежнасьці ад таго,
 * што карыстальнік націснуў. Калі ён націснуў на знешнюю рыску
 * змяняцца памер. Калі заціснуў асноўную рэч, кола цягаецца пад
 * курсорам.
 * Ці патрэбны дадатковы клас рэч для відарыса на фоне?
*/

class CropOutline:public QGraphicsItem
{

};

class CropItem:public QGraphicsItem
{
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
    void hoverEnterEvent(QGraphicsSceneHoverEvent * h_event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent * h_event) override;
};

class CropView:public QGraphicsView
{
    Q_OBJECT
protected:
    void mousePressEvent(QMouseEvent* m_event) override;
    void mouseReleaseEvent(QMouseEvent* m_event) override;
    void mouseMoveEvent(QMouseEvent* m_event) override;
};

class LocWidget:public QLabel
{
private:
    QPoint _pos_;
protected:
    virtual void mousePressEvent(QMouseEvent* m_event) override;
    virtual void mouseMoveEvent(QMouseEvent* m_event) override;
public:
    LocWidget(QWidget* tata = nullptr);
};

class ElidedText: public QWidget
{
protected:
    virtual void paintEvent(QPaintEvent* p_event);
public:
    ElidedText(QWidget* tata = nullptr);
};

class TouchForm : public QWidget
{
    Q_OBJECT

public:
    explicit TouchForm(QWidget *parent = nullptr);
    ~TouchForm();
    enum Position{PNone = 0, PLeftTop = 1, PMidTop = 2,
                 PRightTop = 3, PMidLeft = 4, PMidRight = 5,
                 PLeftDown = 6, PMidDown = 7, PRightDown = 8,
                 PInside = 9};
    enum linetype{PLine = 1,PBezier = 2};
public slots:
    void paintState(int state);
    void wgt_show();
private:
    QWidget wgt;
    QWidget wgt2;
    QWidget wgt3;
    LocWidget wgt4;
    ElidedText wgt5;
    QTimer timer;
    QLabel * generate_label(const QPainter::CompositionMode& mode);
    QLabel * generate_label2(QGraphicsEffect* graphic_effect);
    QImage brightness(const QImage& origImg, int brght);
    QList<QColor> _color_list_;
    QList<QTouchEvent::TouchPoint> _touchpoint_list_;
    Ui::TouchForm *ui;
    int _paint_state_;
    int cur_x;
    int cur_y;
    int cur_x_size;
    int cur_y_size;
    uchar chPos(const QPointF& point, const QRectF& rect) const;
    uchar chLine(uchar start_pos, uchar fin_pos) const;
    void setShiftPoints(QPointF& startShift, QPointF& finShift,
                        const QPointF& start, const QPointF& finish,
                        uchar start_pos,uchar fin_pos, const QRectF& rect) const;
protected:
    virtual void paintEvent(QPaintEvent* p_event) override;
    virtual bool event (QEvent * reg_event) override;
    virtual void keyPressEvent(QKeyEvent* key_event)override;
    virtual void closeEvent(QCloseEvent*  cl_event) override;
};

#endif // TOUCHFORM_H
