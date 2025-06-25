#include "listgraph.h"


ListGraph::ListGraph(int vertices, char params):_flags_(params)
{
    readjustGraph(vertices);
    return;
}

void ListGraph::readjustGraph(int vertices)
{
    _list_graph_.clear();
    for(int i = 0; i<vertices;i++)
    {
        _list_graph_.insert({i,GraphVertice()});
        _list_graph_.at(i)._id_=i;
    }
    return;
}

void ListGraph::addEdge(int source, int destination, int weight)
{
    setWeight(source,destination,weight);
    //adj_graph[source].push_back(destination);       //true for (source node num)->(destination node num)
    //adj_graph[destination].push_back(source);       //true for (source node num)<-(destination node num) - backtracking for the Undirected graph
    return;
}

void ListGraph::setWeight(int source, int destination, int weight)
{
    std::map<int,GraphVertice>::iterator source_it = _list_graph_.find(source);
    std::map<int,GraphVertice>::iterator dest_it = _list_graph_.find(destination);
    if(source_it==_list_graph_.end())
    {
        _list_graph_.insert({source,GraphVertice()});
        _list_graph_.at(source)._id_=source;
    }
    if(dest_it==_list_graph_.end())
    {
        _list_graph_.insert({destination,GraphVertice()});
        _list_graph_.at(destination)._id_=destination;
    }

    std::map<int,int>::iterator source_edge(source_it->second._edges_.find(destination));
    if(source_edge==source_it->second._edges_.end())
    {
        source_it->second._edges_.insert({destination,weight});
    }
    else
    {
        source_it->second._edges_.at(destination)=weight;
    }
    if(!(_flags_&Gr_Directed))
    {
        std::map<int,int>::iterator dest_edge(dest_it->second._edges_.find(source));
        if(dest_edge==dest_it->second._edges_.end())
        {
            dest_it->second._edges_.insert({source,weight});
        }
        else
        {
            dest_it->second._edges_.at(source)=weight;
        }
    }
    return;
}

int ListGraph::addVertex(int id)
{
    std::map<int,GraphVertice>::iterator max_index;
    if(id>=0)
    {
        max_index=_list_graph_.find(id);
        if(max_index!=_list_graph_.end())
        {
            return -1;
        }
    }
    else
    {
        max_index = std::max_element(_list_graph_.begin(),_list_graph_.end(),
                                     [](const std::pair<int,GraphVertice>& a,
                                     const std::pair<int,GraphVertice>& b)
        {
            return a.first<b.first;
        });
        id = max_index->first+1;
    }
    _list_graph_.insert({id,GraphVertice()});
    return id;
}

void ListGraph::removeVertex(int id)
{
    std::map<int,GraphVertice>::iterator rem_it(_list_graph_.find(id));
    if(rem_it==_list_graph_.end())
    {
        return;
    }
    _list_graph_.erase(rem_it);
    for(rem_it=_list_graph_.begin();rem_it!=_list_graph_.end();rem_it++)
    {
        rem_it->second._edges_.erase(id);
    }
    return;
}

int ListGraph::getSize() const
{
    return static_cast<int>(_list_graph_.size());
}
void ListGraph::setParams(GraphParams flags)
{
    _flags_ = flags;
    return;
}
char ListGraph::getParams() const
{
    return _flags_;
}

std::string ListGraph::getAdjacencyList() const
{
    std::string answer("Adjacency list for the Graph:\n");
    std::map<int,GraphVertice>::const_iterator it = _list_graph_.cbegin();
    while(it!=_list_graph_.cend())
    {
        answer.append(std::to_string(it->first)+":{");
        std::map<int,int>::const_iterator edge = it->second._edges_.cbegin();
        while(edge!=it->second._edges_.cend())
        {
            answer.append(std::to_string(edge->first));
            if(_flags_&Gr_Weighted)
            {
                answer.append("("+std::to_string(edge->second)+")");
            }
            edge++;
            if(edge!=it->second._edges_.cend())
            {
                answer.append(", ");
            }
        }
        answer.append("};\n");
        it++;
    }
    return answer;
}
std::string ListGraph::getEdgeTable() const
{
    std::string answer("Edge Table for the Graph:\n");
    std::map<int,GraphVertice>::const_iterator it = _list_graph_.cbegin();
    while(it!=_list_graph_.cend())
    {
        std::map<int,int>::const_iterator edge = it->second._edges_.cbegin();
        while(edge!=it->second._edges_.cend())
        {
            answer.append(std::to_string(it->first));
            if(_flags_&Gr_Weighted)
            {
                answer.append("-("+std::to_string(edge->second)+")->");
            }
            else
            {
                answer.append("->");
            }
            answer.append(std::to_string(edge->first));
            edge++;
            if(edge!=it->second._edges_.cend())
            {
                answer.append(", ");
            }
        }
        answer.append("\n");
        it++;
    }
    return answer;
}
