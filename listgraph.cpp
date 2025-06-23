#include "listgraph.h"

ListGraph::ListGraph()
{

}

void ListGraph::addEdge(int left, int right)
{
    adj_graph[left].push_back(right);
    adj_graph[right].push_back(left);
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
