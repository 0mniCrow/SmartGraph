#ifndef STATICGRITEM_H
#define STATICGRITEM_H
#include "abstractGrItem.h"
#include "templateItemData.h"

typedef QString GrItemData;

class StaticGrItem:public AbstractGrItem, public ItemDataInterface<GrItemData>
{
    Q_OBJECT
private:
    GrItemData _data_;
public:
    explicit StaticGrItem(const item_id_t& gr_id=item_id_t(),
                 const GrItemData& gr_data = GrItemData(),
                 int gr_radius = DEF_ITEM_RADIUS,
                 QGraphicsItem* tata = nullptr);
};

#endif // STATICGRITEM_H
