#ifndef STATICGRITEM_H
#define STATICGRITEM_H
#include "abstractgrqtitem.h"
#include "templateItemData.h"

using item_data_type = QString;

class QStringItemData:public ItemDataInterface<item_data_type>
{
private:
    item_data_type _data_;
public:
    virtual QString getGrData() const override {return _data_;};
    virtual void setGrData(const item_data_type& data,dataChangeType gr_type) override {
        if(gr_type ==DC_External)
            _data_=data;
        return;};
};


class StaticGrItem:public AbstractGrQtItem,public QStringItemData
{
private:
    QPixmap                             _orig_pixmap_;
    QPixmap                             _icon_;
    static const QString                _object_name_;
    void iconUpdate();
    void drawVertexCircle(QPainter* painter);
    void drawVertexIcon(QPainter* painter);
    void drawPinNeedle(QPainter* painter);
protected:
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;
    virtual void paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget) override;
public:
    enum GrItemForm{GR_ItemForm_Circle = 1, GR_ItemForm_Square, GR_ItemForm_Triangle};
    enum GrItemType{StaticItem = GR_STATIC_ITEM};
    enum {Type = UserType+GR_STATIC_ITEM};
    explicit StaticGrItem(const item_id_t& id=GR_ITEM_ID_DEF,
                          int radius = DEF_ITEM_RADIUS,
                          QGraphicsObject *tata = nullptr);
    int type() const override{return Type;}
    virtual QRectF boundingRect() const override;
    virtual QPainterPath shape() const override;
    void setImage(const QPixmap& image);
    QPixmap getImage() const { return _orig_pixmap_; }
    virtual void setRadius(int radius) override;
    virtual void drawGrObject() override;
    virtual void setGrData(const item_data_type& data, dataChangeType gr_type)override;
    virtual char grObjectType() const noexcept override;
    virtual QStringView getObjectName() const override;
};

#endif // STATICGRITEM_H
