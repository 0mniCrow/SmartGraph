#include "listgraph.h"

using IntVertex = ListVertex<int>;
using IntVertIter = std::map<int,ListVertex<int>>::iterator;
using CharVertex = ListVertex<char>;
using CharVertIter = std::map<int,ListVertex<char>>::iterator;
using EdgeIter = std::map<int,int>::iterator;

using CurVertex = IntVertex;
using CurVertexIter = IntVertIter;

ListGraph::ListGraph():_flags_(Gr_Unweighted_Undirected)
{

}

ListGraph::ListGraph(int size, char params):_flags_(params)
{
    readjustGraph(size);
    return;
}

void ListGraph::readjustGraph(int size)
{
    _list_graph_.clear();
    for(int i = 0; i<size;i++)
    {
        _list_graph_.insert({i,CurVertex()});
    }
    return;
}

void ListGraph::setEdge(int vert_id_src, int vert_id_dest)
{
    CurVertexIter src_iter = _list_graph_.find(vert_id_src);
    EdgeIter src_edge = (src_iter->second._edges_.find(vert_id_dest));
    if(src_edge == src_iter->second._edges_.end())
    {
        src_iter->second._edges_.insert({vert_id_dest,DEF_WEIGHT});
    }
    if(!(_flags_&Gr_Directed))
    {
        CurVertexIter dest_iter = _list_graph_.find(vert_id_dest);
        EdgeIter dest_edge(dest_iter->second._edges_.find(vert_id_src));
        if(dest_edge==dest_iter->second._edges_.end())
        {
            dest_iter->second._edges_.insert({vert_id_src,DEF_WEIGHT});
        }
    }
    return;
}

bool ListGraph::addEdge(int vert_id_src, int vert_id_dest, int weight)
{
    if((!_list_graph_.count(vert_id_src))||(!_list_graph_.count(vert_id_dest)))
    {
        return false;
    }
    setEdge(vert_id_src,vert_id_dest);
    if(_flags_&Gr_Weighted)
    {
        if(!setWeight(vert_id_src,vert_id_dest,weight))
        {
            return false;
        }
    }
    //adj_graph[source].push_back(destination);       //true for (source node num)->(destination node num)
    //adj_graph[destination].push_back(source);       //true for (source node num)<-(destination node num) - backtracking for the Undirected graph
    return true;
}

bool ListGraph::setWeight(int vert_id_src, int vert_id_dest, int weight)
{
    CurVertexIter src_iter = _list_graph_.find(vert_id_src);
    if(src_iter == _list_graph_.end())
    {
        return false;
    }
    EdgeIter src_edges = src_iter->second._edges_.find(vert_id_dest);
    if(src_edges == src_iter->second._edges_.end())
    {
        return false;
    }
    src_edges->second = weight;
    if(!(_flags_&Gr_Directed))
    {
        CurVertexIter dest_iter = _list_graph_.find(vert_id_dest);
        if(dest_iter == _list_graph_.end())
        {
            return false;
        }
        EdgeIter dest_edges = dest_iter->second._edges_.find(vert_id_dest);
        if(dest_edges == dest_iter->second._edges_.end())
        {
            return false;
        }
        dest_edges->second = weight;
    }
    return true;
}

int ListGraph::addVertex(int vert_id, cur_type val)
{
    CurVertexIter max_index;
    if(vert_id>=0)
    {
        max_index=_list_graph_.find(vert_id);
        if(max_index!=_list_graph_.end())
        {
            return -1;
        }
    }
    else
    {
//        max_index = std::max_element(_list_graph_.begin(),_list_graph_.end(),
//                                     [](const std::pair<int,IntVertex>& a,
//                                     const std::pair<int,IntVertex>& b)
//        {
//            return a.first<b.first;
//        });
//        vert_id = max_index->first+1;
        for(CurVertexIter it = _list_graph_.begin(); it!=_list_graph_.end();it++)
        {
            CurVertexIter next = it;
            next++;
            if((next==_list_graph_.end())||(next->first!=(it->first+1)))
            {
                vert_id = it->first+1;
                break;
            }
        }
    }
    _list_graph_.insert({vert_id,CurVertex(val)});
    return vert_id;
}

bool ListGraph::setValue(int vertex_id, cur_type val)
{
    CurVertexIter it = _list_graph_.find(vertex_id);
    if(it!=_list_graph_.end())
    {
        it->second._value_ = val;
        return true;
    }
    return false;
}

cur_type ListGraph::value(int vertex_id)
{
    CurVertexIter it = _list_graph_.find(vertex_id);
    if(it!=_list_graph_.end())
    {
        return it->second._value_;
    }
    return DEF_VAL;
}

cur_type& ListGraph::operator()(int vertex_id)
{
    CurVertexIter it = _list_graph_.find(vertex_id);
    if(it!=_list_graph_.end())
    {
        return it->second._value_;
    }
    else
    {
        throw "Graph index id is out of border";
    }
}

cur_type& ListGraph::at(int vertex_id)
{
    return operator()(vertex_id);
}

std::vector<int> ListGraph::getEdges(int vertex_id)
{
    std::vector<int> edges;
    CurVertexIter vertex= _list_graph_.find(vertex_id);
    if(vertex==_list_graph_.end())
        return edges;
    for(std::pair<int,int> conn: vertex->second._edges_)
    {
        edges.push_back(conn.first);
    }
    return edges;
}

const std::map<int,int>& ListGraph::getConnections(int vertex_id)
{
    CurVertexIter it = _list_graph_.find(vertex_id);
    if(it!=_list_graph_.end())
    {
        return it->second._edges_;
    }
    else
    {
        throw "Graph index id is out of border";
    }
}


void ListGraph::removeVertex(int vertex_id)
{
    CurVertexIter rem_it(_list_graph_.find(vertex_id));
    if(rem_it==_list_graph_.end())
    {
        return;
    }
    _list_graph_.erase(rem_it);
    for(rem_it=_list_graph_.begin();rem_it!=_list_graph_.end();rem_it++)
    {
        rem_it->second._edges_.erase(vertex_id);
    }
    return;
}

int ListGraph::size() const
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
    std::map<int,CurVertex>::const_iterator it = _list_graph_.cbegin();
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
    std::map<int,CurVertex>::const_iterator it = _list_graph_.cbegin();
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
