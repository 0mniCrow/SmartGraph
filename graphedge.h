#ifndef GRAPHEDGE_H
#define GRAPHEDGE_H

#include <QGraphicsItem>

class GraphNode;

class GraphEdge: public QGraphicsItem
{
private:
    GraphNode* _source_node_;
    GraphNode* _dest_node_;
    QPointF _source_coord_;
    QPointF _dest_coord_;
    bool _directed_;
    qreal _arrowSize_ = 10;
    char _state_;
public:
    GraphEdge(GraphNode* src, GraphNode* dst, bool directed = false);
    GraphNode* sourceNode() const;
    GraphNode* destNode() const;

    void adjust();
    enum {Type = UserType+2};
    enum {EdgeHideState = 0, EdgeDruggedState = 1, EdgeLinkedState = 2};
    int type() const override {return Type;}
    char state() const {return _state_;}

protected:
    QRectF boundingRect() const override;
    void paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget) override;

};

#endif // GRAPHEDGE_H
