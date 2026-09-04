#include "abstractgrqtitem.h"

AbstractGrQtItem::AbstractGrQtItem(const item_id_t &id,
                                   int radius,
                                   QGraphicsObject *tata):
    QGraphicsObject(tata),
    AbstractGrItem(id),
    _radius_(radius)
{

}
