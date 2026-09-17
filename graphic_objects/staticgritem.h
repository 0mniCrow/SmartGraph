#ifndef STATICGRITEM_H
#define STATICGRITEM_H
#include "abstractgrqtitem.h"
#include "templateItemData.h"

using item_data_type = QString;

class StaticGrItem:public AbstractGrQtItem,public ItemDataInterface<item_data_type>
{
private:
    item_data_type _data_;
public:
    enum GrItemType{StaticItem = GR_STATIC_ITEM};
    enum {Type = UserType+GR_STATIC_ITEM};
    explicit StaticGrItem(const item_id_t& id=GR_ITEM_ID_DEF,
                          int radius = DEF_ITEM_RADIUS,
                          const item_data_type& data = item_data_type(),
                          QGraphicsObject *tata = nullptr);
    virtual void setGrData(const item_data_type& data, dataChangeType gr_type) override final;
    virtual item_data_type getGrData() const override final;
    int type() const override{return Type;}
};

#endif // STATICGRITEM_H
