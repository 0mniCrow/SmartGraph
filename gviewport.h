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
    char _mode_;
    QList<std::unique_ptr<GViewItem>> _items_;
    QList<std::unique_ptr<GViewEdge>> _edges_;
    GViewEdge* _new_edge_;
public:
    enum {GPort_NoMode = 0,
          GPort_add = 1,
          GPort_delete = 2,
          GPort_addEdge = 3,
          GPort_delEdge = 4};
    GViewPort(QWidget* tata = nullptr);
    void changeAddMode(bool mode);
    void changeDeleteMode(bool mode);
    void changeAddEdgeMode(bool mode);
    bool deleteMode()const{return _delete_mode_;}
    bool addMode()const{return _add_mode_;}
    void setMode(char mode);
    char mode() const;
protected:
    void mouseReleaseEvent(QMouseEvent* m_event) override;
    void mouseMoveEvent(QMouseEvent* m_event) override;

};

#endif // GVIEWPORT_H
