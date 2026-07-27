#ifndef ABSTRACTGRCONNECTION_H
#define ABSTRACTGRCONNECTION_H
#include "abstractGrInterface.h"
#include <QGraphicsObject>
#include <QPointF>

class AbstractGrItem;

class AbstractGrConnection:public AbstractGrInterface, public QGraphicsObject
{
    Q_OBJECT
private:
    QPointF _src_point_;
    QPointF _dest_point_;
    AbstractGrItem* _src_item_;
    AbstractGrItem* _dest_item_;
    int _vertex_radius_;
    bool _directed_;
    qreal _arrowSize_ = 10;
    qreal _weight_;
public:
    AbstractGrConnection(QGraphicsItem* tata = nullptr);
    void recalculate();
    AbstractGrItem* source()const noexcept{return _src_item_;}
    AbstractGrItem* destination() const noexcept{return _dest_item_;}
    void setSource(AbstractGrItem* src);
    void setDestination(AbstractGrItem* dest);
    qreal grWeight() const noexcept{return _weight_;}
    void setGrWeight(qreal weight) noexcept {_weight_ = weight;}
};

#endif // ABSTRACTGRCONNECTION_H
