#ifndef ABSTRACTGRITEM_H
#define ABSTRACTGRITEM_H
#include "abstractGrInterface.h"
#include "templateItemData.h"
#include <QVector>
#include <QString>

class AbstractGrConnection;

class AbstractGrItem: public AbstractGrInterface
{
protected:
    QVector<AbstractGrConnection*>      _edges_;
public:
    enum GrObjectType{AbstractItem = GR_ABSTRACT_ITEM};
    explicit AbstractGrItem(const item_id_t& id=GR_ITEM_ID_DEF);
    virtual ~AbstractGrItem() = default;
    bool addEdge(AbstractGrConnection* edge);
    void delEdge(AbstractGrConnection* edge);
    QList<AbstractGrConnection*> getEdges() const;

    virtual void setGrX(coord_real x) =0;
    virtual void setGrY(coord_real y) =0;
    virtual coord_real getGrX() const =0;
    virtual coord_real getGrY() const =0;
    virtual void setGrWidth(coord_real width) =0;
    virtual void setGrHeight(coord_real height) =0;
    virtual coord_real getGrWidth() const =0;
    virtual coord_real getGrHeight() const =0;
    virtual void moveGr(coord_real x, coord_real y) =0;
    virtual char grObjectType() const noexcept override{return AbstractItem;}

};

#endif // ABSTRACTGRITEM_H
