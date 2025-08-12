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

namespace Ui {
class TouchForm;
}


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
public slots:
    void paintState(int state);
    void wgt_show();
private:
    QWidget wgt;
    QWidget wgt2;
    QWidget wgt3;
    LocWidget wgt4;
    ElidedText wgt5;
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
protected:
    virtual void paintEvent(QPaintEvent* p_event) override;
    virtual bool event (QEvent * reg_event) override;
    virtual void keyPressEvent(QKeyEvent* key_event)override;
    virtual void closeEvent(QCloseEvent*  cl_event) override;
};

#endif // TOUCHFORM_H
