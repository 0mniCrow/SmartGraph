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
#include <QGridLayout>

namespace Ui {
class TouchForm;
}

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
    QLabel * generate_label(const QPainter::CompositionMode& mode);
    QList<QColor> _color_list_;
    QList<QTouchEvent::TouchPoint> _touchpoint_list_;
    Ui::TouchForm *ui;
    int _paint_state_;
protected:
    virtual void paintEvent(QPaintEvent* p_event) override;
    virtual bool event (QEvent * reg_event) override;
};

#endif // TOUCHFORM_H
