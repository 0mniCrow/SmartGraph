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
    ui->textEdit->append(QString(graph.getAdjacencyMatrix().c_str()));
    ui->textEdit->append(QString(graph.getEdgeTable().c_str()));

    VectorGraph sec_graph(5,VectorGraph::Gr_Directed|VectorGraph::Gr_Weighted);

    sec_graph.addEdge(0,4,8);
    sec_graph.addEdge(1,0,2);
    sec_graph.addEdge(1,2,3);
    sec_graph.addEdge(2,3,2);
    sec_graph.addEdge(2,0,5);
    sec_graph.addEdge(2,4,3);
    sec_graph.addEdge(3,1,6);
    sec_graph.addEdge(3,4,1);
    sec_graph.addEdge(4,2,7);
    sec_graph.addEdge(4,1,1);

    int next_vertex(sec_graph.addVertex());

    sec_graph.addEdge(next_vertex,4,2);
    sec_graph.addEdge(1,next_vertex,6);

    ui->textEdit->append(QString(sec_graph.getAdjacencyMatrix().c_str()));
    ui->textEdit->append(QString(sec_graph.getEdgeTable().c_str()));


    l_graph.addEdge(1,0);
    l_graph.addEdge(2,0);
    l_graph.addEdge(1,2);
    ui->textEdit->append(l_graph.get());
}

MainWindow::~MainWindow()
{
    delete ui;
}

