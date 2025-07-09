#ifndef LISTGRAPH_H
#define LISTGRAPH_H

#include <map>
#include <list>
#include <string>
#include <algorithm>
#define DEF_VAL -1
#define DEF_WEIGHT 0

using cur_type = int;

template <typename T>
struct ListVertex
{
    T _value_;
    std::map<int,int> _edges_;
    ListVertex()
    {
        _value_=T();
        return;
    }
    ListVertex(const T& t)
    {
        _value_=t;
        return;
    }
};

class ListGraph
{
private:
    //std::map<int,std::list<int>> adj_graph;
    std::map<int,ListVertex<cur_type>> _list_graph_;
    char _flags_;
    void readjustGraph(int size);
    void setEdge(int vert_id_src, int vert_id_dest);
public:
    enum GraphParams{ Gr_Unweighted_Undirected=0x00, Gr_Directed = 0x01, Gr_Weighted=0x02};
    ListGraph();
    explicit ListGraph(int size, char params=Gr_Unweighted_Undirected);
    ListGraph(int size, cur_type def_val, char params = Gr_Unweighted_Undirected);
    bool addEdge(int vert_id_src, int vert_id_dest, int weight = 1);
    bool setWeight(int  vert_id_src, int vert_id_dest, int weight);
    int addVertex(int vert_id = -1, cur_type val = 0);
    bool setValue(int vertex_id,cur_type val);
    bool isExists(int vertex_id);
    cur_type value(int vertex_id);
    cur_type& operator()(int vertex_id);
    cur_type& at(int vertex_id);
    std::vector<int> getEdges(int vertex_id);
    const std::map<int,int>& getConnections(int vertex_id);
    int edgesCount(int vertex_id) const;
    int edgeWeightAt(int vertex_id, int edge_id) const;
    void removeVertex(int vertex_id);
    int size() const;
    void clear();
    void fill(cur_type val);
    void setParams(GraphParams flags);
    char getParams() const;
    std::string getAdjacencyList() const;
    std::string getEdgeTable() const;

    friend std::string Breadth_first_search(ListGraph& obj, int root_index);
    friend std::string Depth_first_search(ListGraph& obj, int root_index);
};

#endif // LISTGRAPH_H
