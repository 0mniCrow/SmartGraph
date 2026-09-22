#ifndef ABSTRACTGRCONNECTION_H
#define ABSTRACTGRCONNECTION_H
#include "abstractGrInterface.h"
#include <QPointF>

#define ABSTRACT_EDGE_WIDTH 2.0
#define ABSTRACT_EDGE_DEF_WEIGHT 10


class AbstractGrItem;

class AbstractGrConnection:public AbstractGrInterface
{
private:
    AbstractGrItem*         _src_item_;
    AbstractGrItem*         _dest_item_;
    bool                    _directed_;
    char                    _mode_;
    qreal                   _weight_;
    void setBasicSource(AbstractGrItem* source);
    void setBasicDestination(AbstractGrItem* destination);
protected:
    virtual void redraw() = 0;
    //Праверка, ці ўсталяваны ўсе умовы згодна з рэжымам(mode)
    virtual bool checkStatus() const;
public:
    enum GrObjectType{AbstractConnection = GR_ABSTRACT_CONNECTION};
    enum ConnectionMode{GrEdge_Null=0,
                        GrEdge_regular,
                        GrEdge_incomplete,
                        GrEdge_deletion,
                        GrEdge_userMode};

    explicit AbstractGrConnection(const item_id_t& id=GR_ITEM_ID_DEF,
                                  bool directed = false,
                                  qreal weight = ABSTRACT_EDGE_DEF_WEIGHT);
    AbstractGrConnection(AbstractGrItem* source,
                         AbstractGrItem* destination,
                         char mode, bool directed = false,
                         const item_id_t& id= GR_ITEM_ID_DEF,
                         qreal weight = ABSTRACT_EDGE_DEF_WEIGHT);
    virtual ~AbstractGrConnection() = default;
    AbstractGrItem* getSource() const noexcept;
    AbstractGrItem* getDestination() const noexcept;
    virtual void setSource(AbstractGrItem* src);
    virtual void setDestination(AbstractGrItem* dest);
    void setDirected(bool state);
    bool isDirected() const noexcept;
    virtual void setMode(char mode);
    virtual char getMode() const;
    virtual void setLengthAsWeigth();
    void setWeight(qreal weight);
    qreal getWeight() const noexcept;

    /*
     * recalcEndpoints функцыя удакладняе сапраўдныя пазіцыі
     * пачатку і канчатку лініі рэбра.
     */
    virtual void recalcEndpoints() = 0;

    //getLength функцыя вяртае !сапраўдную даўжыню рэбра.
    virtual qreal getLength() const = 0;
    //getGrStartX вяртае каардынату Х !сапраўднага пачатку рэбра.
    virtual qreal getGrStartX() const = 0;
    //getGrEndX вяртае каардынату Х !сапраўднага канчатку рэбра.
    virtual qreal getGrEndX() const = 0;
    //getGrStartY вяртае каардынату У !сапраўднага пачатку рэбра.
    virtual qreal getGrStartY() const = 0;
    //getGrEndY вяртае каардынату У !сапраўднага канчатку рэбра.
    virtual qreal getGrEndY() const = 0;
    //getGrStartCoords вяртае ў дасланых пераменных каардынаты сапраўднага пачатку рэбра.
    virtual void getGrStartCoords(qreal& x, qreal& y) const = 0;
    //getGrEndCoords вяртае ў дасланых пераменных каардынаты сапраўднага канчатку рэбра.
    virtual void getGrEndCoords(qreal& x, qreal& y) const = 0;
    virtual void drawGrObject() override;

};

#endif // ABSTRACTGRCONNECTION_H
