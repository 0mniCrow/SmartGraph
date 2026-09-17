#include "staticgritem.h"

StaticGrItem::StaticGrItem(const item_id_t& id,
                           int radius,
                           const item_data_type &data,
                           QGraphicsObject* tata):
    AbstractGrQtItem(id,radius,tata),_data_(data)
{
    return;
}

void StaticGrItem::setGrData(const item_data_type& data, dataChangeType gr_type)
{
    _data_ = data;
//    switch(gr_type)
//    {
//    case DC_External:
//    {
//        emit changedExternally(data);
//    }
//        break;
//    case DC_Internal:
//    {
//        emit changedInternally(this);
//    }
//        break;
//    }
    return;
}

item_data_type StaticGrItem::getGrData() const
{
    return _data_;
}
