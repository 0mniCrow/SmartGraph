#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "vectorgraph.h"
#include "listgraph.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    VectorGraph graph;
    ListGraph l_graph;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
