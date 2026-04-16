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
#include <QTimer>
#include "graphic_elements/nodeobjectinfo.h"
#include "widgets/gview_tooltip_window.h"
#include "widgets/gview_edit_window.h"

/*#define INFO_COMPLEX_OBJECT*/
typedef QMap<QString,QString> vert_map;

#ifdef INFO_COMPLEX_OBJECT
    using info_type = NodeObjectInfo;
#else
    using info_type = QString;
#endif

#define ICON_SIZE QSize(32,32)

class GViewEdge;

class GViewItem:public QGraphicsObject
{
    Q_OBJECT
private:
    enum ItemFlags{ GV_None = 0x00, GV_Is_Clicked = 0x01,
                  GV_Ignore_Next_Move = 0x02,
                  GV_Is_Dragged = 0x04,GV_Is_Forced = 0x08};
    QPixmap * _no_image_;
    QPixmap _icon_;
    QVector<GViewEdge*> _edges_;
    info_type _info_;
    QColor _color_;
    QPointF _adv_pos_;
    GViewToolTip * _tooltip_window_;
    GViewEdit * _edit_window_;
    QPoint _last_screen_pos_;
    QTimer _show_timer_;
    int _radius_;
    bool _is_clicked_;
    char _flags_;

    void checkBorders();
    char GVflags() const {return _flags_;}
    void setGVFlag(char flag, bool state=true) { _flags_= state? _flags_|flag : _flags_ & ~flag;}
    void setGVFlags(char flags) {_flags_ = flags;}
    void startTipTimer();
    void breakTipTimer();
    void callTipWindow(QGraphicsSceneMouseEvent* m_event = nullptr);
    void callEditWindow(QGraphicsSceneMouseEvent* m_event = nullptr);
public:


#ifdef INFO_COMPLEX_OBJECT
    GViewItem(int radius,
              const NodeObjectInfo& info,
              const QColor& color = QColor());
    GViewItem(int radius,
              NodeObjectInfo&& info,
              const QColor& color = QColor());
#else
    GViewItem(int radius,QPixmap* def_image,
              const QString& info = QString(),
              const QColor& color = QColor());
#endif

    GViewItem(int radius, QPixmap* def_image, const QColor& color);
    ~GViewItem();
    void addEdge(GViewEdge* edge);
    void delEdge(GViewEdge* edge);
    int radius()const{return _radius_;}
    void setRadius(int radius);
    void setColor(const QColor& color);
     #ifdef INFO_COMPLEX_OBJECT
    void setInfo(NodeObjectInfo&& info);
    const NodeObjectInfo& info()const;
    #else
    void setInfo(const QString& info);
    QString info()const;
    #endif

    QColor color()const;
    bool isForceCalc()const {return _flags_&GV_Is_Forced;}
    void setForceCalc(bool state){_flags_= state? _flags_|GV_Is_Forced : _flags_ & ~GV_Is_Forced;}
    void calcForce();
    bool advPosition();
    enum {Type = UserType+1};
    int type() const override{return Type;}

    void gatherInfo(vert_map* item_container) const;
    //void loadInfo(vert_map* item_container);
protected:
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;
    void paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget) override;
    void mousePressEvent(QGraphicsSceneMouseEvent * m_event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* m_event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * m_event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* m_event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent * h_event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent * h_event) override;
private slots:
    void showTipWindow();
    void getNewInfo(const QString& new_val);
signals:
    void changedInternally(GViewItem* self);
    void changedExternally(QString new_val);
};


#endif // GVIEWITEM_H
