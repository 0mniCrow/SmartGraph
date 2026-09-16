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
     * пазіцыя пачатку лініі не тое ж, што пазіцыя аб'екту старту,
     * гэта пазіцыя скарочана ў адпаведнасьці да памеру стартавага аб'екту.
    */
    QPointF                 _start_endpoint_;
    /*
     * пазіцыя канчатку лініі не тое ж, што пазіцыя аб'екту фінішу,
     * гэта пазіцыя скарочана ў адпаведнасьці да памеру фінішнага аб'екту.
    */
    QPointF                 _finish_endpoint_;
    /*
     * аб'ект камунікацыі з аб'ектам візуальнага порта
    */
    ItemCommunicator*       _communicator_;
    //Пераразлічванне пазіцый пачатку і канчатку лініі рабра. !Пункты павінны быць пераведзены ў каардынаты бягучага ітэма!
    void recalculation(const QPointF& start_point, const QPointF& fin_point);
    void setDefaultPreferences();
protected:
    virtual QRectF boundingRect() const override = 0;
    virtual void paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget) override = 0;


    /*
     * Source\Destination point фунцыі вяртаюць пазіцыі аб'ектаў старту і фінішу,
     * не пазіцыі пачатку і канчатку лініі
    */
    QPointF getSourcePoint() const;
    /*
     * Source\Destination point фунцыі вяртаюць пазіцыі аб'ектаў старту і фінішу,
     * не пазіцыі пачатку і канчатку лініі
    */
    QPointF getDestinationPoint() const;
    //Даўжыня рэбра паміж дзьвума пунктамі. !Пункты павінны быць пераведзены ў каардынаты бягучага ітэма!
    qreal getLength(const QPointF& src, const QPointF& dest) const;
    virtual void redraw() override;

    /*
     * Кумулятыўная функцыя, трэба каб вызначыць працоўны стан аб'екту.
     * !Пры перагрузцы функцыі ў канцы трэба вяртаць выклік той жа функцыі аб'екту продка.
    */
    virtual bool checkStatus() const override;


public:
    explicit AbstractGrQtConnection(const item_id_t& id=GR_ITEM_ID_DEF,
                                    bool directed = false,
                                    qreal weight = ABSTRACT_EDGE_DEF_WEIGHT,
                                    QGraphicsObject* tata = nullptr);
    AbstractGrQtConnection(AbstractGrItem* source,
                           AbstractGrItem* destination,
                           char mode, bool directed = false,
                           const item_id_t& id= GR_ITEM_ID_DEF,
                           qreal weight = ABSTRACT_EDGE_DEF_WEIGHT,
                           QGraphicsObject* tata = nullptr);
    virtual ~AbstractGrQtConnection();
    void setArrowSize(qreal ar_size);
    qreal getArrowSize() const;
    void setCommunicator(ItemCommunicator* communicator);
    ItemCommunicator* getCommunicator() const noexcept;

    /*
     * setStartEndpoint функцыя задае пазіцыю пачатку лініі,
     * не пазіцыю аб'ектаў старту
     */
    void setStartEndpoint(const QPointF& point);
    /*
     * getStartEndpoint функцыя вяртае пазіцыю пачатку лініі,
     * не пазіцыю аб'ектаў старту
     */
    QPointF getStartEndpoint() const;
    /*
     * setFinishEndpoint функцыя задае пазіцыю канчатку лініі,
     * не пазіцыю аб'ектаў фінішу
     */
    void setFinishEndpoint(const QPointF& point);
    /*
     * getFinishEndpoint функцыя вяртае пазіцыю пачатку лініі,,
     * не пазіцыю аб'ектаў фінішу
     */
    QPointF getFinishEndpoint() const;
    qreal getEdgeLength() const;
    virtual int type() const override = 0;
    virtual char grObjectType() const noexcept override;

    virtual void recalcEndpoints() override;
    void recalcEndpoints(const QPointF& destination);
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
