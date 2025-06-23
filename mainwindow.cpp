#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),graph(4)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    graph.addEdge(0,1);
    graph.addEdge(0,2);
    graph.addEdge(1,3);
    graph.addEdge(2,3);
    ui->textEdit->append(graph.get());

    l_graph.addEdge(1,0);
    l_graph.addEdge(2,0);
    l_graph.addEdge(1,2);
    ui->textEdit->append(l_graph.get());
}

MainWindow::~MainWindow()
{
    delete ui;
}

