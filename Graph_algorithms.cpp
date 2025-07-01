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
