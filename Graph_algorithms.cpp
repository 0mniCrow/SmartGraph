#include "Graph_algorithms.h"
#include "matrixmodel.h"

std::string Breadth_first_search(ListGraph& obj, int root_index)            //Паслядоўнасць "Пошук ў шырыню"
{                                                                           //Адлюстроўвае ўсе выключныя вузы графа.
    std::string answer;                                                     //Радок адказу
    std::queue<int> BFS_queue;                                              //Чарга "па-узроўневага" праходу праз граф
    std::unordered_set<int> visited_vertices;                               //Ненакіраваны набор значэнняў, захоўвае ўжо наведаныя вузы графу
    std::map<int,GraphVertice>& cur_graph = obj._list_graph_;               //Выцягваем кантэйнер графа з аб'екта графа-спіса
    auto it = cur_graph.find(root_index);                                   //Шукаем пачатковую вузу, з якой пачнем абыход
    if(it==cur_graph.end())
    {
        return answer;
    }
    answer.append("Breadth first search (list-based graph):\n");
    answer.append("Root index: "+std::to_string(root_index)+";\n");
    visited_vertices.insert(it->first);                                     //Дадаем каранёвую вузу ў "наведаныя"
    BFS_queue.push(root_index);                                             //Уцісківаем каранувую вузу ва ў чаргу
    while(!BFS_queue.empty())                                               //Пакуль у чарзе застаюцца "ненаведаныя" вузы,
    {                                                                       //Мы наведваем ды адзначаем іх
        int cur_index = BFS_queue.front();                                  //Выцягваем індэкс бягучай вузы з чаргі
        BFS_queue.pop();                                                    //Выдаляем індэкс з чаргі
        auto connected_indexes = cur_graph.at(cur_index)._edges_.begin();   //Ствараем ітэратар са сьпісу ўсіх злучаных вузаў графа.
        answer.append("Visited["+std::to_string(cur_index)+"], ");          //Адзначаем ў адказе індэкс наведанай вузы
        while(connected_indexes!=cur_graph.at(cur_index)._edges_.end())
        {
            if(visited_vertices.find(connected_indexes->first)==visited_vertices.end())
            {                                                               //Калі гэты індэкс не сустракаўся раней(не знаходзіцца ў наборы наведаных)
                visited_vertices.insert(connected_indexes->first);          //Дадаем яго ў набор наведаных
                BFS_queue.push(connected_indexes->first);                   //Уціскваем яго ў канец чаргі
            }
            connected_indexes++;                                            //Наступная злучаная вуза.
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
    std::vector<std::vector<int>>& graph_matrix = obj._adj_matrix_;
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
        std::vector<int>& node = graph_matrix.at(cur_index);
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
    std::string Deep_search(std::map<int,GraphVertice>& list_graph,
                            std::unordered_set<int>& visited_vertices,
                            int cur_index)
    {
        std::string answer;                                                     //Радок адказу
        visited_vertices.insert(cur_index);                                     //Дадаем бягучую вяршыню ў набор наведанных вяршынь
        answer.append(std::to_string(cur_index)+", ");                          //Дадаем у адказ бягучую вершыню.
        auto conn = list_graph.at(cur_index)._edges_.begin();                   //Наведваем кожную злучаную вершыню
        while(conn != list_graph.at(cur_index)._edges_.end())                   //са сьпісу сувязей
        {
            if(visited_vertices.find(conn->first)==visited_vertices.end())      //Калі гэта вяршыня не была наведана раней
            {
                answer.append("| ("+std::to_string(cur_index)+")-["+            //дадаем у адказ індэкс бягучай вяршыні,
                              std::to_string(conn->second)+"]->"+               //вагу сувязі паміж вяршынямі,
                              Deep_search(list_graph,visited_vertices,conn->first));    //і адказ рэкурсіўнага выкліку са злучанай
            }                                                                   //вяршыняй у якасьці бягучага індэксу.
            conn++;
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
    answer.append(GrAlg::Deep_search(obj._list_graph_,visited_vertices,root_index));
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
