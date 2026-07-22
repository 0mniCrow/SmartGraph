#include "abstractGrItem.h"
#include "graphic_objects/abstractgrconnection.h"
AbstractGrItem::AbstractGrItem(const item_id_t &id,
                               int radius,
                               QGraphicsItem *tata):QGraphicsObject(tata),
    _radius_(radius)
{

}
