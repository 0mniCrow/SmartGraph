#include "vectorgraph.h"

VectorGraph::VectorGraph(int vertices):adj_matrix(vertices,std::vector(vertices,0))
{
    return;
}
void VectorGraph::addEdge(int left, int right)
{
    int size = static_cast<int>(adj_matrix.size());
    if((size>=left)&&
            (size>=right))
    {
        adj_matrix[left][right] = 1;
        adj_matrix[right][left] = 1;
    }
    return;
}

QString VectorGraph::get() const
{
    QString answer("Adjacency Matrix for the Graph:\n");
    for(std::size_t i = 0; i<adj_matrix.size();i++)
    {
        for(std::size_t j = 0; j<adj_matrix.size();j++)
        {
            answer.append(QString::number(adj_matrix.at(i).at(j))+" ");
        }
        answer.append("\n");
    }
    return answer;
}
