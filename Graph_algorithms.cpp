#include "Graph_algorithms.h"
#include "matrixmodel.h"

std::string Breadth_first_search(ListGraph& obj, int root_index)            //Паслядоўнасць "Пошук ў шырыню"
{                                                                           //Адлюстроўвае ўсе выключныя вузы графа.
    std::string answer;                                                     //Радок адказу
    std::queue<int> BFS_queue;                                              //Чарга "па-узроўневага" праходу праз граф
    std::unordered_set<int> visited_vertices;                               //Ненакіраваны набор значэнняў, захоўвае ўжо наведаныя вузы графу
//    std::map<int,GraphVertice>& cur_graph = obj._list_graph_;               //Выцягваем кантэйнер графа з аб'екта графа-спіса
//    auto it = cur_graph.find(root_index);                                   //Шукаем пачатковую вузу, з якой пачнем абыход
//    if(it==cur_graph.end())
//    {
//        return answer;
//    }
    if(!obj.isExists(root_index))
    {
        return answer;
    }
    answer.append("Breadth first search (list-based graph):\n");
    answer.append("Root index: "+std::to_string(root_index)+";\n");
    //visited_vertices.insert(it->first);                                     //Дадаем каранёвую вузу ў "наведаныя"
    visited_vertices.insert(root_index);
    BFS_queue.push(root_index);                                             //Уцісківаем каранувую вузу ва ў чаргу
    while(!BFS_queue.empty())                                               //Пакуль у чарзе застаюцца "ненаведаныя" вузы,
    {                                                                       //Мы наведваем ды адзначаем іх
        int cur_index = BFS_queue.front();                                  //Выцягваем індэкс бягучай вузы з чаргі
        BFS_queue.pop();                                                    //Выдаляем індэкс з чаргі
//      auto connected_indexes = cur_graph.at(cur_index)._edges_.begin();   //Ствараем ітэратар са сьпісу ўсіх злучаных вузаў графа.
        answer.append("Visited["+std::to_string(cur_index)+"], ");          //Адзначаем ў адказе індэкс наведанай вузы
//        while(connected_indexes!=cur_graph.at(cur_index)._edges_.end())
//        {
//            if(visited_vertices.find(connected_indexes->first)==visited_vertices.end())
//            {                                                               //Калі гэты індэкс не сустракаўся раней(не знаходзіцца ў наборы наведаных)
//                visited_vertices.insert(connected_indexes->first);          //Дадаем яго ў набор наведаных
//                BFS_queue.push(connected_indexes->first);                   //Уціскваем яго ў канец чаргі
//            }
//            connected_indexes++;                                            //Наступная злучаная вуза.
//        }

        auto connections = obj.getEdges(cur_index);
        for(int edge: connections)
        {
            if(visited_vertices.find(edge)==visited_vertices.end())
            {
                visited_vertices.insert(edge);
                BFS_queue.push((edge));
            }
        }
    }
    answer.append("\nSearch finished.");
    return answer;
    //У сутнасьці мы ствараем сьпіс усіх унікальных элементаў графа.
}

std::string Breadth_first_search(VectorGraph& obj, int root_index)
{
    std::string answer;
    std::queue<int> BFS_queue;
    std::unordered_set<int> visited_vertices;
    std::vector<std::vector<cur_type>>& graph_matrix = obj._adj_matrix_;
    int matrix_size = static_cast<int>(graph_matrix.size());
    if(matrix_size<=root_index) return answer;

    answer.append("Breadth first search (matrix-based graph):\n");
    answer.append("Root index: "+std::to_string(root_index)+";\n");

    visited_vertices.insert(root_index);
    BFS_queue.push(root_index);
    while(!BFS_queue.empty())
    {
        int cur_index = BFS_queue.front();
        BFS_queue.pop();
        std::vector<cur_type>& node = graph_matrix.at(cur_index);
        int node_size = static_cast<int>(node.size());
        answer.append("Visited["+std::to_string(cur_index)+"], ");
        for(int i = 0; i<node_size;i++)
        {
            if(node.at(i))
            {
                if(visited_vertices.find(i)==visited_vertices.end())
                {
                    visited_vertices.insert(i);
                    BFS_queue.push(i);
                }
            }
        }
    }
    return answer;
}

namespace GrAlg
{
    //Рэкурсіўная (стэкавая) функцыя для глыбіннага пошуку
    std::string Deep_search(ListGraph& obj,//std::map<int,GraphVertice>& list_graph,
                            std::unordered_set<int>& visited_vertices,
                            int cur_index)
    {
        std::string answer;                                                     //Радок адказу
        visited_vertices.insert(cur_index);                                     //Дадаем бягучую вяршыню ў набор наведанных вяршынь
        answer.append(std::to_string(cur_index)+", ");                          //Дадаем у адказ бягучую вершыню.
//        auto conn = list_graph.at(cur_index)._edges_.begin();                   //Наведваем кожную злучаную вершыню
//        while(conn != list_graph.at(cur_index)._edges_.end())                   //са сьпісу сувязей
//        {
//            if(visited_vertices.find(conn->first)==visited_vertices.end())      //Калі гэта вяршыня не была наведана раней
//            {
//                answer.append("| ("+std::to_string(cur_index)+")-["+            //дадаем у адказ індэкс бягучай вяршыні,
//                              std::to_string(conn->second)+"]->"+               //вагу сувязі паміж вяршынямі,
//                              Deep_search(list_graph,visited_vertices,conn->first));    //і адказ рэкурсіўнага выкліку са злучанай
//            }                                                                   //вяршыняй у якасьці бягучага індэксу.
//            conn++;
//        }
        auto connections = obj.getEdges(cur_index);
        for(int edge:connections)
        {
            if(visited_vertices.find(edge)==visited_vertices.end())
            {
                answer.append("| ("+std::to_string(cur_index)+")-["+
                                              std::to_string(obj.edgeWeightAt(cur_index,edge))+"]->"+
                                              Deep_search(/*list_graph*/obj,visited_vertices,/*conn->first*/edge));
            }
        }
        return answer;
    }

    std::string Deep_search(std::vector<std::vector<int>>& adj_matrix,
                            std::unordered_set<int>& visited_vertices,
                            int cur_index)
    {
        std::string answer;
        visited_vertices.insert(cur_index);
        answer.append(std::to_string(cur_index)+", ");
        int matrix_size = static_cast<int>(adj_matrix.size());
        for(int i = cur_index; i<matrix_size;i++)
        {
            if(adj_matrix.at(cur_index).at(i))
            {
                if(visited_vertices.find(i)==visited_vertices.end())
                {
                    answer.append("("+std::to_string(cur_index)+")-["+
                                  std::to_string(adj_matrix.at(cur_index).at(i))+
                                  "]->"+Deep_search(adj_matrix,visited_vertices,i));
                }
            }
        }
        return answer;
    }

}
//Глыбінны пошук усіх вяршынь графа. У адрозьненні ад пошука ў шырыню,
//як толькі алгарытм знаходзіць сувязь з наступнай вяршыняй, ён пераскоквае
//да яе і пачынае шукаць яе сувязі з наступнымі вяршынямі. Гэты алгарытм працуе
//як пошук у бінарным дрэве.
std::string Depth_first_search(ListGraph& obj, int root_index)
{
    std::string answer("Depth First Search (list-based graph):\nRoot index: "+
                       std::to_string(root_index)+";\n");
    if(obj._list_graph_.find(root_index)==obj._list_graph_.end())
    {
        answer.append("Index ["+std::to_string(root_index)+"] doesn't exist in the current graph.");
        return answer;
    }
    std::unordered_set<int> visited_vertices;
    answer.append(GrAlg::Deep_search(obj/*._list_graph_*/,visited_vertices,root_index));
    answer.append("\nDFS end;");
    return answer;
}

std::string Depth_first_search(VectorGraph& obj, int root_index)
{
    std::string answer("Depth First Search (matrix-based graph):\nRoot index: "+
                       std::to_string(root_index)+";\n");
    int matrix_size= static_cast<int>(obj._adj_matrix_.size());
    if(root_index>=matrix_size)
    {
        answer.append("Index ["+std::to_string(root_index)+"] doesn't exist in the current graph.");
        return answer;
    }
    std::unordered_set<int> visited_vertices;
    answer.append(GrAlg::Deep_search(obj._adj_matrix_,visited_vertices,root_index));
    answer.append("\nDFS end;");
    return answer;
}


//___________________________________________________________



bool szBordCheck(int cur_row, int cur_col, int rows_size, int cols_size)
{
    return (0<=cur_row && cur_row<rows_size && 0<=cur_col && cur_col < cols_size);
}


int orangesRotting_iteration(std::vector<std::vector<int>>& matrix, long long &time)
{
    if(!matrix.size())
    {
        return -2;
    }
    int row_size = static_cast<int>(matrix.size());
    int col_size = static_cast<int>(matrix.at(0).size());
    bool changed = false;
    using Clock = std::chrono::high_resolution_clock;
    auto t0 = Clock::now();
    int elapsed_time = 0;
    std::vector<std::vector<int>> directions({{1,0},{0,1},{-1,0},{0,-1}});
    while(true)
    {
        changed = false;
        for(int i = 0; i<row_size;i++)
        {
            for(int j = 0; j<col_size;j++)
            {
                if(matrix.at(i).at(j)==elapsed_time+2)
                {
                    for(const auto&dir: directions)
                    {
                        int row_x = i+dir[0];
                        int col_y = j+dir[1];
                        if(szBordCheck(row_x,col_y,row_size,col_size)&&
                                matrix.at(row_x).at(col_y)==1)
                        {
                            matrix.at(row_x).at(col_y) = matrix.at(i).at(j)+1;
                            changed = true;
                        }
                    }
                }
            }
        }
        if(!changed) break;

        elapsed_time++;
    }

    for(int i = 0; i<row_size;i++)
    {
        for(int j = 0; j<col_size;j++)
        {
            if(matrix.at(i).at(j)==1)
            {
                return -1;
            }
        }
    }
    auto t1 = Clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(t1-t0);
    time = elapsed.count();
    return elapsed_time;//
}

void addAction(std::vector<std::vector<int> > &matrix,
               int active_row, int active_col,
               std::vector<PlayAction>& actions)
{
    PlayAction action;
    action.row = active_row;
    action.column = active_col;
    switch(matrix.at(active_row).at(active_col))
    {
    case -2:
    {
        action.value ="*";
    }
        break;
    case -1:
    {
        action.value ="?";
    }
        break;
    case 0:
    {
        action.value ="_";
    }
        break;
    case 1:
    {
        action.value ="o";
    }
        break;
    case 2:
    {
        action.value ="x";
    }
        break;
    default:
    {
        action.value =("o"+std::to_string(matrix.at(active_row).at(active_col)));
    }
    }
    actions.push_back(action);
    return;
}

void addAction(int active_row, int active_col,
               std::vector<PlayAction>& actions,
               std::string info,
               PlayAction::PlayActionsType actionType,
               bool is_changing, int new_val)
{
    PlayAction action;
    action.row = active_row;
    action.column = active_col;
    action.value =info;
    action.actionType = actionType;
    if(is_changing)
    {
        action.to_change = true;
        action.val_to_change = new_val;
    }
    else
    {
        action.to_change = false;
        action.val_to_change = 0;
    }
    actions.push_back(action);
    return;
}

std::string drawMatrix(std::vector<std::vector<int>>& matrix, int active_row = -1, int active_col=-1)
{
    std::string answer;
    int row_size = static_cast<int>(matrix.size());
    int col_size = static_cast<int>(matrix.at(0).size());

    for(int i = 0; i<row_size;i++)
    {
        for(int j = 0; j<col_size;j++)
        {
            if((i==active_row)&&(j==active_col))
            {
                answer.append("(");
            }
            else
            {
                answer.append("[");
            }
            switch(matrix.at(i).at(j))
            {
            case -2:
            {
                answer.append("*_");
            }
                break;
            case -1:
            {
                answer.append("?_");
            }
                break;
            case 0:
            {
                answer.append("__");
            }
                break;
            case 1:
            {
                answer.append("o_");
            }
                break;
            case 2:
            {
                answer.append("x_");
            }
                break;
            default:
            {
                answer.append("o"+std::to_string(matrix.at(i).at(j)));
            }
            }

            if((i==active_row)&&(j==active_col))
            {
                answer.append(") ");
            }
            else
            {
                answer.append("] ");
            }
        }
        answer.append("\n");
    }
    answer.append("\n");
    return answer;
}

void O_R_DFS(std::vector<std::vector<int>>& matrix, int row, int col, int time,std::string& actions,std::vector<std::vector<int>>& visual_matrix)
{
    int row_size = static_cast<int>(matrix.size());
    int col_size = static_cast<int>(matrix.at(0).size());
    matrix.at(row).at(col) = time;
    visual_matrix.at(row).at(col) = time;
    //actions.append("New elapsed time for row["+std::to_string(row)+"] col["+std::to_string(col)+"] = "+std::to_string(time)+";\n");
    std::vector<std::vector<int>> directions({{1,0},{0,1},{-1,0},{0,-1}});
    for(auto dir:directions)
    {
        int x = row+dir[0];
        int y = col+dir[1];
        //actions.append("Neighbour cell row["+std::to_string(x)+"] col["+std::to_string(y)+"] check for fresh orange;\n");
        if(szBordCheck(x,y,row_size,col_size))
        {
            int prev_val = visual_matrix.at(x).at(y);
            visual_matrix.at(x).at(y) = -1;
            actions.append(drawMatrix(visual_matrix,x,y));
            if(matrix.at(x).at(y)==1)
            {
                //actions.append("Cell row["+std::to_string(x)+"] col["+std::to_string(y)+"] has a fresh orange;\n");
                visual_matrix.at(x).at(y) =  1;
                actions.append(drawMatrix(visual_matrix,x,y));
                O_R_DFS(matrix,x,y,time+1,actions,visual_matrix);
            }
            else if(matrix.at(x).at(y)>time+1)
            {
                visual_matrix.at(x).at(y) = matrix.at(x).at(y);
                actions.append(drawMatrix(visual_matrix,x,y));
//                actions.append("Cell row["+std::to_string(x)+"] col["+std::to_string(y)+
//                               "] has strange elapsed time ["+std::to_string(matrix.at(x).at(y))+
//                               "] in comp to time+1 ["+std::to_string(time+1)+"];\n");
                O_R_DFS(matrix,x,y,time+1,actions,visual_matrix);
            }
            else
            {
                visual_matrix.at(x).at(y) = prev_val;
            }
        }
    }

}

int orangesRotting_DFS(std::vector<std::vector<int>>& matrix, long long &time, std::string& actions)
{
    if(!matrix.size())
    {
        return -2;
    }
    actions.clear();
    int row_size = static_cast<int>(matrix.size());
    int col_size = static_cast<int>(matrix.at(0).size());
    std::vector<std::vector<int>> visual_matrix(row_size,std::vector<int>(col_size,0));
    int elapsed_time = 0;
    using Clock = std::chrono::high_resolution_clock;
    auto t0 = Clock::now();
    for(int i = 0; i<row_size;i++)
    {
        for(int j = 0; j<col_size;j++)
        {
            if(matrix.at(i).at(j)==2)
            {
                //actions.append("Rotten orange found at row["+std::to_string(i)+"] col["+std::to_string(j)+"];\n");
                visual_matrix.at(i).at(j) = 2;
                actions.append(drawMatrix(visual_matrix,i,j));
                O_R_DFS(matrix,i,j,2,actions,visual_matrix);
            }
        }
    }

    for(int i = 0; i<row_size;i++)
    {
        for(int j = 0; j<col_size;j++)
        {
            if(matrix.at(i).at(j)==1)
            {
                return -1;
            }
            elapsed_time = std::max(elapsed_time,matrix.at(i).at(j)-2);
        }
    }
    auto t1 = Clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(t1-t0);
    time = elapsed.count();
    return elapsed_time;
}

using std::vector;

int orangesRotting_BFS(std::vector<std::vector<int>>& matrix,
                       long long& time, std::string& actions,
                       std::vector<PlayAction> &vec_actions)
{
    if(!matrix.size())
    {
        return -2;
    }
    actions.clear();
    vec_actions.clear();
    int row_size = static_cast<int>(matrix.size());
    int col_size = static_cast<int>(matrix.at(0).size());
    std::vector<std::vector<int>> directions({{1,0},{0,1},{-1,0},{0,-1}});
    std::vector<std::vector<int>> visual_matrix(row_size,std::vector<int>(col_size,0));
    using Clock = std::chrono::high_resolution_clock;
    auto t0 = Clock::now();
    std::queue<std::vector<int>> q;
    for(int i = 0; i<row_size;i++)
    {
        for(int j = 0; j<col_size;j++)
        {
            if(matrix.at(i).at(j)==2)
            {
                q.push({i,j});
                visual_matrix.at(i).at(j) = 2;
            }
            else if(matrix.at(i).at(j)==1)
            {
                visual_matrix.at(i).at(j) = 1;
            }
        }
    }
    actions.append(drawMatrix(visual_matrix));
    int elapsed_time = 0;
    while(!q.empty())
    {
        elapsed_time++;
        int len = q.size();
        while(len--)
        {
            vector<int> cur = q.front();
            q.pop();
            int i = cur.at(0);
            int j = cur.at(1);
            int cur_val = visual_matrix.at(i).at(j);
            visual_matrix.at(i).at(j) = -2;
            actions.append(drawMatrix(visual_matrix,i,j));
            addAction(matrix,i,j,vec_actions);
            for(auto dir: directions)
            {
                int x = i+dir.at(0);
                int y = j+dir.at(1);
                if(szBordCheck(x,y,row_size,col_size))
                {
                    int cur_sub_val = visual_matrix.at(x).at(y);
                    visual_matrix.at(x).at(y) = -1;
                    actions.append(drawMatrix(visual_matrix,x,y));
                    addAction(matrix,x,y,vec_actions);
                    if(matrix.at(x).at(y)==1)
                    {
                        matrix.at(x).at(y) = 2;
                        cur_sub_val = 2;
                        q.push({x,y});
                    }
                    visual_matrix.at(x).at(y) = cur_sub_val;
                    actions.append(drawMatrix(visual_matrix,i,j));
                    addAction(matrix,i,j,vec_actions);
                }
            }
            visual_matrix.at(i).at(j) = cur_val;
        }
    }
    for(int i = 0; i<row_size;i++)
    {
        for(int j = 0; j<col_size;j++)
        {
            if(matrix.at(i).at(j)==1)
            {
                return -1;
            }
        }
    }
    actions.append(drawMatrix(visual_matrix));
    auto t1 = Clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(t1-t0);
    time = elapsed.count();
    return std::max(0,elapsed_time-1);
}


//______________________________________________________________________________

bool countIslands_szCheck(Vector2D<LandNode>& matrix,int row, int col)
{
    int row_count = matrix.rowCount();
    int col_count = matrix.colCount();
    bool answer = true;
    answer = answer&&(row>=0);
    answer = answer&&(row<row_count);
    answer = answer&&(col>=0);
    answer = answer&&(col<col_count);
    answer = answer&&(matrix(row,col)._land_type_=='L');
    answer = answer&&(!matrix(row,col)._visited_);
    return answer;
}

void countIslands_DFS(Vector2D<LandNode>& matrix, int row, int col, std::vector<PlayAction> &actions)
{
    vector<std::pair<int,int>> neighbours({
                                              {-1,-1},  {-1,0},  {-1,1},
                                              { 0,-1},           { 0,1},
                                              { 1,-1},  { 1,0},  { 1,1}});
    matrix(row,col)._visited_ = true;
    int neigh_size = static_cast<int>(neighbours.size());
    for(auto neigh = 0; neigh<neigh_size; neigh++)
    {
        int neigh_row = row+neighbours.at(neigh).first;
        int neigh_col = col+neighbours.at(neigh).second;
        if(szBordCheck(neigh_row,neigh_col,matrix.rowCount(),matrix.colCount()))
        {
            addAction(neigh_row,neigh_col,actions,"?");
        }
        if(countIslands_szCheck(matrix,neigh_row,neigh_col))
        {
            addAction(neigh_row,neigh_col,actions,"Neighbourland!");
            countIslands_DFS(matrix,neigh_row,neigh_col, actions);
        }
        if(szBordCheck(neigh_row,neigh_col,matrix.rowCount(),matrix.colCount()))
        {
            addAction(row,col,actions,"Back to prew neigh.");
        }
    }
    return;
}

int countIslands(Vector2D<LandNode>& matrix, std::vector<PlayAction> &actions)
{
    actions.clear();
    int r_count = matrix.rowCount();
    int c_count = matrix.colCount();
    int count = 0;
    for(int i = 0; i<r_count;i++)
    {
        for(int j = 0; j<c_count;j++)
        {
            if((matrix(i,j)._land_type_ == 'L')&&
                    (!matrix(i,j)._visited_))
            {
                addAction(i,j,actions,"Base Land!");
                countIslands_DFS(matrix,i,j,actions);
                addAction(i,j,actions,"Back to baseland");
                count++;
            }
        }
    }
    return count;
}

bool countLand_brdrChck(Vector2D<char>& matrix, int row, int col)
{
    int rows_size = matrix.rowCount();
    int cols_size = matrix.colCount();
    if(szBordCheck(row,col,rows_size,cols_size)&&(matrix(row,col)=='L'))
    {
        return true;
    }
    return false;
}
void countLand_OPT_DFS(Vector2D<char>& matrix, int row, int col, std::vector<PlayAction>& actions)
{
    matrix(row,col) = 'W';
    vector<std::pair<int,int>> neighbours({
                                              {-1,-1},  {-1,0},  {-1,1},
                                              { 0,-1},           { 0,1},
                                              { 1,-1},  { 1,0},  { 1,1}});
    for(std::pair<int,int>& neigh: neighbours)
    {
        int neigh_x = row+ neigh.first;
        int neigh_y = col+ neigh.second;
        if(szBordCheck(neigh_x, neigh_y,matrix.rowCount(),matrix.colCount()))
        {
            addAction(neigh_x,neigh_y,actions,"CheckNeigh",PlayAction::PAct_Warn);
        }
        if(countLand_brdrChck(matrix,neigh_x,neigh_y))
        {
            addAction(neigh_x,neigh_y,actions,"LandFound",PlayAction::PAct_Safe);
            countLand_OPT_DFS(matrix,neigh_x,neigh_y,actions);
        }
        else if(szBordCheck(neigh_x, neigh_y,matrix.rowCount(),matrix.colCount()))
        {
            addAction(neigh_x,neigh_y,actions,"Water",PlayAction::PAct_Err);
        }
        if(szBordCheck(neigh_x, neigh_y,matrix.rowCount(),matrix.colCount()))
        {
            addAction(row,col,actions,"Back2Prew",PlayAction::PAct_Safe);
        }
    }
    return;
}

int countLand_OPT(Vector2D<char>& matrix, std::vector<PlayAction>& actions)
{
    int rows_size = matrix.rowCount();
    int cols_size = matrix.colCount();
    int count = 0;
    for(int x = 0; x<rows_size; x++)
    {
        for(int y = 0; y<cols_size;y++)
        {
            addAction(x,y,actions,"Search",PlayAction::PAct_Warn);
            if(matrix(x,y)=='L')
            {
                count++;
                addAction(x,y,actions,"Base\"L\"Found",PlayAction::PAct_Safe);
                countLand_OPT_DFS(matrix, x, y,actions);
                addAction(x,y,actions,"Back2BaseLand",PlayAction::PAct_Safe);
            }
            else
            {
                addAction(x,y,actions,"Water",PlayAction::PAct_Err);
            }
        }
    }
    return count;
}


void countLand_BFS(Vector2D<LandNode>&matrix, int row, int col, std::vector<PlayAction>& actions)
{
    vector<std::pair<int,int>> neighbours({
                                              {-1,-1},  {-1,0},  {-1,1},
                                              { 0,-1},           { 0,1},
                                              { 1,-1},  { 1,0},  { 1,1}});
    std::queue<std::pair<int,int>> q;
    q.push({row,col});
    matrix(row,col)._visited_=true;
    while(!q.empty())
    {
        int cur_row = q.front().first;
        int cur_col = q.front().second;
        q.pop();
        addAction(cur_row,cur_col,actions,"CurrLand",PlayAction::PAct_Safe);
        for(std::pair<int,int>& neigh: neighbours)
        {
            int neigh_row = cur_row+neigh.first;
            int neigh_col = cur_col+neigh.second;
            if(szBordCheck(neigh_row, neigh_col,matrix.rowCount(),matrix.colCount()))
            {
                addAction(neigh_row,neigh_col,actions,"CheckNeigh",PlayAction::PAct_Warn);
                if(matrix(neigh_row,neigh_col)._land_type_=='L')
                {
                    if(matrix(neigh_row,neigh_col)._visited_)
                    {
                        addAction(neigh_row,neigh_col,actions,"Land Visited",PlayAction::PAct_Err);
                    }
                    else
                    {
                        addAction(neigh_row,neigh_col,actions,"LandFound",PlayAction::PAct_Safe);
                        matrix(neigh_row,neigh_col)._visited_ = true;
                        q.push({neigh_row,neigh_col});
                    }
                }
                else
                {
                    addAction(neigh_row,neigh_col,actions,"Water",PlayAction::PAct_Err);
                }
                addAction(cur_row,cur_col,actions,"Back2Prew",PlayAction::PAct_Safe);
            }
        }
    }
    return;
}
int countLand_BFS_based(Vector2D<LandNode>&matrix, std::vector<PlayAction>& actions)
{
    int res = 0;
    for(int row = 0; row<matrix.rowCount();row++)
    {
        for(int col = 0; col<matrix.colCount();col++)
        {
            addAction(row,col,actions,"Search",PlayAction::PAct_Warn);
            if(countIslands_szCheck(matrix,row,col))
            {
                addAction(row,col,actions,"Base\"L\"Found",PlayAction::PAct_Safe);
                countLand_BFS(matrix,row,col,actions);
                addAction(row,col,actions,"Back2BaseLand",PlayAction::PAct_Safe);
                res++;
            }
            else if(matrix(row,col)._land_type_ =='W')
            {
                addAction(row,col,actions,"Water",PlayAction::PAct_Err);
            }
            else if((matrix(row,col)._land_type_=='L')&&
                    matrix(row,col)._visited_)
            {
                addAction(row,col,actions,"Visited",PlayAction::PAct_Err);
            }
        }
    }
    return res;
}

void countLand_BFS_OPT(Vector2D<char>&matrix, int row, int col, std::vector<PlayAction>& actions)
{
    vector<std::pair<int,int>> neighbours({
                                              {-1,-1},  {-1,0},  {-1,1},
                                              { 0,-1},           { 0,1},
                                              { 1,-1},  { 1,0},  { 1,1}});

    std::queue<std::pair<int,int>> q;
    q.push({row,col});
    matrix(row,col)='V';
    while(!q.empty())
    {
        int cur_row = q.front().first;
        int cur_col = q.front().second;
        q.pop();
        addAction(cur_row,cur_col,actions,"CurrLand",PlayAction::PAct_Safe);
        for(std::pair<int,int>& neigh: neighbours)
        {
            int neigh_row = cur_row+neigh.first;
            int neigh_col = cur_col+neigh.second;
            if(szBordCheck(neigh_row, neigh_col,matrix.rowCount(),matrix.colCount()))
            {
                addAction(neigh_row,neigh_col,actions,"CheckNeigh",PlayAction::PAct_Warn);
                if(matrix(neigh_row,neigh_col)=='L')
                {
                    addAction(neigh_row,neigh_col,actions,"LandFound",PlayAction::PAct_Safe);
                    matrix(neigh_row,neigh_col)= 'V';
                    q.push({neigh_row,neigh_col});
                }
                else if(matrix(neigh_row,neigh_col)=='W')
                {
                    addAction(neigh_row,neigh_col,actions,"Water",PlayAction::PAct_Err);
                }
                else if(matrix(neigh_row,neigh_col)=='V')
                {
                    addAction(neigh_row,neigh_col,actions,"Land Visited",PlayAction::PAct_Err);
                }
                addAction(cur_row,cur_col,actions,"Back2Prew",PlayAction::PAct_Safe);
            }
        }
    }
    return;
}

int countLand_BFS_OPT_based(Vector2D<char>&matrix, std::vector<PlayAction>& actions)
{
    int islands = 0;
    for(int x = 0; x<matrix.rowCount();x++)
    {
        for(int y = 0; y<matrix.colCount();y++)
        {
            addAction(x,y,actions,"Search",PlayAction::PAct_Warn);
            if(matrix(x,y)=='L')
            {
                addAction(x,y,actions,"NewLandFound!",PlayAction::PAct_Safe);
                countLand_BFS_OPT(matrix,x,y,actions);
                islands++;
            }
            else if(matrix(x,y)=='W')
            {
                addAction(x,y,actions,"Water",PlayAction::PAct_Err);
            }
            else if(matrix(x,y)=='V')
            {
                addAction(x,y,actions,"Visited",PlayAction::PAct_Err);
            }
        }
    }
    return islands;
}

//______________________________________________________________________________________________________
//_________________________________Flood Fill__________________________________________________________

using std::pair;

void floodFill_DFS(Vector2D<int>& matrix, int row, int col,
                   int oldColour, int newColour,
                   std::vector<PlayAction>& actions)
{
    if(!szBordCheck(row,col,matrix.rowCount(),matrix.colCount()))
    {
        return;
    }
    addAction(row,col,actions,"Colour check",PlayAction::PAct_Warn);
    if(matrix(row,col)!=oldColour)
    {
        addAction(row,col,actions,"Non-base colour",PlayAction::PAct_Err);
        return;
    }
    matrix(row,col) = newColour;
    addAction(row,col,actions,"Base colour!",PlayAction::PAct_Safe,true,newColour);
    vector<pair<int,int>> directions({{1,0},{0,1},{-1,0},{0,-1}});
    for(pair<int,int>& dir: directions)
    {
        int neigh_row = row+dir.first;
        int neigh_col = col+dir.second;
        floodFill_DFS(matrix,neigh_row,neigh_col,oldColour,newColour,actions);
        addAction(row,col,actions,"Back2prev",PlayAction::PAct_Safe);
    }
}

void floodFill_DFS_Base(Vector2D<int>& matrix,
                        int row, int col, int newColour,
                        std::vector<PlayAction>& actions)
{
    if(szBordCheck(row,col,matrix.rowCount(),matrix.colCount()))
    {
        if(matrix(row,col)==newColour)
            return;
    }
    else
    {
        return;
    }
    int oldColour = matrix(row,col);
    addAction(row,col,actions,"Base colour ["+
              std::to_string(oldColour)+"]",
              PlayAction::PAct_Safe);
    floodFill_DFS(matrix,row,col,oldColour,newColour,actions);
    addAction(row,col,actions,"Colour ["+
              std::to_string(newColour)+"] filled",
              PlayAction::PAct_Safe);
    return;
}


int floodFill_BFS(Vector2D<int>& matrix, int row, int col, int newColour, std::vector<PlayAction>& actions)
{
    if(matrix(row,col)==newColour)
    {
        return 0;
    }
    vector<pair<int,int>> directions({{1,0},{0,1},{-1,0},{0,-1}});
    std::queue<pair<int,int>>q;
    int old_colour = matrix(row,col);
    q.push({row,col});
    matrix(row,col) = newColour;
    addAction(row,col,actions,"StartPt",PlayAction::PAct_Safe,true,newColour);
    addAction(row,col,actions,"Base colour ["+
              std::to_string(old_colour)+"]",
              PlayAction::PAct_Safe);
    int count = 1;
    while(!q.empty())
    {
        int cur_row = q.front().first;
        int cur_col = q.front().second;
        addAction(cur_row,cur_col,actions,"CurrCell",PlayAction::PAct_Safe);
        q.pop();
        for(const pair<int,int>& dir: directions)
        {
            int neigh_row = cur_row+dir.first;
            int neigh_col = cur_col+dir.second;
            if(szBordCheck(neigh_row,neigh_col,matrix.rowCount(),matrix.colCount()))
            {
                addAction(neigh_row,neigh_col,actions,"Colour check",PlayAction::PAct_Warn);
                if(matrix(neigh_row,neigh_col)== old_colour)
                {
                    addAction(neigh_row,neigh_col,actions,"Base colour!",PlayAction::PAct_Safe,true,newColour);
                    matrix(neigh_row,neigh_col) = newColour;
                    q.push({neigh_row,neigh_col});
                    count++;
                }
                else
                {
                    addAction(neigh_row,neigh_col,actions,"Non-base colour",PlayAction::PAct_Err);
                }
            }
        }
        addAction(cur_row,cur_col,actions,"Back2Curr",PlayAction::PAct_Safe);
    }
    return count;
}


//___________________________________________________________________________________________________________________
//__________________________________________________Bipartite-check----------------


bool isBipartite_BFS(ListGraph& obj, std::string& actions)
{
    actions.clear();
    actions.append("BFS_check if the list-based graph is bipartite;\n");
    for(int i = 0; i<obj.size();i++)
    {
        obj.at(i) = -1;
    }
    std::queue<int> BFS_queue;
    for(int i = 0; i<obj.size();i++)
    {
        if(obj.at(i)==-1)
        {
            actions.append("Vertex at ["+std::to_string(i)+
                           "] has default value ("+
                           std::to_string(obj.at(i))+");\n");
            obj.at(i)= 0;
            BFS_queue.push(i);
            while(!BFS_queue.empty())
            {
                int cur_vertex = BFS_queue.front();
                BFS_queue.pop();
                actions.append("Vertex ["+std::to_string(cur_vertex)+
                               "] is a current vertex;\n");
                vector<int> adjustments(obj.getEdges(cur_vertex));
                for(int edged_vertex:adjustments)
                {
                    actions.append("Vertex ["+std::to_string(edged_vertex)+
                                   "] has value ("+std::to_string(obj(edged_vertex))+
                                   ");\n");
                    if(obj.at(edged_vertex)==-1)
                    {

                        obj.at(edged_vertex) = 1 - obj.at(cur_vertex);
                        BFS_queue.push(edged_vertex);
                        actions.append("Vertex ["+std::to_string(edged_vertex)+
                                       "] vas recolorased to ("+
                                       std::to_string(obj(edged_vertex))+");\n");
                    }
                    else if(obj.at(edged_vertex)==obj.at(cur_vertex))
                    {
                        actions.append("Vertex ["+std::to_string(edged_vertex)+
                                       "] has the same coloration "
                                       "as vertex at["+std::to_string(cur_vertex)+"];\n");
                        actions.append("Graph isn't bipartite;");
                        return false;
                    }
                }
            }
        }
    }
    for(int i = 0; i<obj.size();i++)
    {
        actions.append("ver["+std::to_string(i)+"]val("+std::to_string(obj(i))+")|");
    }
    actions.append("\nGraph is bipartite;");
    return true;
}

bool isBipartite_DFS(int cur_index,int cur_val, ListGraph& obj, string& actions)
{
    actions.append("Vertex ["+std::to_string(cur_index)+
                   "] is a current vertex;\n");
    obj(cur_index) = cur_val;
    actions.append("Vertex ["+std::to_string(cur_index)+
                   "] vas recolorased to ("+
                   std::to_string(obj(cur_index))+");\n");
    vector<int> adjustments(obj.getEdges(cur_index));
    for(int edged_vertex: adjustments)
    {
        actions.append("Connected vertex["+std::to_string(edged_vertex)+
                       "] value ("+std::to_string(obj(edged_vertex))+
                       ");\n");
        if(obj.at(edged_vertex)==-1)
        {
            if(!isBipartite_DFS(edged_vertex,1-cur_val,obj,actions))
            {
                return false;
            }
            actions.append("Back to cur Vertex ["+std::to_string(cur_index)+"]\n");
        }
        else if(obj.at(edged_vertex)==obj.at(cur_index))
        {
            actions.append("Vertex ["+std::to_string(edged_vertex)+
                           "] has the same coloration "
                           "as vertex at["+std::to_string(cur_index)+"];\n");
            actions.append("Graph isn't bipartite;");
            return false;
        }
//        else
//        {
//            actions.append("Src vert ["+std::to_string(cur_index)+"] clr ("+
//                           std::to_string(obj.at(cur_index))+
//                           ") =/= dest vert ["+
//                           std::to_string(edged_vertex)+
//                           "] clr ("+std::to_string(obj.at(edged_vertex))+");\n");
//        }
    }
    return true;
}

bool isBipartite_DFS_Base(ListGraph& obj, string& actions)
{
    actions.clear();
    actions.append("DFS_check if the list-based graph is bipartite;\n");
    obj.fill(-1);
    for(int i = 0; i<obj.size();i++)
    {
        if(obj.at(i)==-1)
        {
            actions.append("Vertex at ["+std::to_string(i)+
                           "] has default value ("+
                           std::to_string(obj.at(i))+");\n");
            if(!isBipartite_DFS(i,0,obj,actions))
            {
                return false;
            }
        }
    }
    for(int i = 0; i<obj.size();i++)
    {
        actions.append("ver["+std::to_string(i)+"]val("+std::to_string(obj(i))+")|");
    }
    actions.append("\nGraph is bipartite;");
    return true;
}



//________________________________________________________________________________________________________________________
//_________________________________________________Word_ladder____________________________________________________________

int minWordTransform(string& start, string& target, std::map<string,int>& dictionary, string& actions)
{
    if(start == target)
    {
        actions.append("Maching found ["+start+"] = ["+target+"];\n");
        return 1;
    }
    int min = INT_MAX;
    dictionary[start] = 1;
    actions.append("Current word ["+start+"];\n");
    for(int i = 0; i<static_cast<int>(start.size());i++)
    {
        char orig_char = start[i];
        for(char c = 'a'; c <='z';c++)
        {
            start[i] = c;
            if((dictionary.find(start)!=dictionary.end())&&(dictionary[start]==0))
            {
                actions.append("According word found ["+start+"] in dictionary;\n");
                min = std::min(min,1+minWordTransform(start,target,dictionary,actions));
            }
        }
        start[i] = orig_char;
    }

    dictionary[start] = 0;
    return min;
}

int wordLadder_Backtrack(string& start, string& target, vector<string>& variants, string& actions)
{
    std::map<string,int> dictionary;
    for(const string& word: variants)
    {
        dictionary.insert({word,0});
    }
    actions.append("WordLadder with backtracking;\n");
    int result = minWordTransform(start,target,dictionary,actions);
    if(result == INT_MAX)
        result = 0;
    return result;
}


int wordLadder_BFS(string& start, string& target, vector<string>& variants, string& actions)
{
    actions.clear();
    actions.append("WordLadder with BFS;\n");
    std::unordered_set<string> dict_set(variants.begin(),variants.end());
    actions.append("Trying to transform word ["+start+"] to word ["+target+"];\n");
    actions.append("We have following dictionary:\n");
    for(string& st: variants)
    {
        actions.append("\t"+st+";\n");
    }
    int steps = 0;
    int base_word_size = static_cast<int>(start.length());
    std::queue<string> words;
    words.push(start);
    while(!words.empty())
    {
        steps++;
        int cur_word_size = static_cast<int>(words.size());
        for(int i = 0; i<cur_word_size;i++)
        {
            string cur_word(words.front());
            actions.append("Current word ["+cur_word+"]\n");
            words.pop();
            for(int j = 0; j < base_word_size;j++)
            {
                char orig_char = cur_word[j];
                actions.append("Trying to change the ["+std::to_string(j)+"]'th symbol\n characters:");
                for(char c = 'a';c<='z';c++)
                {
                    actions.append(string(1,c)+",");
                    cur_word[j] = c;
                    if(cur_word == target)
                    {
                        actions.append("\nThe target word ["+cur_word+"] found;\n");
                        return steps+1;
                    }
                    else if(dict_set.find(cur_word)!=dict_set.end())
                    {
                        actions.append("\nThe new word ["+cur_word+"] found in the dictionary;\n");
                        dict_set.erase(cur_word);
                        words.push(cur_word);
                    }
                }
                actions.append("\n");
                cur_word[j] = orig_char;
            }
        }
    }
    actions.append("Word can't be created with obtained dictionary;\n");
    return 0;
}


//________________________________________________SnakesNLadders___________________________________________________________

int SnakesNLadders_minDiceThrow_BFS(ListGraph& obj, string& actions)
{
    actions.clear();
    actions.append("Finding min dice rolls count 4 snakes&ladders;\n");
    std::queue<std::pair<int,int>> BFS_queue;
    obj.at(0) = true;
    BFS_queue.push({0,0});
    std::queue<string> rolls_queue;
    rolls_queue.push("0");
    while(!BFS_queue.empty())
    {
        std::pair<int,int> cur_pos = BFS_queue.front();
        int cur_vertex = cur_pos.first;
        int cur_dist = cur_pos.second;
        string cur_rolls = rolls_queue.front();
        rolls_queue.pop();
        actions.append("Cur vert ["+std::to_string(cur_vertex)+
                       "] with dist ["+std::to_string(cur_dist)+"];\n");
        actions.append(cur_rolls+";\n");
        if(cur_vertex == (obj.size()-1))
        {
            actions.append("End of bord with ["+
                           std::to_string(cur_dist)+"] dice rolls;\n");
            return cur_dist;
        }
        BFS_queue.pop();
        actions.append("Unmarked vertices from ["+
                       std::to_string(cur_vertex)+"] to reach: ");
        vector<int> rolls = obj.getEdges(cur_vertex);
        int rolls_size = static_cast<int>(rolls.size());
        string reachable;
        for(int i = 0; i<rolls_size;i++)//next_pos:rolls)
        {
            string next_rolls = cur_rolls;
            int next_pos = rolls.at(i);
            if(!obj.at(next_pos))
            {
                int next_dist = cur_dist;
                if(rolls_size ==1 && next_pos!=obj.size()-2)
                {
                    if(cur_vertex>next_pos)
                    {
                        reachable.append("Vert ["+std::to_string(cur_vertex)+
                                       "] is snake head, leads to ["+
                                       std::to_string(next_pos)+"] ");
                    }
                    else
                    {
                        reachable.append("Vert ["+std::to_string(cur_vertex)+
                                       "] is ladder, leads to ["+
                                       std::to_string(next_pos)+"] ");
                    }
                    next_rolls.erase(next_rolls.find_last_of('>')+1);
                    next_rolls.append(std::to_string(next_pos));
                }
                else
                {
                    reachable.append(" ["+std::to_string(next_pos)+
                               "],");
                    next_dist+=1;
                    next_rolls.append("-("+std::to_string(i+1)+")->"+std::to_string(next_pos));
                }
                obj.at(next_pos) = true;

//                if(rolls.size()!=1||next_pos==obj.size()-2)                 //Калі мы трапляем на драбіны,
//                {                                                           //Пазіцыя аўтаматычна пераносіцца на наступную
//                                                                            //Таму
//                }                                                           //Колькасьць кідаў не павялічваецца

                rolls_queue.push(next_rolls);
                BFS_queue.push({next_pos,next_dist});
            }

        }
        if(!reachable.empty())
        {
            reachable.pop_back();
            actions.append(reachable+";\n");
        }
        else
        {
            actions.append("none;\n");
        }
    }
    actions.append("The algorithm can't reach the end of the borad;");
    return -1;
}

void SnL_algorithm_DFS(int cur_vertex, int cur_roll_count, int& rolls, ListGraph& obj, string& actions, vector<int> prev_rolls)
{
    if(cur_roll_count>=rolls)
    {
        return;
    }
    if((obj.at(cur_vertex)>=0)&&obj.at(cur_vertex)<=cur_roll_count)
    {
        return;
    }
    if(cur_vertex==obj.size()-1)
    {
        actions.append("________|||End of bord with ["+
                       std::to_string(cur_roll_count)+"] dice rolls: (");
        actions.append("(");
        for(int i: prev_rolls)
        {
            actions.append(std::to_string(i)+"->");
        }
        actions.pop_back();
        actions.pop_back();
        actions.append(")|||_______\n");
        rolls = cur_roll_count;
        return;
    }

    obj.at(cur_vertex) = cur_roll_count;
    actions.append("Cur vertex ["+std::to_string(cur_vertex)+
                   "] with roll count ["+std::to_string(cur_roll_count)+"];\n");
    actions.append("Previous rolls: ");
    if(prev_rolls.size())
    {
        actions.append("(");
        for(int i: prev_rolls)
        {
            actions.append(std::to_string(i)+"->");
        }
        actions.pop_back();
        actions.pop_back();
        actions.append(")");
    }
    else
    {
        actions.append("none;");
    }
    actions.append("\n");

    vector<int> diceroll_options(obj.getEdges(cur_vertex));
    int diceroll_size = static_cast<int>(diceroll_options.size());
    for(int i = 0; i<diceroll_size; i++)//edge: diceroll_options)
    {
        vector<int> cur_rolls = prev_rolls;
        int next_roll_count = cur_roll_count;
        if(diceroll_size!=1 || cur_vertex==obj.size()-2)
        {
            actions.append("\tFrom vert ["+std::to_string(cur_vertex)+
                           "] Dice result ("+
                           std::to_string(i+1)+") means go to vert ["+
                           std::to_string(diceroll_options.at(i))+
                           "];\n");
            next_roll_count+=1;
            cur_rolls.push_back(diceroll_options.at(i));
        }
        else
        {
            if(diceroll_options.at(i)<cur_vertex)
            {
                actions.append("\t["+std::to_string(cur_vertex)+
                               "] is snake head. Index["+
                               std::to_string(cur_vertex)+
                               "] changed to ["+
                               std::to_string(diceroll_options.at(i))+
                               "];\n");
                cur_rolls.pop_back();
                cur_rolls.push_back(diceroll_options.at(i));
            }
            else
            {
                actions.append("\t["+std::to_string(cur_vertex)+
                               "] is ladder. Index["+std::to_string(cur_vertex)+
                               "] changed to: ["+std::to_string(diceroll_options.at(i))+
                               "];\n");

                cur_rolls.pop_back();
                cur_rolls.push_back(diceroll_options.at(i));
            }
        }
        SnL_algorithm_DFS(diceroll_options.at(i),next_roll_count,rolls,obj,actions,cur_rolls);
    }
    return;
 }

int SnakesNLadders_minDiceThrow_DFS(ListGraph& obj, string& actions)
{
    actions.clear();
    actions.append("Finding min dice roll count S&L with DFS;\n");
    int rolls = INT_MAX;
    vector<int> prev_rolls;
    prev_rolls.push_back(0);
    SnL_algorithm_DFS(0,0,rolls,obj,actions,prev_rolls);
    actions.append("The minumal number of rolls: ["+std::to_string(rolls)+
                   "];\n");
    return rolls==INT_MAX?-1:rolls;
}


//___________________________________________________WaterJigProblem________________________________________________________

void printCurState(int cur_r_jug, int r_jug_max,
                   int cur_l_jug, int l_jug_max,
                   int step, string& actions)
{
    actions.append("Current state: right jug ["+
                   std::to_string(cur_r_jug)+"] of ["+
                   std::to_string(r_jug_max)+
                   "], left jug ["+
                   std::to_string(cur_l_jug)+"] of ["+
                   std::to_string(l_jug_max)+"], step ("+
                   std::to_string(step)+");\n");
//    actions.append("\n");
//    for(int i = 0; i<visited.size();i++)
//    {
//        for(int j = 0; j<visited.at(i).size();j++)
//        {
//            actions.append("["+std::to_string(i)+"]["+
//                           std::to_string(j)+"]("+
//                           (visited.at(i).at(j)?"true":"false")+"), ");
//        }
//        actions.pop_back();
//        actions.pop_back();
//        actions.append("\n");
//    }
    return;
}


int waterJigProblem_BFS(int right_jig, int left_jig, int desirable_value, string& actions, vector<PlayAction> &act_st)
{
    actions.append("Water jig solution using BFS-based algorithm;\n");
    if(desirable_value>std::max(right_jig,left_jig))
    {
        actions.append("Desirable value of water ["+
                       std::to_string(desirable_value)+
                       "] is above the value of both jigs: r:["+
                       std::to_string(right_jig)+"],l:["+
                       std::to_string(left_jig)+"]\n");
        return -1;
    }
    std::queue<vector<int>> BFS_queue;  // [r-jig,l-jig,step]
    vector<vector<bool>> visited(right_jig+1,vector<bool>(left_jig+1,false)); //matrix-based graph
    actions.append("Trying to get ["+
                   std::to_string(desirable_value)+
                   "] liters of water, using 2 jigs: right:["+
                   std::to_string(right_jig)+"],left:["+
                   std::to_string(left_jig)+"]\n");
    if(desirable_value<right_jig+1)
    {
        for(int i = 0; i<left_jig+1;i++)
        {
            addAction(desirable_value,i,act_st,"Must reach",PlayAction::PAct_Warn);
        }
    }
    if(desirable_value<left_jig+1)
    {
        for(int i = 0; i<right_jig+1;i++)
        {
            addAction(i,desirable_value,act_st,"Must reach",PlayAction::PAct_Warn);
        }
    }
    //printCurState(visited,actions);
    BFS_queue.push({0,0,0});
    addAction(0, 0, act_st,
                   "Starting state;",
                   PlayAction::PlayActionsType::PAct_Safe,true,1);
    visited.at(0).at(0) = true;
    while(!BFS_queue.empty())
    {
        vector<int> cur_state = BFS_queue.front();
        BFS_queue.pop();
        int cur_r_jig = cur_state.at(0);
        int cur_l_jig = cur_state.at(1);
        int steps = cur_state.at(2);
        printCurState(cur_r_jig,right_jig,cur_l_jig,left_jig,steps,actions);
        addAction(cur_r_jig, cur_l_jig, act_st,
                       "r["+std::to_string(cur_r_jig)+"/"+std::to_string(right_jig)+"], l["+
                  std::to_string(cur_l_jig)+"/"+std::to_string(left_jig)+"]",
                       PlayAction::PlayActionsType::PAct_Warn);
//        actions.append("\tCurrent step is ["+
//                       std::to_string(steps)+
//                       "], right jig:["+
//                       std::to_string(cur_r_jig)+
//                       "],left jig:["+
//                       std::to_string(cur_l_jig)+"]\n");
        addAction(cur_r_jig,cur_l_jig,act_st,"Check opt", PlayAction::PAct_Safe);
        if(cur_r_jig == desirable_value || cur_l_jig == desirable_value)
        {
            addAction(cur_r_jig,cur_l_jig,act_st,"St rchd!", PlayAction::PAct_Safe);
            actions.append("\tSolution is found;\n");
            //printCurState(visited,actions);
            return steps;
        }

        if(!visited.at(right_jig).at(cur_l_jig))
        {
            addAction(right_jig,cur_l_jig,act_st,"Fill R (r["+
                      std::to_string(right_jig)+"]l["+
                      std::to_string(cur_l_jig)+"])",
                      PlayAction::PAct_Warn,true,1);
            actions.append("On step ["+std::to_string(steps)+
                           "] filling right jig is possible;\n");
            visited.at(right_jig).at(cur_l_jig) = true;
            BFS_queue.push({right_jig,cur_l_jig,steps+1});
            addAction(cur_r_jig,cur_l_jig,act_st,"Back2 cur",PlayAction::PAct_Safe);
            //printCurState(visited,actions);
        }

        if(!visited.at(cur_r_jig).at(left_jig))
        {
            addAction(cur_r_jig,left_jig,act_st,"Fill L(r["+
                      std::to_string(cur_r_jig)+"]l["+
                      std::to_string(left_jig)+"])",
                      PlayAction::PAct_Warn,true,1);
            actions.append("On step ["+std::to_string(steps)+
                           "] filling left jig is possible;\n");
            visited.at(cur_r_jig).at(left_jig) = true;
            BFS_queue.push({cur_r_jig,left_jig,steps+1});
            addAction(cur_r_jig,cur_l_jig,act_st,"Back2 cur",PlayAction::PAct_Safe);
            //printCurState(visited,actions);
        }

        if(!visited.at(0).at(cur_l_jig))
        {
            addAction(0,cur_l_jig,act_st,"Empt R(r["+
                      std::to_string(0)+"]l["+
                      std::to_string(cur_l_jig)+"])",
                      PlayAction::PAct_Warn,true,1);
            actions.append("On step ["+std::to_string(steps)+
                           "] empty right jig is possible;\n");
            visited.at(0).at(cur_l_jig) = true;
            BFS_queue.push({0,cur_l_jig,steps+1});
            addAction(cur_r_jig,cur_l_jig,act_st,"Back2 cur",PlayAction::PAct_Safe);
            //printCurState(visited,actions);
        }

        if(!visited.at(cur_r_jig).at(0))
        {
            addAction(cur_r_jig,0,act_st,"Empt L(r["+
                      std::to_string(cur_r_jig)+"]l["+
                      std::to_string(0)+"])",
                      PlayAction::PAct_Warn,true,1);
            actions.append("On step ["+std::to_string(steps)+
                           "] empty left jig is possible;\n");
            visited.at(cur_r_jig).at(0) = true;
            BFS_queue.push({cur_r_jig,0,steps+1});
            addAction(cur_r_jig,cur_l_jig,act_st,"Back2 cur",PlayAction::PAct_Safe);
            //printCurState(visited,actions);
        }

        int pour_r_to_l = std::min(cur_r_jig,left_jig-cur_l_jig);
        if(!visited.at(cur_r_jig-pour_r_to_l).at(cur_l_jig+pour_r_to_l))
        {
            addAction(cur_r_jig-pour_r_to_l,
                      cur_l_jig+pour_r_to_l,act_st,
                      "R(["+std::to_string(cur_r_jig)+
                      "]["+std::to_string(cur_l_jig)+
                      "])>L(["+std::to_string(cur_r_jig-pour_r_to_l)+
                      "]["+std::to_string(cur_l_jig+pour_r_to_l)+"])",
                      PlayAction::PAct_Warn,true,1);
            actions.append("On step ["+std::to_string(steps)+
                           "] pour right jig to left jig is possible;\n");
            visited.at(cur_r_jig-pour_r_to_l).at(cur_l_jig+pour_r_to_l) = true;
            BFS_queue.push({cur_r_jig-pour_r_to_l,cur_l_jig+pour_r_to_l,steps+1});
            addAction(cur_r_jig,cur_l_jig,act_st,"Back2 cur",PlayAction::PAct_Safe);
            //printCurState(visited,actions);
        }

        int pour_l_to_r = std::min(cur_l_jig,right_jig-cur_r_jig);
        if(!visited.at(cur_r_jig+pour_l_to_r).at(cur_l_jig-pour_l_to_r))
        {
            addAction(cur_r_jig+pour_l_to_r,
                      cur_l_jig-pour_l_to_r,act_st,
                      "R(["+std::to_string(cur_r_jig)+
                      "]["+std::to_string(cur_l_jig)+
                      "]<L(["+std::to_string(cur_r_jig+pour_l_to_r)+
                      "]["+std::to_string(cur_l_jig-pour_l_to_r)+"])",
                      PlayAction::PAct_Warn,true,1);
            actions.append("On step ["+std::to_string(steps)+
                           "] pour left jig to right jig is possible;\n");
            visited.at(cur_r_jig+pour_l_to_r).at(cur_l_jig-pour_l_to_r) = true;
            BFS_queue.push({cur_r_jig+pour_l_to_r,cur_l_jig-pour_l_to_r,steps+1});
            addAction(cur_r_jig,cur_l_jig,act_st,"Back2 cur",PlayAction::PAct_Safe);
            //printCurState(visited,actions);
        }

    }
    addAction(0,0,act_st,"Impossible",PlayAction::PAct_Err);
    actions.append("Algorithm can't reach the solution;\n");
    return -1;
}

//________________________________Atlantic-Pacific Waterflow________________________________


void twoWayWaterFlow_BFS(Vector2D<Atl_Pac_Node>& matrix,
                         vector<std::queue<pair<int,int>>> BFS_queues,
                         vector<PlayAction>& actions)
{
    vector<pair<int,int>> directions({{0,1},{0,-1},{-1,0},{1,0}});
    for(int shores = 0; shores<static_cast<int>(BFS_queues.size());shores++)
    {
        while(!BFS_queues.at(shores).empty())
        {
            pair<int,int> cur_coord = BFS_queues.at(shores).front();
            BFS_queues.at(shores).pop();
            int cur_row = cur_coord.first;
            int cur_col = cur_coord.second;
            addAction(cur_row,cur_col,actions,"Cur land",PlayAction::PAct_Safe);

            matrix(cur_row,cur_col)._visited_.at(shores) = true;
            for(pair<int,int>& dir:directions)
            {
                int next_row = cur_row+dir.first;
                int next_col = cur_col+dir.second;
                if(szBordCheck(next_row,next_col,matrix.rowCount(),matrix.colCount()))
                {
                    addAction(next_row,next_col,actions,"?Check",PlayAction::PAct_Warn);
                    if(matrix(next_row,next_col)._value_>=matrix(cur_row,cur_col)._value_)
                    {
                        if(!matrix(next_row,next_col)._visited_.at(shores))
                        {
                            BFS_queues.at(shores).push({next_row,next_col});
                            matrix(next_row,next_col)._visited_.at(shores)=true;
                            addAction(next_row,next_col,actions,"High land!",PlayAction::PAct_Safe);
                        }
                        else
                        {
                            addAction(next_row,next_col,actions,"Land visited",PlayAction::PAct_Err);
                        }
                    }
                    else
                    {
                        addAction(next_row,next_col,actions,"Land is low",PlayAction::PAct_Err);
                    }
                    addAction(cur_row,cur_col,actions,"Back 2 base;",PlayAction::PAct_Safe);
                }
            }
        }
    }
}

int twoWayWaterFlow(Vector2D<Atl_Pac_Node> &matrix, vector<PlayAction>& actions)
{
    vector<std::queue<pair<int,int>>> BFS_queues(2,std::queue<pair<int,int>>());
    for(int i = 0; i<matrix.rowCount();i++)
    {
        for( int j = 0; j<matrix.colCount();j++)
        {
            matrix(i,j)._visited_.resize(2,false);
        }
    }
    int atlantic_q = 1, pacific_q=0;
    for(int i = 0; i <matrix.colCount();i++)
    {
        addAction(0,i,actions,"pacific shore",PlayAction::PAct_Warn);
        addAction(matrix.rowCount()-1,i,actions,"atlant shore",PlayAction::PAct_Warn);
        BFS_queues.at(pacific_q).push({0,i});
        matrix(0,i)._visited_.at(pacific_q) = true;
        BFS_queues.at(atlantic_q).push({matrix.rowCount()-1,i});
        matrix(matrix.rowCount()-1,i)._visited_.at(atlantic_q) = true;
    }
    for(int j = 0; j < matrix.rowCount()-1;j++)
    {
        addAction(j+1,0,actions,"pacific shore",PlayAction::PAct_Warn);
        addAction(j,matrix.rowCount()-1,actions,"atlant shore",PlayAction::PAct_Warn);
        BFS_queues.at(pacific_q).push({j+1,0});
        matrix(j+1,0)._visited_.at(pacific_q) = true;
        BFS_queues.at(atlantic_q).push({j,matrix.colCount()-1});
        matrix(j,matrix.colCount()-1)._visited_.at(atlantic_q) = true;
    }
    int answer = 0;
    twoWayWaterFlow_BFS(matrix,BFS_queues,actions);

    for(int i = 0; i < matrix.rowCount();i++)
    {
        for(int j = 0; j<matrix.colCount();j++)
        {
            bool cross = true;
            for(int l = 0; l<static_cast<int>(matrix(i,j)._visited_.size());l++)
            {
                cross=cross&&matrix(i,j)._visited_.at(l);
            }
            if(cross)
            {
                addAction(i,j,actions,"Crossing!",PlayAction::PAct_Warn);
                answer++;
            }
        }
    }
    return answer;
}

//________________________________Labyrinth pathfinding______________________________________


bool shortPathCore_DFS(Vector2D<LandNode>& matrix,
                       int cur_row, int cur_col,
                       int finish_row,int finish_col,
                       int& min_distance, int cur_distance,
                       vector<PlayAction>& actions,
                       std::stack<pair<int,int>>& result_path)
{
    if(cur_row==finish_row && cur_col==finish_col)
    {
        addAction(finish_row,finish_col,actions,"Destination!",PlayAction::PAct_Safe);
        if(min_distance>cur_distance)
        {
            std::stack<pair<int,int>> empty;
            std::swap(result_path,empty);
            min_distance = cur_distance;
        }
        result_path.push({finish_row,finish_col});
        return true;
    }
    vector<pair<int,int>> directions({{0,1},{0,-1},{-1,0},{1,0}});
    matrix(cur_row,cur_col)._visited_ = true;
    addAction(cur_row,cur_col,actions,"Cur pos",PlayAction::PAct_Safe);
    //for(pair<int,int>& dir:directions)
    while(directions.size())
    {
        int pos = std::rand()%directions.size();

        int neigh_row = cur_row+directions.at(pos).first;//dir.first;
        int neigh_col = cur_col+directions.at(pos).second;//dir.second;
        directions.erase(directions.begin()+pos);
        if(szBordCheck(neigh_row,neigh_col,matrix.rowCount(),matrix.colCount()))
        {
            if(matrix(neigh_row,neigh_col)._land_type_==1)
            {
                addAction(neigh_row,neigh_col,actions,"?PathCheck",PlayAction::PAct_Warn);
                if(!matrix(neigh_row,neigh_col)._visited_)
                {
                    addAction(neigh_row,neigh_col,actions,"A new way!",PlayAction::PAct_Safe);
                    shortPathCore_DFS(matrix,
                                      neigh_row,neigh_col,
                                      finish_row,finish_col,
                                      min_distance,cur_distance+1,
                                      actions,result_path);

                    result_path.push(std::make_pair(cur_row,cur_col));

                }
                else
                {
                    addAction(neigh_row,neigh_col,actions,"Visited_path",PlayAction::PAct_Err);
                }
                addAction(cur_row,cur_col,actions,"Back2Prev",PlayAction::PAct_Safe);
            }
        }
    }
    return false;

}

int shortPathLength_DFS(Vector2D<LandNode>& matrix,
                        int start_row, int start_col,
                        int finish_row, int finish_col,
                        vector<PlayAction>& actions)
{
    if(!szBordCheck(start_row,start_col,matrix.rowCount(),matrix.colCount())||
       !szBordCheck(finish_row,finish_col,matrix.rowCount(),matrix.colCount()))
    {
        return -1;
    }
    std::srand(std::time(0));
    std::stack<pair<int,int>> result_path;
    int distance = INT_MAX;
    shortPathCore_DFS(matrix,
                      start_row, start_col,
                      finish_row,finish_col,
                      distance,0,actions,
                      result_path);
    while(!result_path.empty())
    {
        addAction(result_path.top().first,
                  result_path.top().second,
                  actions,"Fin step",
                  PlayAction::PAct_Warn,
                  true,2);
        result_path.pop();
    }
    if(distance<0)
    {
        return -1;
    }
    return distance;
}


int shortPathLength_BFS(Vector2D<LandNode>& matrix,
                        int start_row, int start_col,
                        int finish_row, int finish_col,
                        vector<PlayAction>& actions)
{
    if(!szBordCheck(start_row,start_col,matrix.rowCount(),matrix.colCount())||
       !szBordCheck(finish_row,finish_col,matrix.rowCount(),matrix.colCount()))
    {
        return -1;
    }
    if(matrix(start_row,start_col)._land_type_==0 ||
            matrix(finish_row,finish_col)._land_type_ == 0)
    {
        return -1;
    }
    struct Trinity
    {
        int _row_;
        int _col_;
        int _distance_;
        vector<pair<int,int>> _prev_steps_;
        Trinity():_row_(0),_col_(0),_distance_(0){}
        Trinity(int row,int col, int dist):
            _row_(row),_col_(col),_distance_(dist){}
        Trinity(const Trinity& t):
            _row_(t._row_),_col_(t._col_),
            _distance_(t._distance_),
            _prev_steps_(t._prev_steps_){}
    };
    std::srand(std::time(0));
    std::queue<Trinity> BFS_queue;
    BFS_queue.push(Trinity(start_row,start_col,0));
    matrix(start_row,start_col)._visited_ = true;
    while(!BFS_queue.empty())
    {
        Trinity cur_data(BFS_queue.front());
        int cur_row = cur_data._row_;
        int cur_col = cur_data._col_;
        int distance = cur_data._distance_;
        BFS_queue.pop();
        addAction(cur_row,cur_col,actions,"Cur step["+std::to_string(distance)+"]");
        if(cur_row == finish_row && cur_col == finish_col)
        {
            addAction(cur_row,cur_col,actions,"Destination!");
            vector<pair<int,int>> prev_steps = cur_data._prev_steps_;
            int prev_st_size = static_cast<int>(prev_steps.size());
            for(int i = 0; i<prev_st_size;i++)
            {
                addAction(prev_steps.at(i).first,
                          prev_steps.at(i).second,
                          actions,
                          "Res step ["/*+
                          std::to_string(
                              prev_steps.at(i)._distance_)
                          +*/"]",PlayAction::PAct_Warn);
            }
            addAction(cur_row,cur_col,actions,"Find!");
            return distance;
        }
        vector<pair<int,int>> directions({{0,1},{0,-1},{-1,0},{1,0}});
        while(directions.size())
        {
            int pos = std::rand()%directions.size();
            int neigh_row = cur_row+directions.at(pos).first;
            int neigh_col = cur_col+directions.at(pos).second;
            directions.erase(directions.begin()+pos);
            if(szBordCheck(neigh_row,neigh_col,matrix.rowCount(),matrix.colCount()))
            {
                if(matrix(neigh_row,neigh_col)._land_type_==1)
                {
                    addAction(neigh_row,neigh_col,actions,"?CheckPath",PlayAction::PAct_Warn);
                    if(matrix(neigh_row,neigh_col)._visited_)
                    {
                        addAction(neigh_row,neigh_col,actions,"Visited!",PlayAction::PAct_Err);
                    }
                    else
                    {
                        addAction(neigh_row,neigh_col,actions,"New path!",PlayAction::PAct_Safe);
                        matrix(neigh_row,neigh_col)._visited_ = true;
                        Trinity t(neigh_row,neigh_col,distance+1);
                        t._prev_steps_ = cur_data._prev_steps_;
                        t._prev_steps_.push_back(std::make_pair(cur_row,cur_col));
                        BFS_queue.push(t);
                    }
                    addAction(cur_row,cur_col,actions,"Back2Prev",PlayAction::PAct_Safe);
                }
            }
        }
    }
    addAction(start_row,start_col,actions,"Imposible2find",PlayAction::PAct_Err);
    return -1;
}


//________________________________Clone Graph_______________________________________________



Snode * cloneGraph_BFS(Snode * original, string&actions)
{
    actions.append("Trying to clone graph using BFS;\n");
    if(!original)
    {
        actions.append("Input data is empty;\n");
        return nullptr;
    }
    std::map<Snode*,Snode*> cloned_map;
    std::queue<Snode*> BFS_queue;

    Snode * clone_core = new Snode;
    clone_core->_value_ = original->_value_;
    cloned_map.insert({original,clone_core});
    BFS_queue.push(original);
    int counter =1;
    actions.append("The core node with value ["+
                   std::to_string(original->_value_)+"] "
                   "has been cloned. Element count ["+
                   std::to_string(counter)+"];\n");
    while(!BFS_queue.empty())
    {
        int conn_count = 0;
        Snode * cur_node = BFS_queue.front();
        BFS_queue.pop();
        actions.append("Current element value ["+
                       std::to_string(cur_node->_value_)+"] "
                       "Element count ["+
                       std::to_string(counter)+"];\n");
        auto it = cur_node->_connections_.begin();
        while(it!=cur_node->_connections_.end())
        {
            if(!(cloned_map.count(*it)))
            {
                actions.append("The node with value ["+
                               std::to_string((*it)->_value_)+"] "
                               "has been cloned. Element count ["+
                               std::to_string(++counter)+"]\n");
                Snode * cur_copy = new Snode;
                cur_copy->_value_ = (*it)->_value_;
                cloned_map.insert({*it,cur_copy});
                BFS_queue.push(*it);
            }
            actions.append("The node with value ["+
                           std::to_string(cloned_map.at(*it)->_value_)+"] "
                           "has been added as edged to the node "
                           "with value ["+std::to_string(
                               cloned_map.at(cur_node)->_value_)+
                           "], current connection count ["+
                           std::to_string(++conn_count)+"]\n");
            cloned_map.at(cur_node)->_connections_.push_back(cloned_map.at(*it));
            it++;
        }
    }

    return clone_core;
}


bool compareSnodeGraphs(Snode * first_node, Snode* sec_node, std::unordered_map<Snode *, Snode *> &visited)
{
    if((!first_node)||(!sec_node))
    {
        return false;
    }
    if(first_node->_value_!=sec_node->_value_)
    {
        return false;
    }
    if(first_node->_connections_.size()!=sec_node->_connections_.size())
    {
        return false;
    }
    visited.insert({first_node,sec_node});
    auto r_it = first_node->_connections_.begin(),
         l_it = sec_node->_connections_.begin();


    while(r_it!=first_node->_connections_.end())
    {
        Snode * r_node = *r_it;
        Snode * l_node = *l_it;
        if(visited.count(r_node))
        {
            if(visited.at(r_node)!=l_node)
            {
                return false;
            }
        }
        else
        {
            if(!compareSnodeGraphs(r_node,l_node,visited))
            {
                return false;
            }
        }
        r_it++;
        l_it++;
    }
    return true;
}
