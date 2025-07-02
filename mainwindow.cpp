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

    //-----------------------------------------------------------------------------
    //-----------Rotten Orange problem-------------------------------------------
//    std::vector<std::vector<int>> matrix({{2,1,0,2,1},{1,0,1,2,1},{1,0,0,2,1}});
//    std::vector<std::vector<int>> snd_matrix({{2,1,0,2,1},{0,0,1,2,1},{1,0,0,2,1}});
//    long long time = 0;
//    std::string answer;
//    std::vector<PlayAction> actions;
//    ui->textEdit->append("Elapsing iterations: "+QString::number(orangesRotting_BFS(matrix,time,answer,actions)));
//    ui->textEdit->append("Elapsing time: "+QString::number(time));
//    ui->textEdit->append(QString(answer.c_str()));
//    time = 0;
//    answer.clear();
//    ui->textEdit->append("Elapsing iterations: "+QString::number(orangesRotting_BFS(snd_matrix,time,answer,actions)));
//    ui->textEdit->append("Elapsing time: "+QString::number(time));
//    ui->textEdit->append(QString(answer.c_str()));
//    model = new MatrixModel(matrix);
//    ui->tableView->setModel(model);
//    model->setActions(actions);
    vector<vector<char>> grid({{'L','L','W','W','W'},
                               {'W','L','W','W','L'},
                               {'L','W','W','L','L'},
                               {'W','W','W','W','W'},
                               {'L','W','L','L','W'}});
    vector<vector<char>> grid2({{'W','L','L','L','W','W','W'},
                                {'W','W','L','L','W','L','W'}});
    vector<vector<char>> matrix({ { 'L', 'W', 'W', 'W', 'W' },
                                  { 'W', 'L', 'W', 'W', 'L' },
                                  { 'L', 'W', 'W', 'L', 'L' },
                                  { 'W', 'W', 'W', 'W', 'W' },
                                  { 'L', 'W', 'L', 'L', 'W' } });
    Vector2D<LandNode> pro_matrix;
    pro_matrix.resize(matrix.size(),matrix.at(0).size());
    for(int i = 0;i<pro_matrix.rowCount();i++)
    {
        for(int j = 0; j<pro_matrix.colCount();j++)
        {
            pro_matrix(i,j)._land_type_=matrix.at(i).at(j);
        }
    }
    ui->textEdit->append("Number of islands: "+QString::number(countIslands(pro_matrix)));
//    connect(ui->pushButton_play,SIGNAL(clicked(bool)),model,SLOT(startActions()));
//    connect(model,SIGNAL(updateBar(int, int)),this,SLOT(setProgressBar(int,int)));
}
void MainWindow::setProgressBar(int val, int max)
{
    ui->progressBar->setValue(val*100/max);
}
MainWindow::~MainWindow()
{
    delete ui;
}

