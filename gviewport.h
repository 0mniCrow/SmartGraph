#ifndef GVIEWPORT_H
#define GVIEWPORT_H

#include <QWidget>
#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include "gviewscene.h"
#include <QPixmap>
#include <QCursor>
#include <QRandomGenerator>
#include <QMouseEvent>
#include <memory>

class GViewPort:public QGraphicsView
{
    Q_OBJECT
public:
    enum GPort_Mode{GPort_NoMode,
                    GPort_add,
                    GPort_delete,
                    GPort_startAddEdge,
                    GPort_finAddEdge,
                    GPort_startDelEdge,
                    GPort_finDelEdge};
    GViewPort(int vertex_radius,QWidget* tata = nullptr);
    void setMode(GPort_Mode mode);
    GPort_Mode mode() const {return _mode_;}
    int vertRadius()const {return _vertex_radius_;}
    void setRadius(int radius);
protected:
    void mouseReleaseEvent(QMouseEvent* m_event) override;
    void mouseMoveEvent(QMouseEvent* m_event) override;
private:
    QList<GViewItem*> _vertices_;
    QList<GViewEdge*> _edges_;
    GViewEdge* _new_edge_;
    GViewItem* _del_edge_;
    GViewItem* _selected_vertex_;
    int _vertex_radius_;
    GPort_Mode _mode_;
    int _counter_;

    void delLinkedEdges(GViewItem* vertex);
    bool addEdge(GViewItem* source, GViewItem* dest,bool directed = true);
    void startAddEdge(GViewItem* src);
    void finishAddEdge(GViewItem* dest);
    void startDelEdge(GViewItem* src);
    void finishDelEdge(GViewItem* dest);
    QGraphicsItem* grabItem(QMouseEvent* m_event);
    GViewItem* grabGItem(QMouseEvent* m_event);
    void addItem(GViewItem* vertex, const QPoint& pos);
    void deleteItem(GViewItem* vertex);
signals:
    void selectedInfo(QString info);
};

#endif // GVIEWPORT_H
