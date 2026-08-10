#ifndef STATICGRITEM_H
#define STATICGRITEM_H
#include "abstractGrItem.h"

class StaticGrItem:public AbstractGrItem
{
    Q_OBJECT
private:
    GrItemData _data_;
public:
    enum GrItemType{StaticItem = GR_STATIC_ITEM};
    explicit StaticGrItem(const item_id_t& gr_id=item_id_t(),
                 const GrItemData& gr_data = GrItemData(),
                 int gr_radius = DEF_ITEM_RADIUS,
                 QGraphicsObject *tata = nullptr);
    virtual void setGrData(const GrItemData& data, dataChangeType gr_type) override final;
    virtual GrItemData getGrData() const override final;
    virtual char grItemType() const noexcept override{return StaticItem;}
};

#endif // STATICGRITEM_H
