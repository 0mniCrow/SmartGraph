#ifndef GVIEWSCENE_H
#define GVIEWSCENE_H

#include "gviewitem.h"
#include "
#include <QGraphicsScene>
#include <QApplication>
#include <QColor>
#include <QCursor>
#include <QRandomGenerator>
#include <QGraphicsSceneMouseEvent>

class GViewScene:public QGraphicsScene
{
private:
    bool _add_mode_;
    bool _create_edge_mode_;
    GViewItem* _src_4_edge_;
public:
    GViewScene(QObject* tata = nullptr);
    bool addMode()const{return _add_mode_;}
    void setAddMode(bool add_mode);
    bool edgeMode()const{return _create_edge_mode_;}
    void setEdgeMode(bool edge_mode);
protected:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* m_event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* m_event) override;
};

#endif // GVIEWSCENE_H
