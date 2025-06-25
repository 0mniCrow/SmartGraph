#ifndef VECTORGRAPH_H
#define VECTORGRAPH_H
#include <vector>
#include <string>

class VectorGraph
{
private:
    std::vector<std::vector<int>> _adj_matrix_;
    char _flags_;
public:
    enum GraphParams{ Gr_Unweighted_Undirected=0x00, Gr_Directed = 0x01, Gr_Weighted=0x02};

    explicit VectorGraph(int vertices);
    VectorGraph(int vertices, char params);
    void addEdge(int source, int destination, int weight = 1);
    void setWeight(int source, int destination, int weight);
    int addVertex();
    int getSize() const;
    void setParams(char flags);
    char getParams() const;
    std::string getAdjacencyMatrix() const;
    std::string getEdgeTable() const;
};

#endif // VECTORGRAPH_H
