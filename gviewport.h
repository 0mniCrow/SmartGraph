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
private:
    bool _add_mode_;
    bool _delete_mode_;
    bool _add_edge_mode_;

    QList<GViewItem*> _vertices_;
    QList<GViewEdge*> _edges_;
    GViewEdge* _new_edge_;
    void delLinkedEdges(GViewItem* vertex);
    bool addEdge(GViewItem* source, GViewItem* dest);
    void startEdge(GViewItem* src);
    void finishEdge(GViewItem* dest);
    QGraphicsItem* grabItem(QMouseEvent* m_event);
    void addItem(GViewItem* vertex, const QPoint& pos);
    void deleteItem(GViewItem* vertex);
public:
    enum GPort_Mode{GPort_NoMode,
          GPort_add,
          GPort_delete,
          GPort_startAddEdge,
          GPort_finAddEdge};
    GViewPort(QWidget* tata = nullptr);
    void changeAddMode(bool mode);
    void changeDeleteMode(bool mode);
    void changeAddEdgeMode(bool mode);
    bool deleteMode()const{return _delete_mode_;}
    bool addMode()const{return _add_mode_;}
    void setMode(GPort_Mode mode) {_mode_ = mode;}
    GPort_Mode mode() const {return _mode_;}
protected:
    void mouseReleaseEvent(QMouseEvent* m_event) override;
    void mouseMoveEvent(QMouseEvent* m_event) override;
private:
    GPort_Mode _mode_;

};

#endif // GVIEWPORT_H
