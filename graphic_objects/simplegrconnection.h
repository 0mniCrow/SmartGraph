#ifndef SIMPLEGRCONNECTION_H
#define SIMPLEGRCONNECTION_H

#include "abstractgrqtconnection.h"

#define CONNECTION_WIDTH 2.0

class SimpleGrConnection:public AbstractGrQtConnection
{
private:
    void getArrowSholders(double arr_atan, const QPointF& arr_head,
                          QPointF& first_sholder, QPointF& sec_sholder, bool dest_point = true);
protected:
    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget) override;
    virtual void redraw() override;
public:
    enum GraphicItemType{SimpleConnection = GR_SIMPLE_CONNECTION};
    enum {Type = UserType+GR_SIMPLE_CONNECTION};
    explicit SimpleGrConnection();
    SimpleGrConnection(AbstractGrItem* source,
                       AbstractGrItem* destination,
                       ItemCommunicator *communicator,
                       char mode, bool directed = false,
                       const item_id_t& id= GR_ITEM_ID_DEF,
                       qreal weight = ABSTRACT_EDGE_DEF_WEIGHT,
                       QGraphicsObject* tata = nullptr);
    virtual ~SimpleGrConnection() = default;
    virtual int type() const override;
    virtual QPainterPath shape() const override;
};

#endif // SIMPLEGRCONNECTION_H
