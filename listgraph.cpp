#include "listgraph.h"

ListGraph::ListGraph()
{

}

void ListGraph::addEdge(int source, int destination)
{
    adj_graph[source].push_back(destination);       //true for (source node num)->(destination node num)
    adj_graph[destination].push_back(source);       //true for (source node num)<-(destination node num) - backtracking for the Undirected graph
    return;
}
QString ListGraph::get() const
{
    QString answer("Adjacency list for the Graph:\n");
    for(std::map<int,std::list<int>>::const_iterator i = adj_graph.cbegin(); i!=adj_graph.cend();i++)
    {
        answer.append(QString::number(i->first)+"->");
        for(std::list<int>::const_iterator j = i->second.cbegin();j!=i->second.cend();j++)
        {
           answer.append(QString::number(*j)+" ");
        }
        answer.append("\n");
    }
    return answer;
}
