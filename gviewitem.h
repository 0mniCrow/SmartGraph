#ifndef GVIEWITEM_H
#define GVIEWITEM_H
//#include <qglobal.h>
//#ifdef Q_OS_WIN
//    #include <windows.h>
//#endif


#define LINE_CLICKED_WIDTH 3.0
#define LINE_SELECT_WIDTH 2.0
#define LINE_BASE_WIDTH 1.0
#define SELECTED_RISE 5
#define MOUSE_SENSE_DECR 0.5
#define PIN_HEAD_RADIUS 3

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QColor>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QCursor>
#include <QMenu>
#include <QIcon>
#include "graphic_elements/nodeobjectinfo.h"

using info_type = QString;
//using info_type = NodeObjectInfo;

#define ICON_SIZE QSize(32,32)

class GViewEdge;

class GViewItem:public QGraphicsItem
{
private:
    enum ItemFlags{ GV_None = 0x00, GV_Is_Clicked = 0x01,
                  GV_Ignore_Next_Move = 0x02,
                  GV_Is_Dragged = 0x04,GV_Is_Forced = 0x08};
    QVector<GViewEdge*> _edges_;
    QString _info_;
    QColor _color_;
    QPointF _adv_pos_;
    int _radius_;
    bool _is_clicked_;
    char _flags_;

    void checkBorders();
    char GVflags() const {return _flags_;}
    void setGVFlag(char flag, bool state=true) { _flags_= state? _flags_|flag : _flags_ & ~flag;}
    void setGVFlags(char flags) {_flags_ = flags;}
public:
    GViewItem(int radius, const QString& info = QString(), const QColor& color = QColor());
    GViewItem(int radius, const QColor& color);
    ~GViewItem();
    void addEdge(GViewEdge* edge);
    void delEdge(GViewEdge* edge);
    int radius()const{return _radius_;}
    void setRadius(int radius);
    void setColor(const QColor& color);
    void setInfo(const QString& info);
    QString info()const;
    QColor color()const;
    bool isForceCalc()const {return _flags_&GV_Is_Forced;}
    void setForceCalc(bool state){_flags_= state? _flags_|GV_Is_Forced : _flags_ & ~GV_Is_Forced;}
    void calcForce();
    bool advPosition();
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
    void mouseMoveEvent(QGraphicsSceneMouseEvent* m_event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent * h_event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent * h_event) override;

};


#endif // GVIEWITEM_H
