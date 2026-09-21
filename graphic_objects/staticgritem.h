#ifndef STATICGRITEM_H
#define STATICGRITEM_H
#include "abstractgrqtitem.h"
#include "templateItemData.h"

using item_data_type = QString;

class QStringItemData:public ItemDataInterface<QString>
{
private:
    QString _data_;
public:
    virtual QString getGrData() const override final{return _data_;};
    virtual void setGrData(const QString& data,dataChangeType gr_type) override final{
        if(gr_type ==DC_External)
            _data_=data;
        return;};
};


class StaticGrItem:public AbstractGrQtItem,public QStringItemData
{
private:
//    item_data_type                      _data_;
    QPixmap                             _orig_pixmap_;
    QPixmap                             _icon_;
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
    enum GrItemType{StaticItem = GR_STATIC_ITEM};
    enum {Type = UserType+GR_STATIC_ITEM};
    explicit StaticGrItem(const item_id_t& id=GR_ITEM_ID_DEF,
                          int radius = DEF_ITEM_RADIUS,
                          QGraphicsObject *tata = nullptr);
//    virtual void setGrData(const item_data_type& data, dataChangeType gr_type) override final;
//    virtual item_data_type getGrData() const override final;
    int type() const override{return Type;}
    virtual QRectF boundingRect() const override;
    virtual QPainterPath shape() const override;
};

#endif // STATICGRITEM_H
