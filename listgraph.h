#ifndef LISTGRAPH_H
#define LISTGRAPH_H

#include <map>
#include <list>
#include <string>
#include <algorithm>

struct GraphVertice
{
    unsigned int _id_;
    int _value_;
    std::map<int,int> _edges_;
    GraphVertice()
    {
        _id_ = 0;
        return;
    }
};

class ListGraph
{
private:
    //std::map<int,std::list<int>> adj_graph;
    std::map<int,GraphVertice> _list_graph_;
    char _flags_;
    void readjustGraph(int vertices);
public:
    enum GraphParams{ Gr_Unweighted_Undirected=0x00, Gr_Directed = 0x01, Gr_Weighted=0x02};

    explicit ListGraph(int vertices, char params=Gr_Unweighted_Undirected);
    void addEdge(int source, int destination, int weight = 1);
    void setWeight(int source, int destination, int weight);
    int addVertex(int id = -1);
    bool setValue(int vertex_id,int val);
    int getValue(int vertex_id);
    int& operator()(int vertex_id);
    void removeVertex(int id);
    int getSize() const;
    void setParams(GraphParams flags);
    char getParams() const;
    std::string getAdjacencyList() const;
    std::string getEdgeTable() const;

    friend std::string Breadth_first_search(ListGraph& obj, int root_index);
    friend std::string Depth_first_search(ListGraph& obj, int root_index);
};

#endif // LISTGRAPH_H
