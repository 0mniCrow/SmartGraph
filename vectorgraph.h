#ifndef VECTORGRAPH_H
#define VECTORGRAPH_H
#include <vector>
#include <QString>

class VectorGraph
{
private:
    std::vector<std::vector<int>> adj_matrix;
public:
    explicit VectorGraph(int vertices);
    void addEdge(int left, int right);
    QString get() const;
};

#endif // VECTORGRAPH_H
