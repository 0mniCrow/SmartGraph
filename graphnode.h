#ifndef GRAPHNODE_H
#define GRAPHNODE_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

class GraphEdge;
using local_val_type = int;
using local_id_type = uint;
using edge_ptr = std::weak_ptr<GraphEdge*>;

class GraphNode:public QGraphicsItem
{
private:
    local_id_type _id_;
    QList<edge_ptr> _edges_;
    QPointF _next_position_;
    local_val_type _value_;
    bool _is_hovered_;
public:
    explicit GraphNode(local_id_type id);
    local_id_type getId() const;
    void setId(const local_id_type& id);
    void addEdge(edge_ptr edge);
    void addEdge(const local_id_type& linked_id);
    void removeEdge(edge_ptr edge);
    void removeEdge(uint linked_id);
    void setValue(const local_val_type& value);
    QList<edge_ptr> edges() const;
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter * painter,const QStyleOptionGraphicsItem * option, QWidget * widget) override;
protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;

    void mousePressEvent(QGraphicsSceneMouseEvent * m_event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * m_event) override;

    void hoverEnterEvent(QGraphicsSceneHoverEvent * h_event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent * h_event) override;
};

#endif // GRAPHNODE_H
