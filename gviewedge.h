#ifndef GVIEWEDGE_H
#define GVIEWEDGE_H

#include <QGraphicsItem>
#include "gviewitem.h"

class GViewEdge:public QGraphicsItem
{
private:
    QPointF _src_point_;
    QPointF _dest_point_;
    GViewItem* _src_item_;
    GViewItem* _dest_item_;
    bool _directed_;
public:
    GViewEdge(GViewItem* source,
              GViewItem* destination,
              bool directed = false);
    GViewItem* source() const;
    GViewItem* destination() const;
    void recalculate();
    enum {Type = UserType+2};
    int type() const override {return Type;}
protected:
    QRectF boundingRect() const override;
    void paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget) override;
};

#endif // GVIEWEDGE_H
