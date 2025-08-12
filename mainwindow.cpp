#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),touchform(new TouchForm())
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    touchform->paintState(11);
    setAttribute(Qt::WA_AcceptTouchEvents, true);
    connect(ui->pushButton_exe,&QPushButton::clicked,this,&MainWindow::execute);
    grabGesture(Qt::TapGesture);
    grabGesture(Qt::TapAndHoldGesture);
    grabGesture(Qt::PanGesture);
    grabGesture(Qt::PinchGesture);
    grabGesture(Qt::SwipeGesture);
    connect(ui->Button_OpenTouchForm,&QPushButton::clicked,touchform,&QWidget::show);
    //connect(ui->Button_OpenTouchForm,&QPushButton::clicked,touchform,&TouchForm::wgt_show);
    ui->line_MouseStatus->setAlignment(Qt::AlignCenter);    ui->line_MouseStatus->setText("(mouse actions require)");
    ui->line_ButtonsPressed->setAlignment(Qt::AlignCenter); ui->line_ButtonsPressed->setText("(mouse actions require)");
    ui->line_LocalX->setAlignment(Qt::AlignCenter);         ui->line_LocalX->setText("(mouse actions require)");
    ui->line_LocalY->setAlignment(Qt::AlignCenter);         ui->line_LocalY->setText("(mouse actions require)");
    ui->line_GlobalX->setAlignment(Qt::AlignCenter);        ui->line_GlobalX->setText("(mouse actions require)");
    ui->line_GlobalY->setAlignment(Qt::AlignCenter);        ui->line_GlobalY->setText("(mouse actions require)");
    ui->line_Modifiers->setAlignment(Qt::AlignCenter);      ui->line_Modifiers->setText("(mouse actions require)");
    ui->line_WheelAngle->setAlignment(Qt::AlignCenter);     ui->line_WheelAngle->setText("(wheel actions require)");
    ui->line_WindowWidth->setAlignment(Qt::AlignCenter);    ui->line_WindowWidth->setText("(window resize require)");
    ui->line_WindowHeight->setAlignment(Qt::AlignCenter);   ui->line_WindowHeight->setText("(window resize require)");
    ui->line_MouseStatus->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->line_ButtonsPressed->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->line_LocalX->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->line_LocalY->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->line_GlobalX->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->line_GlobalY->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->line_Modifiers->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->line_WheelAngle->setAttribute(Qt::WA_TransparentForMouseEvents);
    //ui->tableView->setAttribute(Qt::WA_TransparentForMouseEvents);
    //ui->textEdit->setAttribute(Qt::WA_TransparentForMouseEvents);

    //___________________________________Matrix graph__________________________

//    graph(4),l_graph(3);
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

//    Snode * basic = new Snode(); basic->_value_ = 0;
//    Snode * child1 = new Snode(); child1->_value_ = 1;
//    Snode * child2 = new Snode(); child2->_value_ = 2;
//    Snode * child3 = new Snode(); child3->_value_ = 3;
//    basic->_connections_ = {child1, child2};
//    child1->_connections_ = {basic,child2};
//    child2->_connections_ = {basic, child1, child3};
//    child3->_connections_ = {child2};

//    Snode * compact = new Snode(); compact->_value_ = 0;
//    Snode * compact1 = new Snode(); compact1->_value_ = 1;
//    Snode * compact2 = new Snode(); compact2->_value_ = 2;
//    compact->_connections_ = {compact1, compact2};
//    compact1->_connections_ = {compact};
//    compact2->_connections_ = {compact};

//    Snode * clone = nullptr;
//    string actions;
//    clone = cloneGraph_DFS(basic,actions);
////    clone = cloneGraph_BFS(basic,actions);
//    std::unordered_map<Snode*,Snode*> visited;
//    ui->textEdit->append(QString(actions.c_str()));
//    ui->textEdit->append("Is \"basic\" graph equial to \"clone\" graph? :"
//                         +QString(compareSnodeGraphs(basic,clone,visited)?"True":"False"));
//    ui->textEdit->append("Is \"compact\" graph equial to \"clone\" graph? :"
//                         +QString(compareSnodeGraphs(compact,clone,visited)?"True":"False"));

//    delete basic; delete child1; delete child2; delete child3;
//    delete compact; delete compact1; delete compact2;


//_______________________________Node-based custom graph test_____________________________________

//    NodeGraph graph(NodeGraph::Gr_Directed|NodeGraph::Gr_SortingById);
//    graph.addVertex(0,4);
//    graph.addVertex(1,3);
//    graph.addVertex(2,7);
//    graph.addVertex(3,5);
//    graph.addVertex(4,8);
//    graph.addVertex(5,9);
//    graph.addVertex(6,13);
//    graph.addVertex(7,11);
//    graph.addVertex(9,6);

//    graph.addEdge(0,1);
//    graph.addEdge(0,3);
//    graph.addEdge(1,2);
//    graph.addEdge(1,4);
//    graph.addEdge(2,4);
//    graph.addEdge(3,5);
//    graph.addEdge(5,6);
//    graph.addEdge(5,9);
//    graph.addEdge(6,7);
//    graph.addEdge(7,9);
//    graph.addEdge(9,0);

//    graph.setCoreVertex(0);
//    NodeIterator main_iter(graph);
//    ui->textEdit->append("Current segment count: " + QString::number(graph.segmentCount()));
//    ui->textEdit->append(QString("Main iterator is ")+(main_iter.isValid()?"Valid":"Invalid"));
//    ui->textEdit->append(QString(graph.graphReport().c_str()));
//    cur_id_type new_id = graph.findFreeID();
//    ui->textEdit->append("New element id:"+QString::number(new_id));
//    graph.addVertex(new_id,0);
//    graph.removeEdge(7,9);
//    graph.addEdge(7,new_id);
//    graph.addEdge(new_id,9);
//    ui->textEdit->append(QString(graph.graphReport().c_str()));
//    graph.removeVertex(0);
//    ui->textEdit->append(QString("Main iterator is ")+(main_iter.isValid()?"Valid":"Invalid"));
//    ui->textEdit->append("Current segment count: " + QString::number(graph.segmentCount()));
//    ui->textEdit->append(QString(graph.graphReport().c_str()));
//    vector<NodeGraph> graphs(graph.getSubGraphs());
//    graph.clear();
//    ui->textEdit->append(QString(graph.graphReport().c_str()));
//    for(size_t i = 0; i < graphs.size();i++)
//    {

//        ui->textEdit->append("Current subgraph N:"+QString::number(i+1)+
//                             ", subgraphs count: "+
//                             QString::number(graphs.at(i).segmentCount()));
//        cur_id_type new_core_id = graphs.at(i).findCoreId();
//        ui->textEdit->append("New core id for subgraph: "+QString::number(new_core_id));
//        if(new_core_id>=0)
//        {
//            graphs.at(i).setCoreVertex(new_core_id);
//        }
//        ui->textEdit->append(QString(graphs.at(i).graphReport().c_str()));

//    }

//______________________________________transitive graph closure_________________________________

//    TCGraph gr(4);
//    gr.addEdge(0,1);
//    gr.addEdge(0,2);
//    gr.addEdge(1,2);
//    gr.addEdge(2,0);
//    gr.addEdge(2,3);
//    //gr.addEdge(3,3);
//    string actions;
//    gr.transitiveClosure(actions);
//    ui->textEdit->append(QString(actions.c_str()));

//______________________________________searching for cycles______________________________________

//    ListGraph list_graph(ListGraph::Gr_Directed);
//    list_graph.addVertex();     //0
//    list_graph.addVertex();     //1
//    list_graph.addVertex();     //2
//    list_graph.addVertex();     //3

//    list_graph.addEdge(0,1);
//    list_graph.addEdge(0,2);
//    list_graph.addEdge(1,2);
//    list_graph.addEdge(2,0);
//    list_graph.addEdge(2,3);
//    string actions;
//    ui->textEdit->append(QString(list_graph.getAdjacencyList().c_str()));
//    ui->textEdit->append("Current graph "+QString(hasCycles_DFS(list_graph,actions)?"has a cycle;":"has no cycles"));
//    ui->textEdit->append(QString(actions.c_str()));
//    ui->textEdit->append("Current graph "+QString(hasCycle_BFS_Kahn_alg(list_graph,actions)?"has a cycle;":"has no cycles"));
//    ui->textEdit->append(QString(actions.c_str()));


//    ListGraph NC_l_graph(ListGraph::Gr_Directed);
//    NC_l_graph.addVertex();
//    NC_l_graph.addVertex();
//    NC_l_graph.addVertex();
//    NC_l_graph.addVertex();
//    NC_l_graph.addVertex();
//    NC_l_graph.addVertex();
//    NC_l_graph.addVertex();

//    NC_l_graph.addEdge(0,1);
//    NC_l_graph.addEdge(0,2);
//    NC_l_graph.addEdge(1,2);
//    NC_l_graph.addEdge(2,3);
//    NC_l_graph.addEdge(3,4);
//    NC_l_graph.addEdge(3,5);
//    NC_l_graph.addEdge(4,5);
//    NC_l_graph.addEdge(5,3);
//    NC_l_graph.addEdge(6,2);
//    string actions;
//    ui->textEdit->append(QString(NC_l_graph.getAdjacencyList().c_str()));
//    //ui->textEdit->append("Current graph "+QString(hasCycles_DFS(NC_l_graph,actions)?"has a cycle;":"has no cycles"));
//    ui->textEdit->append("Current graph "+QString(hasCycle_BFS_Kahn_alg(NC_l_graph,actions)?"has a cycle;":"has no cycles"));
//    ui->textEdit->append(QString(actions.c_str()));


//____________________________________Searching cycles in an undirected graph____________________________________________

//    ListGraph list_graph;
//    list_graph.addVertex(0);
//    list_graph.addVertex(4);
//    list_graph.addVertex(2);
//    list_graph.addVertex(3);
//    list_graph.addVertex(1);

//    list_graph.addEdge(0,1);
//    list_graph.addEdge(0,2);
//    list_graph.addEdge(0,3);
//    list_graph.addEdge(1,2);
//    list_graph.addEdge(3,4);


//    ListGraph n_list_graph;
//    n_list_graph.addVertex();
//    n_list_graph.addVertex();
//    n_list_graph.addVertex();
//    n_list_graph.addVertex();

//    n_list_graph.addEdge(0,1);
//    n_list_graph.addEdge(1,2);
//    n_list_graph.addEdge(2,3);
//    string actions;
//    ui->textEdit->append(QString(list_graph.getAdjacencyList().c_str()));
//    ui->textEdit->append("Current graph "+QString(hasCycle_Undirected_BFS(list_graph,actions)?"has a cycle;":"has no cycles"));
//    ui->textEdit->append(QString(actions.c_str()));
//    actions.clear();
//    ui->textEdit->append(QString(n_list_graph.getAdjacencyList().c_str()));
//    ui->textEdit->append("Current graph "+QString(hasCycle_Undirected_BFS(n_list_graph,actions)?"has a cycle;":"has no cycles"));
//    ui->textEdit->append(QString(actions.c_str()));


//    ListGraph d_list;
//    d_list.addVertex(0);
//    d_list.addVertex(1);
//    d_list.addVertex(2);
//    d_list.addVertex(3);
//    d_list.addVertex(5);

//    d_list.addEdge(0,1);
//    //d_list.addEdge(0,2);
//    d_list.addEdge(0,3);
//    d_list.addEdge(1,2);
//    d_list.addEdge(3,5);

//    string actions;
//    ui->textEdit->append(QString(d_list.getAdjacencyList().c_str()));
//    ui->textEdit->append("Current graph "+QString(hasCycle_Undirected_DFS(d_list,actions)?"has a cycle;":"has no cycles"));
//    ui->textEdit->append(QString(actions.c_str()));

//_______________________________________Detecting cycles using "colour" value +DFS __________________________________________

//    ListGraph list_graph(ListGraph::Gr_Directed);
//    list_graph.addVertex();
//    list_graph.addVertex();
//    list_graph.addVertex();
//    list_graph.addVertex();

//    list_graph.addEdge(0,1);
//    list_graph.addEdge(0,2);
//    list_graph.addEdge(1,2);
//    list_graph.addEdge(2,0);
//    list_graph.addEdge(2,3);

//    string actions;
////    ui->textEdit->append(QString(list_graph.getAdjacencyList().c_str()));
////    ui->textEdit->append("Current graph "+QString(hasCycle_Colour(list_graph,actions)?"has a cycle;":"has no cycles"));
////    ui->textEdit->append(QString(actions.c_str()));

//    actions.clear();
//    ListGraph s_graph(ListGraph::Gr_Directed);
//    s_graph.addVertex();
//    s_graph.addVertex();
//    s_graph.addVertex();
//    s_graph.addVertex();
//    s_graph.addVertex();

//    s_graph.addEdge(0,1);
//    s_graph.addEdge(0,2);
//    s_graph.addEdge(1,2);
//    s_graph.addEdge(2,3);
//    s_graph.addEdge(4,2);
//    ui->textEdit->append(QString(s_graph.getAdjacencyList().c_str()));
//    ui->textEdit->append("Current graph "+QString(hasCycle_Colour(s_graph,actions)?"has a cycle;":"has no cycles"));
//    ui->textEdit->append(QString(actions.c_str()));

//_____________________________________Finding negative cycles________________________________________________________

//    ListGraph list_graph(ListGraph::Gr_Directed|ListGraph::Gr_Weighted);
//    list_graph.addVertex();
//    list_graph.addVertex();
//    list_graph.addVertex();
//    list_graph.addVertex();
//    list_graph.addVertex();

//    list_graph.addEdge(0,1,-1);
//    list_graph.addEdge(0,2,4);
//    list_graph.addEdge(1,2,3);
//    list_graph.addEdge(1,3,2);
//    list_graph.addEdge(1,4,2);
//    list_graph.addEdge(3,2,5);
//    list_graph.addEdge(3,1,1);
//    list_graph.addEdge(4,3,-3);
//    string actions;
//    ui->textEdit->append(QString(list_graph.getAdjacencyList().c_str()));
//    ui->textEdit->append("Current graph" + QString(hasNegCycle(list_graph,0,actions)?" has negative cycle;":" has no neg cycles;"));
//    ui->textEdit->append(QString(actions.c_str()));

//    ListGraph n_graph(ListGraph::Gr_Directed|ListGraph::Gr_Weighted);
//    n_graph.addVertex();
//    n_graph.addVertex();
//    n_graph.addVertex();
//    n_graph.addVertex();

//    n_graph.addEdge(0,1,1);
//    n_graph.addEdge(0,2,4);
//    n_graph.addEdge(1,2,-3);
//    n_graph.addEdge(1,3,2);
//    n_graph.addEdge(2,3,3);
//    n_graph.addEdge(3,1,-5);
//    string actions;
//    ui->textEdit->append(QString(n_graph.getAdjacencyList().c_str()));
//    ui->textEdit->append("Current graph" + QString(hasNegCycle(n_graph,0,actions)?" has negative cycle;":" has no neg cycles;"));
//    ui->textEdit->append(QString(actions.c_str()));

    loadGraphicScreen();
    execute();
}

void MainWindow::execute()
{
    ui->textEdit->clear();
    //_________________________________________________Searching for n-sized cycles in directed graph_________________________________

        vector<vector<bool>> graph({{0,1,0,1,0},
                                    {1,0,1,0,1},
                                    {0,1,0,1,0},
                                    {1,0,1,0,1},
                                    {0,1,0,1,0}});
        Vector2D<bool> pro_graph(graph);
        int cyc_length = 4;
        string actions;
        ui->textEdit->append("Current graph has [" +
                             QString::number(
                                 n_sizeCyclesSearch(pro_graph,cyc_length,actions)
                                 )+"] "+QString::number(cyc_length)+"-length cycles;");
        ui->textEdit->append(QString(actions.c_str()));


    return;
}

void MainWindow::loadGraphicScreen()
{
    QGraphicsScene* scene = new QGraphicsScene(QRectF(-100,-100,300,300));

    QGraphicsRectItem* rect_graphic =
            scene->addRect(
                QRectF(30,30,120,80),
                QPen(Qt::black),
                QBrush(Qt::blue));
    rect_graphic->setFlags(QGraphicsItem::ItemIsMovable);
    QGraphicsPixmapItem* pixmap_graphic =
            scene->addPixmap(QPixmap("tiny_dragon.png"));
    pixmap_graphic->setFlags(QGraphicsItem::ItemIsMovable);
    QGraphicsTextItem* text_graphic =
            scene->addText("Гэты тэкст магчыма цягаць і рухаць мышам!");
    text_graphic->setFlags(QGraphicsItem::ItemIsMovable);

    QGraphicsLineItem* line_graphic =
            scene->addLine(QLineF(10,10,80,80),QPen(Qt::red,2));
    line_graphic->setTransform(QTransform().shear(20.5,7.0),true);
    line_graphic->setFlags(QGraphicsItem::ItemIsMovable);
    text_graphic->setParentItem(rect_graphic);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->show();
}

void MainWindow::setProgressBar(int val, int max)
{
    ui->progressBar->setValue(val*100/max);
}
MainWindow::~MainWindow()
{
    if(touchform)
    {
        touchform->close();
        delete touchform;
    }
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent* pe)
{
    switch(pe->key())
    {
    case Qt::Key_Q:
    {
        if(pe->modifiers()&Qt::ControlModifier)
        {
            this->close();
        }
        else
        {
            QMainWindow::keyPressEvent(pe);
        }
    }
        break;
    case Qt::Key_E:
    {
        if(pe->modifiers()&Qt::ControlModifier)
        {
            execute();
        }
        else
        {
            QMainWindow::keyPressEvent(pe);
        }
    }
        break;
    default:
    {
        QMainWindow::keyPressEvent(pe);
    }
    }
    return;
}

void MainWindow::mousePressEvent(QMouseEvent* m_event)
{
    eventQueue(m_event,"Mouse button pressed");
    return;
}

void MainWindow::mouseReleaseEvent(QMouseEvent* m_event)
{
    eventQueue(m_event,"Mouse button released");
    return;
}

void MainWindow::mouseMoveEvent(QMouseEvent* m_event)
{
    eventQueue(m_event,"Mouse is moving");
    return;
}

void MainWindow::eventQueue(QMouseEvent* m_event, const QString& msg)
{
    ui->line_MouseStatus->setText(msg);
    ui->line_ButtonsPressed->setText(buttonsInfo(m_event));
    ui->line_LocalX->setText(QString::number(m_event->position().x()));
    ui->line_LocalY->setText(QString::number(m_event->position().y()));
    ui->line_GlobalX->setText(QString::number(m_event->globalPosition().x()));
    ui->line_GlobalY->setText(QString::number(m_event->globalPosition().y()));
    ui->line_Modifiers->setText(modifiersInfo(m_event));
    m_event->accept();
    return;
}

QString MainWindow::modifiersInfo(QMouseEvent* m_event)
{
    QString answer;
    if(m_event->modifiers()&Qt::ShiftModifier)
    {
        answer.append("Shift ");
    }
    if(m_event->modifiers()&Qt::ControlModifier)
    {
        answer.append("Ctrl ");
    }
    if(m_event->modifiers()&Qt::AltModifier)
    {
        answer.append("Alt");
    }
    return answer;
}

QString MainWindow::buttonsInfo(QMouseEvent* m_event)
{
    QString answer;
    if(m_event->buttons()&Qt::LeftButton)
    {
        answer.append("Left ");
    }
    if(m_event->buttons()&Qt::RightButton)
    {
        answer.append("Right ");
    }
    if(m_event->buttons()&Qt::MiddleButton)
    {
        answer.append("Middle");
    }
    return answer;
}

void MainWindow::wheelEvent(QWheelEvent* w_event)
{
    QPoint wheel_angle = w_event->angleDelta();
    QPoint touchpad_pixels = w_event->pixelDelta();
    if(!wheel_angle.isNull())
    {
        ui->line_WheelAngle->setText(QString::number(wheel_angle.y()));
    }
    else if(!touchpad_pixels.isNull())
    {
        ui->line_WheelAngle->setText(QString::number(touchpad_pixels.y()));
    }
    w_event->accept();
    return;
}

void MainWindow::enterEvent(QEnterEvent* e_event)
{
    ui->label_EnterLeave->setText("Mouse entered window");
    e_event->accept();
}
void MainWindow::leaveEvent(QEvent* l_event)
{
    ui->label_EnterLeave->setText("Mouse left window");
    l_event->accept();
}

void MainWindow::closeEvent(QCloseEvent*  cl_event)
{
    QMessageBox mbx;
    mbx.setText("Вы ўпэўнены што жадаеце закрыць акно?");
    mbx.setInformativeText("Тут немагчымы зьмяненьні!");
    mbx.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
    mbx.setDefaultButton(QMessageBox::Yes);
    mbx.setIcon(QMessageBox::Critical);
    int vynik =mbx.exec();
    switch(vynik)
    {
        case QMessageBox::Yes:
    {
        if(touchform)
        {
            touchform->close();
        }
        cl_event->accept();

    }
        break;
    case QMessageBox::No:
    {
        cl_event->ignore();
    }
    }
    return;
}


void MainWindow::resizeEvent(QResizeEvent* rsz_event)
{
    ui->line_WindowWidth->setText(QString::number(rsz_event->size().width()));
    ui->line_WindowHeight->setText(QString::number(rsz_event->size().height()));
    return;
}

bool MainWindow::event(QEvent* reg_event)
{
//    qDebug()<<"Event type: " << reg_event->type();
    if(reg_event->type()==QEvent::Gesture)
    {
        return gestureEvent(dynamic_cast<QGestureEvent*>(reg_event));
    }
    return QWidget::event(reg_event);
}

bool MainWindow::gestureEvent(QGestureEvent* g_event)
{
    if(QGesture* swipe = g_event->gesture(Qt::SwipeGesture))
    {
        ui->label_gesture_event->setText("Swipe gesture");
        swipeGesture(static_cast<QSwipeGesture*>(swipe));
    }
    else if(QGesture* pan = g_event->gesture(Qt::PanGesture))
    {
        ui->label_gesture_event->setText("Swipe gesture");
        panGesture(static_cast<QPanGesture*>(pan));
    }
    else if(QGesture* pinch = g_event->gesture(Qt::PinchGesture))
    {
        ui->label_gesture_event->setText("Swipe gesture");
        pinchGesture(static_cast<QPinchGesture*>(pinch));
    }
    else if(QGesture* tapNhold = g_event->gesture(Qt::TapAndHoldGesture))
    {
        ui->label_gesture_event->setText("Swipe gesture");
        tapNholdGesture(static_cast<QTapAndHoldGesture*>(tapNhold));
    }
    else if(QGesture* tap = g_event->gesture(Qt::TapGesture))
    {
        ui->label_gesture_event->setText("Swipe gesture");
        tapGesture(static_cast<QTapGesture*>(tap));
    }
    g_event->accept();
    return true;
}


void MainWindow::swipeGesture(QSwipeGesture* sw_gesture)
{
    if(sw_gesture->state()==Qt::GestureFinished)
    {
        QString dir("Direction: ");
        if(sw_gesture->horizontalDirection()==QSwipeGesture::Left)
        {
            dir.append("Left ");
        }
        else if(sw_gesture->horizontalDirection()==QSwipeGesture::Right)
        {
            dir.append("Right ");
        }
        if(sw_gesture->verticalDirection()==QSwipeGesture::Up)
        {
            dir.append("Up ");
        }
        else if(sw_gesture->verticalDirection()==QSwipeGesture::Down)
        {
            dir.append("Down ");
        }
        dir.append(QString::number(sw_gesture->swipeAngle()));
        ui->label_gesture_type->setText(dir);
        update();
    }
    return;
}
void MainWindow::panGesture(QPanGesture* p_gesture)
{
    QString dir("Acseleration: ");
    dir.append(QString::number(p_gesture->acceleration())+";\n");
    dir.append("Previous point offset: x="+
               QString::number(p_gesture->offset().x())+
               " y="+
               QString::number(p_gesture->offset().y())+";\n");
    dir.append("Last point offset: x="+
               QString::number(p_gesture->lastOffset().x())+
               " y="+
               QString::number(p_gesture->lastOffset().y())+";\n");
    dir.append("Delta: x="+
               QString::number(p_gesture->delta().x())+" y="+
               QString::number(p_gesture->delta().y())+";\n");
    ui->label_gesture_type->setText(dir);
    update();
}
void MainWindow::pinchGesture(QPinchGesture* p_gesture)
{
    QString dir;
    if(p_gesture->changeFlags()==QPinchGesture::ScaleFactorChanged)
    {
        dir.append("Scale factor: "+QString::number(p_gesture->scaleFactor())+";\n");
    }
    else if(p_gesture->changeFlags()==QPinchGesture::RotationAngleChanged)
    {
        dir.append("Rotation angle: "+QString::number(p_gesture->rotationAngle())+";\n");
    }
    else if(p_gesture->changeFlags()==QPinchGesture::CenterPointChanged)
    {
        dir.append("Central point new coordinates: x="+
                   QString::number(p_gesture->centerPoint().x())+
                   " y="+QString::number(p_gesture->centerPoint().y())+";\n");
    }
    ui->label_gesture_type->setText(dir);
    update();
}
void MainWindow::tapNholdGesture(QTapAndHoldGesture* tnh_gesture)
{
    QString dir;
    dir.append("Tap and hold triggered after: "+
               QString::number(tnh_gesture->timeout())+
               " milliseconds;\n");
    ui->label_gesture_type->setText(dir);
    update();
}
void MainWindow::tapGesture(QTapGesture* t_gesture)
{
    QString dir;
    dir.append("Tap was made at: x="+
               QString::number(t_gesture->position().x())+
               " y="+
               QString::number(t_gesture->position().y())+";\n");
    ui->label_gesture_type->setText(dir);
    update();
}
