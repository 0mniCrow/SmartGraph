#ifndef ABSTRACTGRCONNECTION_H
#define ABSTRACTGRCONNECTION_H
#include "abstractGrInterface.h"
#include <QGraphicsObject>
#include <QPointF>

#define ABSTRACT_EDGE_WIDTH 2.0

class AbstractGrItem;

class AbstractGrConnection:public QGraphicsObject, public AbstractGrInterface
{
    Q_OBJECT
private:
    QPointF _src_point_;
    QPointF _dest_point_;
    AbstractGrItem* _src_item_;
    AbstractGrItem* _dest_item_;
    //int _vertex_radius_;
    bool _directed_;
    qreal _arrowSize_ = 10;
    qreal _weight_;
    char _mode_;
protected:
    QRectF boundingRect() const override;
    void paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget) override;
public:
    enum GraphicItemType{AbstractConnection = 3};
    enum ConnectionMode{GrEdge_Null, GrEdge_regular, GrEdge_incomplete, GrEdge_deletion};
    AbstractGrConnection(const item_id_t& id=item_id_t(),
                         bool directed = false,
                         QGraphicsObject* tata = nullptr);
    virtual ~AbstractGrConnection() = default;
    void recalculate();
    AbstractGrItem* source() const noexcept{return _src_item_;}
    AbstractGrItem* destination() const noexcept{return _dest_item_;}
    void setSource(AbstractGrItem* src);
    void setDestination(AbstractGrItem* dest);
    qreal grWeight() const noexcept{return _weight_;}
    void setGrWeight(qreal weight) noexcept {_weight_ = weight;}


    virtual void setGrX(coord_real x) override;
    virtual void setGrY(coord_real y) override;
    virtual coord_real getGrX() const override;
    virtual coord_real getGrY() const override;
    virtual void setGrWidth(coord_real width) override;
    virtual void setGrHeight(coord_real height) override;
    virtual coord_real getGrWidth() const override;
    virtual coord_real getGrHeight() const override;
    virtual void moveGr(coord_real x, coord_real y) override;
    virtual void drawGr() override;
    virtual char graphicType() const noexcept override{return AbstractConnection;}
};

#endif // ABSTRACTGRCONNECTION_H
