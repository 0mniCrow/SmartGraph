#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QMessageBox>
#include <QGesture>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QGraphicsProxyWidget>
#include <QDial>
#include "vectorgraph.h"
#include "listgraph.h"
#include "Graph_algorithms.h"
#include "matrixmodel.h"
#include "touchform.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

using std::vector;
using std::string;




class MainWindow : public QMainWindow
{
    Q_OBJECT
protected:
    virtual void mousePressEvent    (QMouseEvent* m_event) override;
    virtual void mouseReleaseEvent  (QMouseEvent* m_event) override;
    virtual void mouseMoveEvent     (QMouseEvent* m_event) override;
    virtual void wheelEvent         (QWheelEvent* w_event) override;
    virtual void enterEvent         (QEnterEvent* e_event) override;
    virtual void leaveEvent         (QEvent* l_event) override;
    virtual void closeEvent         (QCloseEvent*  cl_event) override;
    virtual void resizeEvent        (QResizeEvent* rsz_event) override;
    virtual bool event              (QEvent* reg_event) override;
    bool gestureEvent       (QGestureEvent* g_event);
    void swipeGesture       (QSwipeGesture* sw_gesture);
    void panGesture         (QPanGesture* p_gesture);
    void pinchGesture       (QPinchGesture* p_gesture);
    void tapNholdGesture    (QTapAndHoldGesture* tnh_gesture);
    void tapGesture         (QTapGesture* t_gesture);
    void eventQueue         (QMouseEvent* m_event, const QString& msg);
    QString modifiersInfo   (QMouseEvent* m_event);
    QString buttonsInfo     (QMouseEvent* m_event);
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void setProgressBar(int val, int max);
    void zoomIn();
    void zoomOut();
    void rotateLeft();
    void rotateRight();
private slots:
    void execute();
private:
//    VectorGraph graph;
//    ListGraph l_graph;
    TouchForm * touchform;
    MatrixModel * model;
    void keyPressEvent(QKeyEvent* pe) override;
    void loadGraphicScreen();
    void load2ndGraphicScreen();
    void loadWidgetGraphicScreen();
    Ui::MainWindow *ui;
//    QGraphicsScene scene;
};
#endif // MAINWINDOW_H
