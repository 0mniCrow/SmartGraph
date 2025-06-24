#ifndef LISTGRAPH_H
#define LISTGRAPH_H

#include <map>
#include <list>
#include <QString>
class ListGraph
{
private:
    std::map<int,std::list<int>> adj_graph;
public:
    ListGraph();
    void addEdge(int source, int destination);
    QString get() const;
};

#endif // LISTGRAPH_H
