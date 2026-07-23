#ifndef ITEMCOMMUNICATOR_H
#define ITEMCOMMUNICATOR_H
#include <QObject>
#include <QPixmap>
#include <QTimer>
#include "gviewport.h"
#include "widgets/gview_tooltip_window.h"
#include "widgets/gview_edit_window.h"
#include "imagecropwindow.h"


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
public:
    explicit ItemCommunicator(QObject *parent = nullptr);
    void setCurTime();
    void getCurTime();
    void startToolTipTimer(AbstractGrItem* gr_sender);
    void stopToolTipTimer();
    void setDefImage(const QString& imgAddr);
    const QPixmap& getDefImage() const;
    void callEditWindow(AbstractGrItem* gr_sender);
    void itemIsMoved();

signals:
    void timeOut();
};

#endif // ITEMCOMMUNICATOR_H
