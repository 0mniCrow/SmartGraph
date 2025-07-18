#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),graph(4),l_graph(3)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//___________________________________Matrix graph__________________________

//    graph.addEdge(0,1);
//    graph.addEdge(0,2);
//    graph.addEdge(1,3);
//    graph.addEdge(2,3);
//    ui->textEdit->append(QString(graph.getAdjacencyMatrix().c_str()));
//    ui->textEdit->append(QString(graph.getEdgeTable().c_str()));

//    VectorGraph sec_graph(5,VectorGraph::Gr_Directed|VectorGraph::Gr_Weighted);

//    sec_graph.addEdge(0,4,8);
//    sec_graph.addEdge(0,1,2);
//    sec_graph.addEdge(1,0,2);
//    sec_graph.addEdge(1,2,3);
//    sec_graph.addEdge(2,3,2);
//    sec_graph.addEdge(2,0,5);
//    sec_graph.addEdge(2,4,3);
//    sec_graph.addEdge(3,1,6);
//    sec_graph.addEdge(3,4,1);
//    sec_graph.addEdge(4,2,7);
//    sec_graph.addEdge(4,1,1);

//    int next_vertex(sec_graph.addVertex());

//    sec_graph.addEdge(next_vertex,4,2);
//    sec_graph.addEdge(1,next_vertex,6);

//    ui->textEdit->append(QString(sec_graph.getAdjacencyMatrix().c_str()));
//    ui->textEdit->append(QString(sec_graph.getEdgeTable().c_str()));
//    ui->textEdit->append(QString(Breadth_first_search(sec_graph,0).c_str()));
//    ui->textEdit->append(QString(Depth_first_search(sec_graph,0).c_str()));

    //-----------------------------------List graph----------------------------------------
//    ui->textEdit->append("\n\n\nList graph:");

//    l_graph.addEdge(1,0);
//    l_graph.addEdge(2,0);
//    l_graph.addEdge(1,2);
//    ui->textEdit->append(QString(l_graph.getAdjacencyList().c_str()));
//    ui->textEdit->append(QString(Breadth_first_search(l_graph,0).c_str()));

//    ListGraph sec_list_graph(6,ListGraph::Gr_Directed|ListGraph::Gr_Weighted);
//    sec_list_graph.addEdge(0,1,2);
//    sec_list_graph.addEdge(0,2,3);
//    sec_list_graph.addEdge(1,2,2);
//    sec_list_graph.addEdge(1,3,2);
//    sec_list_graph.addEdge(2,3,2);
//    sec_list_graph.addEdge(3,4,3);
//    sec_list_graph.addEdge(3,5,5);
//    sec_list_graph.addEdge(4,5,2);
//    sec_list_graph.addEdge(5,0,9);

//    ui->textEdit->append(QString(sec_list_graph.getAdjacencyList().c_str()));
//    ui->textEdit->append(QString(sec_list_graph.getEdgeTable().c_str()));

//    sec_list_graph.addVertex(7);
//    sec_list_graph.addEdge(5,7,4);
//    int new_vert=sec_list_graph.addVertex();
//    sec_list_graph.addEdge(7,new_vert,3);

//    ui->textEdit->append(QString(sec_list_graph.getAdjacencyList().c_str()));
//    ui->textEdit->append(QString(sec_list_graph.getEdgeTable().c_str()));

//    ui->textEdit->append(QString(Breadth_first_search(sec_list_graph,0).c_str()));
//    ui->textEdit->append(QString(Depth_first_search(sec_list_graph,3).c_str()));

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
    //________________________Land_Finding______________________________
//    vector<vector<char>> grid({{'L','L','W','W','W'},
//                               {'W','L','W','W','L'},
//                               {'L','W','W','L','L'},
//                               {'W','W','W','W','W'},
//                               {'L','W','L','L','W'}});
//    vector<vector<char>> grid2({{'W','L','L','L','W','W','W'},
//                                {'W','W','L','L','W','L','W'}});
//    vector<vector<char>> matrix({ { 'L', 'W', 'W', 'W', 'W' },
//                                  { 'W', 'L', 'W', 'W', 'L' },
//                                  { 'L', 'W', 'W', 'L', 'L' },
//                                  { 'W', 'W', 'W', 'W', 'W' },
//                                  { 'L', 'W', 'L', 'L', 'W' } });
    //Vector2D<LandNode> pro_matrix;
    //pro_matrix.resize(matrix.size(),matrix.at(0).size());
    //Vector2D<char> pro_matrix;
    //pro_matrix.resize(grid.size(),grid.at(0).size());
    //pro_matrix.resize(grid2.size(),grid2.at(0).size());
//    for(int i = 0;i<pro_matrix.rowCount();i++)
//    {
//        for(int j = 0; j<pro_matrix.colCount();j++)
//        {
            //pro_matrix(i,j)._land_type_=matrix.at(i).at(j);
            //pro_matrix(i,j) = grid.at(i).at(j);
            //pro_matrix(i,j) = grid2.at(i).at(j);
            //pro_matrix(i,j)._land_type_=grid.at(i).at(j);
//        }
//    }

//    model = new MatrixModel(/*grid*/grid2/*matrix*/);
//    ui->tableView->setModel(model);
//    std::vector<PlayAction> actions;
//    ui->textEdit->append("Number of islands: "+QString::number(/*countIslands(pro_matrix,actions)*/
//                                                               /*countLand_OPT(pro_matrix,actions)*/
//                                                               countLand_BFS_OPT_based(pro_matrix,actions)));
//    model->setActions(actions);

    //_______________________________Flood_Fill_______________________________________

//    vector<vector<int>> matrix({
//                                   {1,1,1,0,0,1},
//                                   {0,1,1,1,0,1},
//                                   {1,0,1,1,0,0},
//                                   {1,0,0,0,0,1}
//                               });
//    int start_row = 1,start_col = 2;
//    int new_color = 2;
//    Vector2D<int> pro_matrix;
//    pro_matrix.fill(matrix);
//    model = new MatrixModel(matrix);
//    ui->tableView->setModel(model);
//    std::vector<PlayAction> actions;
//    //floodFill_DFS_Base(pro_matrix,start_row,start_col,new_color,actions);
//    //ui->textEdit->append("FloodFill is ready;");
//    ui->textEdit->append("Flood fill managed to fill ["+
//                         QString::number(floodFill_BFS(pro_matrix,
//                                                       start_row,
//                                                       start_col,
//                                                       new_color,
//                                                       actions))
//                         +"] cells");
//    model->setActions(actions);
//    connect(ui->pushButton_play,SIGNAL(clicked(bool)),model,SLOT(startActions()));
//    connect(model,SIGNAL(updateBar(int, int)),this,SLOT(setProgressBar(int,int)));

    //_________________________________Bipartite check___________________________________

//    ListGraph listgraph;
//    listgraph.addVertex(0);
//    listgraph.addVertex(1);
//    listgraph.addVertex(2);
//    listgraph.addVertex(3);
//    listgraph.addEdge(0,1);
//    listgraph.addEdge(0,2);
//    listgraph.addEdge(1,2);
//    listgraph.addEdge(2,3);
//    ui->textEdit->append(QString(listgraph.getAdjacencyList().c_str()));
//    std::string actions;
//    //isBipartite_BFS(listgraph,actions);
//    isBipartite_DFS_Base(listgraph, actions);
//    ui->textEdit->append(QString(actions.c_str()));

//    listgraph.clear();
//    listgraph.addVertex(0);
//    listgraph.addVertex(1);
//    listgraph.addVertex(2);
//    listgraph.addVertex(3);
//    listgraph.addVertex(4);
//    listgraph.addVertex(5);

//    listgraph.addEdge(0,1);
//    listgraph.addEdge(0,3);
//    listgraph.addEdge(0,5);
//    listgraph.addEdge(1,2);
//    listgraph.addEdge(1,4);
//    listgraph.addEdge(2,3);
//    listgraph.addEdge(2,5);
//    listgraph.addEdge(3,4);
//    listgraph.addEdge(4,5);
//    ui->textEdit->append(QString(listgraph.getAdjacencyList().c_str()));
//    ui->textEdit->append(QString(listgraph.getEdgeTable().c_str()));
//    //isBipartite_BFS(listgraph,actions);
//    isBipartite_DFS_Base(listgraph, actions);
//    ui->textEdit->append(QString(actions.c_str()));


    //_________________________________Word ladder___________________________________

//    vector<string> arr = {"poon", "plee", "same",
//                             "poie", "plie", "poin", "plea"};
//    string start = "toon";
//    string target = "plea";
//    string actions;
//    //ui->textEdit->append("The amount of steps: "+QString::number(wordLadder_Backtrack(start,target,arr,actions)));
//    ui->textEdit->append("The amount of steps: "+QString::number(wordLadder_BFS(start,target,arr,actions)));
//    ui->textEdit->append(QString(actions.c_str()));

    //__________________________________Snakes and laddes_______________________________________

//    ListGraph snek_n_ladder(30, /*false*/-1, ListGraph::Gr_Directed);
//    for(int i = 0; i < 30; i++ )
//    {
//        for(int j = 1; j < 7; j++)
//        {
//            if((i+j)<30)
//            {
//                snek_n_ladder.addEdge(i,i+j);
//            }
//        }
//    }

//    //Drabiny:

//    snek_n_ladder.getConnControl(2).clear();
//    snek_n_ladder.getConnControl(2).insert({21,1});
//    snek_n_ladder.getConnControl(4).clear();
//    snek_n_ladder.getConnControl(4).insert({7,1});
//    snek_n_ladder.getConnControl(10).clear();
//    snek_n_ladder.getConnControl(10).insert({25,1});
//    snek_n_ladder.getConnControl(19).clear();
//    snek_n_ladder.getConnControl(19).insert({28,1});

//    //Zmeji

//    snek_n_ladder.getConnControl(26).clear();
//    snek_n_ladder.getConnControl(26).insert({0,1});
//    snek_n_ladder.getConnControl(20).clear();
//    snek_n_ladder.getConnControl(20).insert({8,1});
//    snek_n_ladder.getConnControl(18).clear();
//    snek_n_ladder.getConnControl(18).insert({6,1});
//    snek_n_ladder.getConnControl(16).clear();
//    snek_n_ladder.getConnControl(16).insert({3,1});
//    //ui->textEdit->append(QString(snek_n_ladder.getAdjacencyList().c_str()));
//    //ui->textEdit->append(QString(snek_n_ladder.getEdgeTable().c_str()));
//    string actions;
//    ui->textEdit->append("The amount of rolls: "+ QString::number(
//                             SnakesNLadders_minDiceThrow_DFS(
//                                 snek_n_ladder,actions)));
//    ui->textEdit->append(QString(actions.c_str()));
//    //ui->textEdit->append(QString(snek_n_ladder.getValueTable().c_str()));

//_______________________________Pouring Jugs___________________________________

//    int m =5, n = 4, d = 2;
//    vector<vector<int>> matrix(m+1,vector<int>(n+1,0));
//    model = new MatrixModel(matrix);
//    ui->tableView->setModel(model);
//    std::vector<PlayAction> actions;
//    string str_actions;
//    ui->textEdit->append("Steps to reach ["+QString::number(d)+"] liters in one of jugs is: "+QString::number(waterJigProblem_BFS(m,n,d,str_actions,actions)));
//    ui->textEdit->append(QString(str_actions.c_str()));
//    model->setActions(actions);
//    connect(ui->pushButton_play,SIGNAL(clicked(bool)),model,SLOT(startActions()));
//    connect(model,SIGNAL(updateBar(int, int)),this,SLOT(setProgressBar(int,int)));

//_______________________________Pacific-Atlantic waterflow___________________________


//    vector<vector<int>> matrix({{1,2,2,3,5},
//                                {3,2,3,4,4},
//                                {2,4,5,3,1},
//                                {6,7,1,4,5},
//                                {5,1,1,2,4}});
//    Vector2D<Atl_Pac_Node> pro_matrix(matrix.size(),matrix.at(0).size());
//    for(int i = 0; i<pro_matrix.rowCount();i++)
//    {
//        for(int j=0;j<pro_matrix.colCount();j++)
//        {
//            pro_matrix(i,j)._value_ = matrix.at(i).at(j);
//        }
//    }
//    model = new MatrixModel(matrix);
//    ui->tableView->setModel(model);
//    std::vector<PlayAction> actions;
//    ui->textEdit->append("Coordinates of lands with two way flows ["+QString::number(twoWayWaterFlow(pro_matrix,actions))+"]");
//    model->setActions(actions);
//    connect(ui->pushButton_play,SIGNAL(clicked(bool)),model,SLOT(startActions()));
//    connect(model,SIGNAL(updateBar(int, int)),this,SLOT(setProgressBar(int,int)));

//_________________________________Labyrinth pathfinder___________________________________

//    vector<vector<int>> matrix({{1,0,1,1,1,1,0,1,1,1},
//                                {1,0,1,0,1,1,1,0,1,1},
//                                {1,1,1,0,1,1,0,1,0,1},
//                                {0,0,0,0,1,0,0,0,0,1},
//                                {1,1,1,0,1,1,1,0,1,0},
//                                {1,0,1,1,1,1,0,1,0,0},
//                                {1,0,0,0,0,0,0,0,0,1},
//                                {1,0,1,1,1,1,0,1,1,1},
//                                {1,1,0,0,0,0,1,0,0,1}});
//    Vector2D<LandNode> pro_matrix(matrix.size(),matrix.at(0).size());
//    for(int i = 0; i<pro_matrix.rowCount();i++)
//    {
//        for(int j = 0; j<pro_matrix.colCount();j++)
//        {
//            pro_matrix(i,j)._land_type_ = matrix.at(i).at(j);
//        }
//    }
//    model = new MatrixModel(matrix);
//    ui->tableView->setModel(model);
//    pair<int,int> start = std::make_pair(0,0);
//    pair<int,int> finish = std::make_pair(3,4);
//    std::vector<PlayAction> actions;
//    ui->textEdit->append("Steps in labyrinth to reach goal ["+
//                         QString::number(/*shortPathLength_DFS*/shortPathLength_BFS(pro_matrix,
//                                                             start.first,start.second,
//                                                             finish.first,finish.second,
//                                                             actions))+"]");
//    model->setActions(actions);
//    connect(ui->pushButton_play,SIGNAL(clicked(bool)),model,SLOT(startActions()));
//    connect(model,SIGNAL(updateBar(int, int)),this,SLOT(setProgressBar(int,int)));

//_________________________________Clone graph_______________________________________________

    Snode * basic = new Snode(); basic->_value_ = 0;
    Snode * child1 = new Snode(); child1->_value_ = 1;
    Snode * child2 = new Snode(); child2->_value_ = 2;
    Snode * child3 = new Snode(); child3->_value_ = 3;
    basic->_connections_ = {child1, child2};
    child1->_connections_ = {basic,child2};
    child2->_connections_ = {basic, child1, child3};
    child3->_connections_ = {child2};

    Snode * compact = new Snode(); compact->_value_ = 0;
    Snode * compact1 = new Snode(); compact1->_value_ = 1;
    Snode * compact2 = new Snode(); compact2->_value_ = 2;
    compact->_connections_ = {compact1, compact2};
    compact1->_connections_ = {compact};
    compact2->_connections_ = {compact};

    Snode * clone = nullptr;
    string actions;
    clone = cloneGraph_BFS(basic,actions);
    std::unordered_map<Snode*,Snode*> visited;
    ui->textEdit->append(QString(actions.c_str()));
    ui->textEdit->append("Is \"basic\" graph equial to \"clone\" graph? :"
                         +QString(compareSnodeGraphs(basic,clone,visited)?"True":"False"));
    ui->textEdit->append("Is \"compact\" graph equial to \"clone\" graph? :"
                         +QString(compareSnodeGraphs(compact,clone,visited)?"True":"False"));

    delete basic; delete child1; delete child2; delete child3;
    delete compact; delete compact1; delete compact2;
}
void MainWindow::setProgressBar(int val, int max)
{
    ui->progressBar->setValue(val*100/max);
}
MainWindow::~MainWindow()
{
    delete ui;
}

