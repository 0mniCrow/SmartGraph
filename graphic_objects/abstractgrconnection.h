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
    int                     _weight_;
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

    AbstractGrConnection(const item_id_t& id=item_id_t(),
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
    void setWeight(int weight);
    int getWeight() const noexcept;
    virtual char grObjectType() const noexcept override;

    virtual void recalculate() = 0;                                     //Для будучых дынамічных падзей
    virtual void drawGr() override;

};

#endif // ABSTRACTGRCONNECTION_H
