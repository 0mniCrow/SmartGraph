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
public:
    GViewPort(QWidget* tata = nullptr);
    void changeAddMode(bool mode);
    bool addMode()const{return _add_mode_;}
protected:
    void mouseReleaseEvent(QMouseEvent* m_event);

};

#endif // GVIEWPORT_H
