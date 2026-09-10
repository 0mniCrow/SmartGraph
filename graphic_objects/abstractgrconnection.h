#ifndef ABSTRACTGRCONNECTION_H
#define ABSTRACTGRCONNECTION_H
#include "abstractGrInterface.h"
#include <QPointF>

#define ABSTRACT_EDGE_WIDTH 2.0


class AbstractGrItem;

class AbstractGrConnection:public AbstractGrInterface
{
private:
    AbstractGrItem*         _src_item_;
    AbstractGrItem*         _dest_item_;
    bool                    _directed_;
    char                    _mode_;
    qreal                   _weight_;
protected:
    virtual void redraw() = 0;
    virtual bool checkStatus();                                         //Праверка, ці ўсталяваны ўсе умовы згодна з рэжымам(mode)
public:
    enum GrObjectType{AbstractConnection = GR_ABSTRACT_CONNECTION};
    enum ConnectionMode{GrEdge_Null=0,
                        GrEdge_regular,
                        GrEdge_incomplete,
                        GrEdge_deletion,
                        GrEdge_userMode};

    explicit AbstractGrConnection(const item_id_t& id=GR_ITEM_ID_DEF,
                                  bool directed = false);
    virtual ~AbstractGrConnection() = default;
    AbstractGrItem* getSource() const noexcept;
    AbstractGrItem* getDestination() const noexcept;
    void setSource(AbstractGrItem* src);
    void setDestination(AbstractGrItem* dest);
    void setDirected(bool state);
    bool isDirected() const noexcept;
    virtual void setMode(char mode);
    virtual char getMode() const;
    virtual void setLengthAsWeigth();
    void setWeight(qreal weight);
    qreal getWeight() const noexcept;
    virtual char grObjectType() const noexcept override;

    virtual void recalculate() const = 0;                                     //Для будучых дынамічных падзей
    virtual qreal getLength() const = 0;
    virtual qreal getGrStartX() const = 0;
    virtual qreal getGrEndX() const = 0;
    virtual qreal getGrStartY() const = 0;
    virtual qreal getGrEndY() const = 0;
    virtual void getGrStartCoords(qreal& x, qreal& y) const = 0;
    virtual void getGrEndCoords(qreal& x, qreal& y) const = 0;
    virtual void drawGr() override;

};

#endif // ABSTRACTGRCONNECTION_H
