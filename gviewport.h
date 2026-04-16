#ifndef GVIEWPORT_H
#define GVIEWPORT_H

#include <QWidget>
#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QScrollBar>
#include "gviewscene.h"
#include "gview_tableVertexModel.h"
#include <QPixmap>
#include <QCursor>
#include <QRandomGenerator>
#include <QMouseEvent>
#include <memory>
#include "listgraph.h"
#include <QTimer>
#include <QBasicTimer>
#include <QSet>
#include <chrono>
#include <QDomNode>

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
    enum GPort_Controls{GPort_Ctr_NoMode = 0x00,
                        GPort_Ctr_Zoom = 0x01,
                        GPort_Ctr_SceneDrag = 0x02,
                        GPort_Ctr_SceneDragMode = 0x04};
    GViewPort(int vertex_radius, VertexModel* model, QWidget* tata = nullptr);
    void setMode(GPort_Mode mode);
    void setControlState(GPort_Controls mode, bool state);
    GPort_Mode mode() const {return _mode_;}
    char controlState() const {return _controls_state_;}
    int vertRadius()const {return _vertex_radius_;}
    void setRadius(int radius);
    bool loadListGraph(const ListGraph& graph);
    void itemMoved();
    void setForceCalc(bool state, bool fixateEdgeLength = true);
    void clear();
    void gatherInfo(nest_vert_map& vertices,
                    nest_vert_map& edges) const;
    void loadInfo(const nest_vert_map& vertices,
                  const nest_vert_map& edges);
    void setZoomMode(bool state);
protected:
    void mousePressEvent(QMouseEvent* m_event) override;
    void mouseReleaseEvent(QMouseEvent* m_event) override;
    void mouseMoveEvent(QMouseEvent* m_event) override;
    void contextMenuEvent(QContextMenuEvent* c_event) override;
    void wheelEvent(QWheelEvent* w_event) override;
    void timerEvent(QTimerEvent * t_event) override;
private:
    VertexModel * _vertices_;
    QList<GViewEdge*> _edges_;
    GViewEdge* _new_edge_;
    GViewItem* _del_edge_;
    GViewItem* _selected_vertex_;
    QBasicTimer _timer_;
    QPixmap _no_image_;                 //дэфолтны відарыс для аб'ектаў
    int _vertex_radius_;
    QPoint _last_pos_;
    QCursor _saved_cursor_;
    GPort_Mode _mode_;
    char _controls_state_;
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
    void createItem(const QPoint& pos);
    void deleteItem(GViewItem* vertex);
    void selectItem(GViewItem* selected_item, bool outside = false);
    void zoomIn();
    void zoomOut();

signals:
    void gviewMessage(QString info);
    void viewNewSelect(GViewItem* new_item);
    void internalForceArrangeAct();
public slots:
    void outsideNewSelect(GViewItem* selected_item);

};

#endif // GVIEWPORT_H
