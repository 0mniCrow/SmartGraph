#ifndef TOUCHFORM_H
#define TOUCHFORM_H

#include <QWidget>
#include <QList>
#include <QColor>
#include <QTouchEvent>
#include <QPainter>

namespace Ui {
class TouchForm;
}

class TouchForm : public QWidget
{
    Q_OBJECT

public:
    explicit TouchForm(QWidget *parent = nullptr);
    ~TouchForm();

private:
    QList<QColor> _color_list_;
    QList<QTouchEvent::TouchPoint> _touchpoint_list_;
    Ui::TouchForm *ui;
protected:
    virtual void paintEvent(QPaintEvent* p_event) override;
    virtual bool event (QEvent * reg_event) override;
};

#endif // TOUCHFORM_H
