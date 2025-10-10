#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QMessageBox>
#include "vectorgraph.h"
#include "listgraph.h"
#include "Graph_algorithms.h"
#include "matrixmodel.h"
#include "touchform.h"
//___________________Visual graph______________________
#include "gviewport.h"
#include "gview_tableVertexView.h"
//______________________End____________________________
//#include "visualisationgraphform.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

using std::vector;
using std::string;

class MainWindow : public QMainWindow
{
    Q_OBJECT
protected:
    virtual void closeEvent         (QCloseEvent*  cl_event) override;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void setProgressBar(int val, int max);
private slots:
    void execute();
    //_____________Visual graph_______________
    void AddObject();
    void RemoveObject();
    void CreateEdge();
    void RemoveEdge();
    void ChangeSize(int radius);
    void updateInfo(QString info);
    void fixEdges(bool status);
    //_______________End______________________
private:
    TouchForm * touchform;
    //VisualisationGraphForm *vis_form;
    MatrixModel * model;
    //____________Visual graph_______________
    GViewScene* _scene_;
    GViewPort* _view_;
    VertexModel* _model_;
    VertexList* _vert_list_;
    //______________End________________________
    void keyPressEvent(QKeyEvent* pe) override;
    void initiateGraphicsView();
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
