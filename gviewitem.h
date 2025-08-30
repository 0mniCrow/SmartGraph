#ifndef GVIEWITEM_H
#define GVIEWITEM_H
#define LINE_WIDTH 2.0

#include <QGraphicsItem>
#include <QColor>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
class GViewEdge;

class GViewItem:public QGraphicsItem
{
private:
    QString _info_;
    QColor _color_;
    bool _is_hovered_;
    bool _is_clicked_;
    QVector<GViewEdge*> _edges_;

public:
    GViewItem(const QString& info = QString(), const QColor& color = QColor());
    GViewItem(const QColor& color);
    void addEdge(GViewEdge* edge);
    void delEdge(GViewEdge* edge);
    void setColor(const QColor& color);
    void setInfo(const QString& info);
    QString info()const;
    QColor color()const;
    enum {Type = UserType+1};
    int type() const override{return Type;}

protected:
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;
    void paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget) override;
    void mousePressEvent(QGraphicsSceneMouseEvent * m_event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * m_event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent * h_event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent * h_event) override;
};

#endif // GVIEWITEM_H
