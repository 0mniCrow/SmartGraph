#include "staticgritem.h"

StaticGrItem::StaticGrItem(const item_id_t& gr_id,
                           const GrItemData& gr_data,
                           int gr_radius,
                           QGraphicsObject* tata):
    AbstractGrItem(gr_id,gr_radius,tata),_data_(gr_data)
{
    return;
}

void StaticGrItem::setGrData(const GrItemData& data, dataChangeType gr_type)
{
    _data_ = data;
    switch(gr_type)
    {
    case AbstractGrItem::DC_External:
    {
        emit changedExternally(data);
    }
        break;
    case AbstractGrItem::DC_Internal:
    {
        emit changedInternally(this);
    }
        break;
    }
    return;
}

GrItemData StaticGrItem::getGrData() const
{
    return _data_;
}
