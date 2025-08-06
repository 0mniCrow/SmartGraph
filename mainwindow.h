#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QMessageBox>
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
    void eventQueue         (QMouseEvent* m_event, const QString& msg);
    QString modifiersInfo   (QMouseEvent* m_event);
    QString buttonsInfo     (QMouseEvent* m_event);
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void setProgressBar(int val, int max);
private slots:
    void execute();
private:
//    VectorGraph graph;
//    ListGraph l_graph;
    TouchForm * touchform;
    MatrixModel * model;
    void keyPressEvent(QKeyEvent* pe) override;

    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
