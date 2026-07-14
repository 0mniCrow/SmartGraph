#ifndef GVIEW_TIME_SLIDER_H
#define GVIEW_TIME_SLIDER_H

#include <QSlider>
#include <QStyleOptionSlider>
#include <QStyle>
#include <QString>
#include <QPainter>
#include <QPaintEvent>
#include <QWheelEvent>
#include <QList>

#define WHEEL_STEP 5

/*
 * Спадкавы клас QSlider з асабістым метадам малявання рысак і надпісаў.
 * Дадаткова клас захоўвае "сапраўдныя" найвышэйшую і найменьшую вялічыні стрыжня слайдэра.
*/

class TimeSlider: public QSlider
{
    Q_OBJECT
private:
    QStringList _text_;
    QStringList _adj_labels_;
    mutable QVector<int> _positions_;                       //Пазіцыі "рысак" слайдэра для вызначэння бліжэйшай пазіцыі ў mousePressEvent
    mutable bool _pos_recalc_needed_;
    int _min_val_;
    int _max_val_;
    int areLabelsAdjusted(const QList<QPair<int, int>>& text_metrix) const;
    QStringList adjustLabels(int step_x, const QFontMetrics& f_metrix) const;
    void collectPositions(QList<QPair<int,int>>& container, const QStringList& list,
                          int step_x, const QFontMetrics& f_metrix) const;
    void alternativeCalculation(QPainter * painter, const QRect &groove_rect, const QRect &handle_rect, QStyleOptionSlider &s_option);
    void drawLabels(QPainter* painter, const QRect& groove_rect);
    void drawSlider(QPainter* painter, QStyleOptionSlider& s_option,
                    const QRect &groove_rect, const QRect& handle_rect);
    void invalidateLabelPos() {_pos_recalc_needed_=true; update();};
    void recalculateLabelPos();
public:

    explicit TimeSlider(Qt::Orientation orientation = Qt::Horizontal,
                        QWidget*parent = nullptr):QSlider(orientation,parent),
        _pos_recalc_needed_(true),_min_val_(0),_max_val_(0)
    {
        connect(this,&QAbstractSlider::rangeChanged,this,[this](int,int){invalidateLabelPos();});
        return;
    }
    void loadTextLabels(const QStringList &list);
    void setMinVal(int min_val) noexcept{_min_val_=min_val;}
    int minVal() const noexcept{return _min_val_;}
    void setMaxVal(int max_val) noexcept{_max_val_=max_val;}
    int maxVal() const noexcept{return _max_val_;}

protected:
    void paintEvent(QPaintEvent* p_event) override;
    QSize sizeHint() const override;
    void mousePressEvent(QMouseEvent* m_event) override;
    void wheelEvent(QWheelEvent* w_event) override;
    void changeEvent(QEvent* c_event) override;
    void resizeEvent(QResizeEvent* r_event) override;

};

#endif // GVIEW_TIME_SLIDER_H
