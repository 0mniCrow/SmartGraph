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
#include "visualisationgraphform.h"

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
private:
    TouchForm * touchform;
    VisualisationGraphForm *vis_form;
    MatrixModel * model;
    void keyPressEvent(QKeyEvent* pe) override;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
