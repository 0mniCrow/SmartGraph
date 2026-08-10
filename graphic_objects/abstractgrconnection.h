#ifndef ABSTRACTGRCONNECTION_H
#define ABSTRACTGRCONNECTION_H
#include "abstractGrInterface.h"
#include "itemcommunicator.h"
#include <QGraphicsObject>
#include <QPointF>

#define ABSTRACT_EDGE_WIDTH 2.0


class AbstractGrItem;

class AbstractGrConnection:public QGraphicsObject, public AbstractGrInterface
{
    Q_OBJECT
private:
    AbstractGrItem*         _src_item_;
    AbstractGrItem*         _dest_item_;
    ItemCommunicator*       _communicator_;
    bool                    _directed_;
    char                    _mode_;
protected:
    virtual QRectF boundingRect() const override = 0;                           //!virtual
    virtual void paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget) override = 0;                                   //!virtual
    virtual void redraw() = 0;                                                  //!virtual
public:
    enum GrObjectType{AbstractConnection = GR_ABSTRACT_CONNECTION};
    enum ConnectionMode{GrEdge_Null=0,
                        GrEdge_regular,
                        GrEdge_incomplete,
                        GrEdge_deletion,
                        GrEdge_userMode};
    enum {Type = UserType+GR_ABSTRACT_CONNECTION};                          //Для QtGraphics завочнай трансфармацыі

    AbstractGrConnection(const item_id_t& id=item_id_t(),
                         bool directed = false,
                         QGraphicsObject* tata = nullptr);
    virtual ~AbstractGrConnection() = default;

    void setArrowSize(qreal ar_size);
    qreal getArrowSize() const;
    AbstractGrItem* getSource() const noexcept;
    AbstractGrItem* getDestination() const noexcept;
    void setSource(AbstractGrItem* src);
    void setDestination(AbstractGrItem* dest);
    void setDirected(bool state);
    bool isDirected() const noexcept;
    void setMode(char mode);
    char getMode() const noexcept;
    void setCommunicator(ItemCommunicator* communicator);
    bool hasSourceItem() const noexcept;
    bool hasMainItems() const noexcept;
    virtual int type() const override;
    virtual char grObjectType() const noexcept final override;
    virtual char grConnectionType() const noexcept;


    virtual QPainterPath shape() const override = 0;                            //!virtual

    virtual void recalculate() = 0;                                             //!virtual
    virtual void setStartPoint(coord_real x, coord_real y) = 0;                 //!virtual
    virtual void setEndPoint(coord_real x, coord_real y) = 0;                   //!virtual
    virtual coord_real getStartX() const = 0;                                   //!virtual
    virtual coord_real getStartY() const = 0;                                   //!virtual
    virtual coord_real getFinX() const = 0;                                     //!virtual
    virtual coord_real getFinY() const = 0;

    virtual void setGrX(coord_real x) override final;
    virtual void setGrY(coord_real y) override final;
    virtual coord_real getGrX() const override final;
    virtual coord_real getGrY() const override final;
    virtual void setGrWidth(coord_real width) override final;
    virtual void setGrHeight(coord_real height) override final;
    virtual coord_real getGrWidth() const override final;
    virtual coord_real getGrHeight() const override final;
    virtual void moveGr(coord_real x, coord_real y) override;
    virtual void drawGr() override;

};

#endif // ABSTRACTGRCONNECTION_H
