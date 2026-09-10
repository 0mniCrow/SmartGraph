#ifndef ABSTRACTGRQTCONNECTION_H
#define ABSTRACTGRQTCONNECTION_H
#include "abstractgrconnection.h"
#include "itemcommunicator.h"
#include <QGraphicsObject>

class AbstractGrQtConnection:public QGraphicsObject, public AbstractGrConnection
{
    Q_OBJECT
private:
    /*
     * пазіцыі пачатку і канчатку лініі не тое ж, што пазіцыі аб'ектаў старту і
     * фінішу, гэтыя пазіцыі скарочаны ў адпаведнасьці да памеру канчатковых аб'ектаў.
    */
    QPointF                 _start_pos_;
    QPointF                 _end_pos_;
    ItemCommunicator*       _communicator_;
protected:
    virtual QRectF boundingRect() const override = 0;
    virtual void paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget) override = 0;
    /*
     * фунцыі вяртаюць пазіцыі аб'ектаў старту і фінішу, не пазіцыі пачатку і канчатку лініі
    */
    QPointF getStartingPoint() const;
    QPointF getEndPoint() const;
    QLineF getQLine() const;

public:
    explicit AbstractGrQtConnection();
    explicit AbstractGrQtConnection(const item_id_t& id=item_id_t(),
                           bool directed = false,
                           QGraphicsObject* tata = nullptr);
    virtual ~AbstractGrQtConnection();
    void setArrowSize(qreal ar_size);
    qreal getArrowSize() const;
    void setCommunicator(ItemCommunicator* communicator);
    ItemCommunicator* getCommunicator() const noexcept;

    virtual int type() const override = 0;
    virtual char grObjectType() const noexcept override;

    virtual qreal getLength() const override;
    virtual qreal getGrStartX() const override;
    virtual qreal getGrEndX() const override;
    virtual qreal getGrStartY() const override;
    virtual qreal getGrEndY() const override;
    virtual void getGrStartCoords(qreal& x, qreal& y) const override;
    virtual void getGrEndCoords(qreal& x, qreal& y) const override;
signals:
    void connectionAboutToBeDestroyed(uint con_id);
};

#endif // ABSTRACTGRQTCONNECTION_H
