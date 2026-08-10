#ifndef SIMPLEGRCONNECTION_H
#define SIMPLEGRCONNECTION_H

#include "abstractgrconnection.h"

class SimpleGrConnection:public AbstractGrConnection
{
    Q_OBJECT
private:
    QPointF _src_point_;
    QPointF _dest_point_;
    unsigned int _weight_;
    bool isOperable() const noexcept;
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
    SimpleGrConnection(AbstractGrItem* source, AbstractGrItem* destination,
                       ItemCommunicator* communicator,
                       const item_id_t& id=item_id_t(),
                       bool directed = false,
                       QGraphicsObject* tata = nullptr);
    SimpleGrConnection(AbstractGrItem* source, char mode,
                       ItemCommunicator *communicator,
                       bool directed = false,
                       const item_id_t& id=item_id_t(),
                       QGraphicsObject* tata = nullptr);
    ~SimpleGrConnection() = default;

    virtual int type() const override;
    virtual char grConnectionType() const noexcept override;

    virtual QPainterPath shape() const override;

    virtual void recalculate() override;
    virtual void setStartPoint(coord_real x, coord_real y) override;
    virtual void setEndPoint(coord_real x, coord_real y) override;
    virtual coord_real getStartX() const override;
    virtual coord_real getStartY() const override;
    virtual coord_real getFinX() const override;
    virtual coord_real getFinY() const override;

};

#endif // SIMPLEGRCONNECTION_H
