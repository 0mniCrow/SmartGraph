#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),graph(4),l_graph(3)
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
    sec_graph.addEdge(0,1,2);
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
    ui->textEdit->append(QString(Breadth_first_search(sec_graph,0).c_str()));
    ui->textEdit->append(QString(Depth_first_search(sec_graph,0).c_str()));

    //---------------------------------------------------------------------------
    ui->textEdit->append("\n\n\nList graph:");

    l_graph.addEdge(1,0);
    l_graph.addEdge(2,0);
    l_graph.addEdge(1,2);
    ui->textEdit->append(QString(l_graph.getAdjacencyList().c_str()));
    ui->textEdit->append(QString(Breadth_first_search(l_graph,0).c_str()));

    ListGraph sec_list_graph(6,ListGraph::Gr_Directed|ListGraph::Gr_Weighted);
    sec_list_graph.addEdge(0,1,2);
    sec_list_graph.addEdge(0,2,3);
    sec_list_graph.addEdge(1,2,2);
    sec_list_graph.addEdge(1,3,2);
    sec_list_graph.addEdge(2,3,2);
    sec_list_graph.addEdge(3,4,3);
    sec_list_graph.addEdge(3,5,5);
    sec_list_graph.addEdge(4,5,2);
    sec_list_graph.addEdge(5,0,9);

    ui->textEdit->append(QString(sec_list_graph.getAdjacencyList().c_str()));
    ui->textEdit->append(QString(sec_list_graph.getEdgeTable().c_str()));

    sec_list_graph.addVertex(7);
    sec_list_graph.addEdge(5,7,4);
    int new_vert=sec_list_graph.addVertex();
    sec_list_graph.addEdge(7,new_vert,3);

    ui->textEdit->append(QString(sec_list_graph.getAdjacencyList().c_str()));
    ui->textEdit->append(QString(sec_list_graph.getEdgeTable().c_str()));

    ui->textEdit->append(QString(Breadth_first_search(sec_list_graph,0).c_str()));
    ui->textEdit->append(QString(Depth_first_search(sec_list_graph,3).c_str()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

