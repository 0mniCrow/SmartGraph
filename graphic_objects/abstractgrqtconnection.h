#ifndef ABSTRACTGRQTCONNECTION_H
#define ABSTRACTGRQTCONNECTION_H
#include "abstractgrconnection.h"
#include "itemcommunicator.h"
#include <QGraphicsObject>

class AbstractGrQtConnection:public QGraphicsObject, public AbstractGrConnection
{
    Q_OBJECT
private:
    ItemCommunicator*       _communicator_;
protected:
    virtual QRectF boundingRect() const override = 0;
    virtual void paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget) override = 0;
public:
    AbstractGrQtConnection(const item_id_t& id=item_id_t(),
                           bool directed = false,
                           QGraphicsObject* tata = nullptr);
    virtual ~AbstractGrQtConnection() = default;
    void setArrowSize(qreal ar_size);
    qreal getArrowSize() const;
    void setCommunicator(ItemCommunicator* communicator);
    ItemCommunicator* getCommunicator() const noexcept;

    virtual int type() const override = 0;

    virtual char grObjectType() const noexcept override;
};

#endif // ABSTRACTGRQTCONNECTION_H
