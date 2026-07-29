#include "staticgritem.h"

StaticGrItem::StaticGrItem(const item_id_t& gr_id,
                           const GrItemData& gr_data,
                           int gr_radius,
                           QGraphicsItem* tata):
    AbstractGrItem(gr_id,gr_radius,tata),_data_(gr_data)
{

}
