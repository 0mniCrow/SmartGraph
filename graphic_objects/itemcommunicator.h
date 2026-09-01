#ifndef ITEMCOMMUNICATOR_H
#define ITEMCOMMUNICATOR_H
#include <QObject>
#include <QPixmap>
#include <QTimer>

#include "widgets/gview_tooltip_window.h"
#include "widgets/gview_edit_window.h"
#include "imagecropwindow.h"

#define TIMER_DELAY 2000
#define DEFAULT_ARROW_SIZE 10

class GViewPort;
class AbstractGrItem;


class ItemCommunicator : public QObject
{
    Q_OBJECT
private:
    GViewPort*              _main_port_;
    unsigned long long      _cur_time_;
    QPixmap                 _def_image_;
    QTimer                  _tip_timer_;
    GViewToolTip *          _tooltip_window_;
    GViewEdit *             _edit_window_;
    ImageCropWindow *       _pic_load_dialog_;
    AbstractGrItem*         _cur_working_item_;
    QPoint                  _tip_pos_;
    qreal                   _arrow_size_;

public:
    explicit ItemCommunicator(GViewPort* port, QObject *parent = nullptr);
    void setCurTime(unsigned long long new_time);
    unsigned long long getCurTime() const noexcept;
    void startToolTipTimer(AbstractGrItem* gr_sender, const QPoint& pos);
    void stopToolTipTimer();
    void setDefImage(const QString& imgAddr);
    const QPixmap& getDefImage() const;
    void callEditWindow(AbstractGrItem* gr_sender, const QPoint& pos);
    void callToolTipWindow(AbstractGrItem* gr_sender, const QPoint& pos);
    void itemIsMoved();
    void setArrowSize(qreal arrow_size);
    qreal getArrowSize() const noexcept;
    void arrowSizeChanged();
    QGraphicsView * getMainPort() const noexcept;
private slots:
    void timeOut();
    void editWindowUpdated();
};

#endif // ITEMCOMMUNICATOR_H
