#ifndef GVIEW_TIME_SLIDER_H
#define GVIEW_TIME_SLIDER_H

#include <QSlider>
#include <QStyleOptionSlider>
#include <QStyle>
#include <QString>
#include <QPainter>
#include <QPaintEvent>
#include <QList>

class TimeSlider: public QSlider
{
    Q_OBJECT
private:
    QStringList _text_;
    int areLabelsAdjusted(const QList<QPair<int, int>>& text_metrix) const;
    QStringList adjustLabels(int step_x, const QFontMetrics& f_metrix) const;
    void collectPositions(QList<QPair<int,int>>& container, const QStringList& list,
                          int step_x, const QFontMetrics& f_metrix) const;
public:

    explicit TimeSlider(Qt::Orientation orientation = Qt::Horizontal,
                        QWidget*parent = nullptr):QSlider(orientation,parent)
    {
        return;
    }
    void loadTextLabels(QStringList &list);
protected:
    void paintEvent(QPaintEvent* p_event) override;
    QSize sizeHint() const override;
    void mousePressEvent(QMouseEvent* m_event) override;
};

#endif // GVIEW_TIME_SLIDER_H
