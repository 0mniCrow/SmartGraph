#include "Graph_algorithms.h"

std::string Breadth_first_search(ListGraph& obj, int root_index)
{
    std::string answer;
    std::queue<int> BFS_queue;
    std::unordered_set<int> visited_vertices;
    std::map<int,GraphVertice>& cur_graph = obj._list_graph_;
    auto it = cur_graph.find(root_index);
    if(it==cur_graph.end())
    {
        return answer;
    }
    answer.append("Breadth first search:\n");
    answer.append("Root index: "+std::to_string(root_index)+";\n");
    visited_vertices.insert(it->first);
    BFS_queue.push(root_index);
    while(!BFS_queue.empty())
    {
        int cur_index = BFS_queue.front();
        BFS_queue.pop();
        auto connected_indexes = cur_graph.at(cur_index)._edges_.begin();
        answer.append("Visited["+std::to_string(cur_index)+"], ");
        while(connected_indexes!=cur_graph.at(cur_index)._edges_.end())
        {
            if(visited_vertices.find(connected_indexes->first)==visited_vertices.end())
            {
                visited_vertices.insert(connected_indexes->first);
                BFS_queue.push(connected_indexes->first);
            }
            connected_indexes++;
        }
    }
    answer.append("\nSearch finished.");
    return answer;
}

std::string Breadth_first_search(VectorGraph& obj, int root_index)
{
    std::string answer;
    std::queue<int> BFS_queue;
    return answer;
}
