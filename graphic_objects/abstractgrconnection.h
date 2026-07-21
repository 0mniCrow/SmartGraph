#ifndef ABSTRACTGRCONNECTION_H
#define ABSTRACTGRCONNECTION_H
#include "abstractGrInterface.h"
#include "abstractGrItem.h"

class AbstractGrConnection:public AbstractGrInterface, public QGraphicsObject
{
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
    AbstractGrConnection();
};

#endif // ABSTRACTGRCONNECTION_H
