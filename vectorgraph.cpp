#include "vectorgraph.h"


VectorGraph::VectorGraph(int vertices):_adj_matrix_(vertices,std::vector<int>(vertices,0))
{
    _flags_=Gr_Unweighted_Undirected;
    return;
}

VectorGraph::VectorGraph(int vertices, char params):
    _adj_matrix_(vertices,std::vector<int>(vertices,0)),
    _flags_(params)
{
    return;
}

void VectorGraph::setParams(char flags)
{
    _flags_=flags;
    return;
}

char VectorGraph::getParams() const
{
    return _flags_;
}

void VectorGraph::addEdge(int source, int destination, int weight)
{
    setWeight(source,destination,weight);
    return;
}

void VectorGraph::setWeight(int source, int destination, int weight)
{
    int size = getSize();
    if((size>=source)&&
        (size>=destination))
    {
        _adj_matrix_.at(source).at(destination) = weight;              //true for (source node num)->(destination node num)
        _flags_&Gr_Directed?:_adj_matrix_.
                            at(destination).at(source) = weight;       //true for (source node num)<-(destination node num) - backtracking link for the Undirected graph
    }
    return;
}

int VectorGraph::addVertex()
{
    int size(getSize());
    _adj_matrix_.push_back(std::vector<int>(size,0));
    for(std::vector<int>& row: _adj_matrix_)
    {
        row.push_back(0);
    }
    return size;
}

int VectorGraph::getSize() const
{
    return static_cast<int>(_adj_matrix_.size());
}

std::vector<std::vector<int>> VectorGraph::getMatrix() const
{
    return _adj_matrix_;
}

std::string VectorGraph::getAdjacencyMatrix() const
{
    std::string answer("Adjacency Matrix for the Graph:\n");
    for(std::size_t i = 0; i<_adj_matrix_.size();i++)
    {
        for(std::size_t j = 0; j<_adj_matrix_.size();j++)
        {
            answer.append(std::to_string(_adj_matrix_.at(i).at(j))+" ");
        }
        answer.append("\n");
    }
    return answer;
}

std::string VectorGraph::getEdgeTable() const
{
    std::string answer("Edge Table for the Graph:\n");
    for(std::size_t i = 0; i<_adj_matrix_.size();i++)
    {
        for(std::size_t j = 0; j<_adj_matrix_.size();j++)
        {
            if(_adj_matrix_.at(i).at(j))
            {
                answer.append(std::to_string(i));
                if(_flags_&Gr_Weighted)
                {
                    answer.append("-("+std::to_string(_adj_matrix_.at(i).at(j))+")->");
                }
                else
                {
                    answer.append("->");
                }
                answer.append(std::to_string(j)+", ");
            }
        }
        answer.append("\n");
    }
    return answer;
}
