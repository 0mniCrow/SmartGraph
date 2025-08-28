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
    bool _add_mode_;
public:
    GViewScene(QObject* tata = nullptr);
    bool addMode()const{return _add_mode_;}
    void setAddMode(bool add_mode);

protected:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* m_event) override;
};

#endif // GVIEWSCENE_H
