#ifndef ABSTRACTGRITEM_H
#define ABSTRACTGRITEM_H
#include "abstractGrInterface.h"
#include "QGraphicsObject"
#include "QGraphicsScene"
#include "itemcommunicator.h"
#include <QPixmap>
#include <QPoint>
#include <QPointF>



#define DEF_ITEM_RADIUS 20
#define MIN_ITEM_RADIUS 10
#define LINE_ITEM_CLICKED_WIDTH 3.0
#define LINE_ITEM_SELECT_WIDTH 2.5
#define LINE_ITEM_BASE_WIDTH 2.0
#define SELECTED_ITEM_RISE 5.0
#define MOUSE_SENSE_ITEM_DECR 0.5
#define PIN_HEAD_ITEM_RADIUS 3

class AbstractGrConnection;

class AbstractGrItem:public AbstractGrInterface, public QGraphicsObject
{
    Q_OBJECT
private:
    QVector<AbstractGrConnection*>      _edges_;
    QPixmap                             _orig_pixmap_;
    QPixmap                             _icon_;
    QPointF                             _adv_pos_;
    QPoint                              _last_screen_pos_;
    ItemCommunicator*                   _communicator_;
    int                                 _radius_;
    char                                _flags_;
    void keepInBorders();
    void iconUpdate();
    void drawVertexCircle(QPainter* painter);
    void drawVertexIcon(QPainter* painter);
    void drawPinNeedle(QPainter* painter);
protected:
    enum ItemFlags{ GV_None = 0x00, GV_Is_Clicked = 0x01,
                  GV_Ignore_Next_Move = 0x02,
                  GV_Is_Dragged = 0x04,GV_Is_Forced = 0x08,
                  GV_Def_Icon = 0x10};
    virtual QRectF boundingRect() const override;
    virtual QPainterPath shape() const override;
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;
    virtual void paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget) override;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * m_event) override;
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* m_event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * m_event) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* m_event) override;
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent * h_event) override;
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent * h_event) override;


public:
    enum GraphicItemType{AbstractItem = 1};
    AbstractGrItem(const item_id_t& id=item_id_t(),
                   int radius = DEF_ITEM_RADIUS,
                   QGraphicsItem* tata = nullptr);
    virtual ~AbstractGrItem();
    void setItemCommunicator(ItemCommunicator* communicator);
    void setRadius(int radius);
    int getRadius() const noexcept {return _radius_;}
    void setGrFlag(char flag, bool state);
    void setGrFlags(char flags);
    char getFlags() const noexcept {return _flags_;}
    void addEdge(AbstractGrConnection* edge);
    void delEdge(AbstractGrConnection* edge);
    QList<AbstractGrConnection*> getEdges() const;
    void setImage(const QPixmap& image);
    QPixmap getImage() const { return _orig_pixmap_; }

    virtual void setGrX(coord_real x) override;
    virtual void setGrY(coord_real y) override;
    virtual coord_real getGrX() const override;
    virtual coord_real getGrY() const override;
    virtual void setGrWidth(coord_real width) override;
    virtual void setGrHeight(coord_real height) override;
    virtual coord_real getGrWidth() const override;
    virtual coord_real getGrHeight() const override;
    virtual void moveGr(coord_real x, coord_real y) override;
    virtual void drawGr() override;
    virtual char graphicType() const noexcept override{return AbstractItem;}
};

#endif // ABSTRACTGRITEM_H
