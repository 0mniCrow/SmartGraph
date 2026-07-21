#ifndef ABSTRACTGRITEM_H
#define ABSTRACTGRITEM_H
#include "abstractGrInterface.h"
#include "QGraphicsObject"
#include "QTimer"

class AbstractGrItem:public AbstractGrInterface, public QGraphicsObject
{
private:
    QPointF                 _adv_pos_;
    QPoint                  _last_screen_pos_;
    QTimer                  _show_timer_;
    int                     _radius_;
    char                    _flags_;
public:
    AbstractGrItem(const item_id_t& id=item_id_t());
};

#endif // ABSTRACTGRITEM_H
