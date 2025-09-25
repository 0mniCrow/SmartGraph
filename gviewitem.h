#ifndef GVIEWITEM_H
#define GVIEWITEM_H
#define LINE_CLICKED_WIDTH 3.0
#define LINE_SELECT_WIDTH 2.0
#define LINE_BASE_WIDTH 1.0
#define SELECTED_RISE 3

#include <QGraphicsItem>
#include <QColor>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>

class GViewEdge;

class GViewItem:public QGraphicsItem
{
private:
    int _radius_;
    QString _info_;
    QColor _color_;
    bool _is_clicked_;
    QVector<GViewEdge*> _edges_;
    void checkBorders();
public:
    GViewItem(int radius, const QString& info = QString(), const QColor& color = QColor());
    GViewItem(int radius, const QColor& color);
    void addEdge(GViewEdge* edge);
    void delEdge(GViewEdge* edge);
    int radius()const{return _radius_;}
    void setRadius(int radius);
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
