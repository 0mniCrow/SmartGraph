#ifndef GVIEWEDGE_H
#define GVIEWEDGE_H

#define EDGE_WIDTH 2.0

#include <QGraphicsItem>
#include "gviewitem.h"

class GViewEdge:public QGraphicsItem
{
public:
    enum GViewEdge_mode{GVedge_regular, GVedge_incomplete, GVedge_deletion};
    GViewEdge(GViewItem* source,
              GViewItem* destination,
              int vert_radius,
              bool directed = false,
              GViewEdge_mode mode = GVedge_regular);
    GViewEdge(GViewItem* source,
              int vert_radius,
              bool direction = false,
              GViewEdge_mode mode = GVedge_incomplete);
    bool isDirected() const{return _directed_;}
    void setDirected(bool directed){_directed_=directed;}
    GViewItem* source() const;
    GViewItem* destination() const;
    GViewItem* setSource(GViewItem* new_src);
    bool setDest(GViewItem* new_dest);
    void setMode(GViewEdge_mode mode){_mode_=mode;}
    GViewEdge_mode mode()const{return _mode_;}
    void recalculate();
    void setVertRadius(int radius);
    void searchDestination(const QPointF& point);
    enum {Type = UserType+2};
    int type() const override {return Type;}
    QPainterPath shape() const override;
protected:
    QRectF boundingRect() const override;
    void paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget) override;
private:
    QPointF _src_point_;
    QPointF _dest_point_;
    GViewItem* _src_item_;
    GViewItem* _dest_item_;
    int _vertex_radius_;
    bool _directed_;
    //bool _incomplete_;
    GViewEdge_mode _mode_;

};

#endif // GVIEWEDGE_H
