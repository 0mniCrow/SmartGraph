#ifndef ABSTRACTGRITEM_H
#define ABSTRACTGRITEM_H
#include "abstractGrInterface.h"
#include "QGraphicsObject"
#include <QPixmap>
#include <QPoint>
#include <QPointF>



#define DEF_RADIUS 20

class AbstractGrConnection;

class AbstractGrItem:public AbstractGrInterface, public QGraphicsObject
{
    Q_OBJECT
private:
    enum ItemFlags{ GV_None = 0x00, GV_Is_Clicked = 0x01,
                  GV_Ignore_Next_Move = 0x02,
                  GV_Is_Dragged = 0x04,GV_Is_Forced = 0x08,
                  GV_Def_Icon = 0x10};

    QPixmap                 _orig_pixmap_;
    QPixmap                 _icon_;
    QPointF                 _adv_pos_;
    QPoint                  _last_screen_pos_;
    int                     _radius_;
    char                    _flags_;
public:
    AbstractGrItem(const item_id_t& id=item_id_t(),
                   int radius = DEF_RADIUS,
                   QGraphicsItem* tata = nullptr);
    virtual ~AbstractGrItem();
    void setRadius(int radius);
    int getRadius() const noexcept;
    void setFlag(char flag, bool state);
    void setFlags(char flags);
    char getFlags();
    void addEdge(AbstractGrConnection* edge);
    void delEdge(AbstractGrConnection* edge);
    QList<AbstractGrConnection*> getEdges() const;
    void setImage(const QPixmap& image);
    QPixmap getImage() const { return _orig_pixmap_;}

    virtual void setX(coord_real x) override;
    virtual void setY(coord_real y) override;
    virtual coord_real getX() const override;
    virtual coord_real getY() const override;
    virtual void setWidth(coord_real width) override;
    virtual void setHeight(coord_real height) override;
    virtual coord_real getWidth() const override;
    virtual coord_real getHeight() const override;
    virtual void move(coord_real x, coord_real y) override;
    virtual void draw() override;

};

#endif // ABSTRACTGRITEM_H
