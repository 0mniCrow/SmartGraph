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

class GViewPort:public QGraphicsView
{
private:
    bool _add_mode_;
    bool _delete_mode_;
    bool _add_edge_mode_;
    GViewEdge* _new_edge_;
public:
    GViewPort(QWidget* tata = nullptr);
    void changeAddMode(bool mode);
    void changeDeleteMode(bool mode);
    void changeAddEdgeMode(bool mode);
    bool deleteMode()const{return _delete_mode_;}
    bool addMode()const{return _add_mode_;}
protected:
    void mouseReleaseEvent(QMouseEvent* m_event) override;
    void mouseMoveEvent(QMouseEvent* m_event) override;

};

#endif // GVIEWPORT_H
