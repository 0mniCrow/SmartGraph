#include "Graph_algorithms.h"

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
