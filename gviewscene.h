#ifndef GVIEWSCENE_H
#define GVIEWSCENE_H

#include "gviewitem.h"
#include "gviewedge.h"
#include <QGraphicsScene>
#include <QApplication>
#include <QColor>
#include <QCursor>
#include <QRandomGenerator>
#include <QGraphicsSceneMouseEvent>

class GViewScene:public QGraphicsScene
{

public:
    GViewScene(QObject* tata = nullptr);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* m_event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* m_event) override;
};

#endif // GVIEWSCENE_H
