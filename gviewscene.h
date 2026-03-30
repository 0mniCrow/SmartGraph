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
private:
    QPixmap _bg_;
public:
    GViewScene(QObject* tata = nullptr);
    bool setBG(const QPixmap& source_bg);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* m_event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* m_event) override;
    void drawBackground(QPainter* painter, const QRectF& rect) override;
};

#endif // GVIEWSCENE_H
