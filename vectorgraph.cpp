#include "vectorgraph.h"

VectorGraph::VectorGraph():_flags_(Gr_Unweighted_Undirected){}

VectorGraph::VectorGraph(int quadr_size,char flags):
    _adj_matrix_(quadr_size,std::vector<cur_type>(quadr_size,0)),
    _flags_(flags)
{
    return;
}

VectorGraph::VectorGraph(int quadr_size, cur_type def_val, char flags):
    _adj_matrix_(quadr_size,vector<cur_type>(quadr_size,def_val)),
    _flags_(flags)
{
    return;
}

VectorGraph::VectorGraph(const vector<vector<cur_type>>& model, char flags):
    _flags_(flags)
{
    fill(model);
}

VectorGraph::~VectorGraph()
{

}

void VectorGraph::resize(int quadr_size)
{
    int model_size = static_cast<int>(_adj_matrix_.size());
    if(quadr_size!=model_size)
    {
        _adj_matrix_.resize(quadr_size);
        for(vector<cur_type>& edges:_adj_matrix_)
        {
            edges.resize(quadr_size);
        }
    }
    return;
}

void VectorGraph::fill(cur_type def_val)
{
    for(vector<cur_type>& edges: _adj_matrix_)
    {
        for(cur_type& edge:edges)
        {
            edge = def_val;
        }
    }
    return;
}

void VectorGraph::fill(const vector<vector<cur_type>>&model)
{
    int model_size = static_cast<int>(model.size());
    if(model_size)
    {
        int edges_size = static_cast<int>(model.at(0).size());
        if(edges_size == model_size)
        {
            _adj_matrix_ = model;
        }
    }
    return;
}

void VectorGraph::setFlags(char flags) {_flags_=flags;return;}
char VectorGraph::flags() const {return _flags_;}


bool VectorGraph::addEdge(int vert_id_src, int vert_id_dest, cur_type val)
{
    return setVal(vert_id_src,vert_id_dest,val);
}

bool VectorGraph::setVal(int vert_id_src, int vert_id_dest, cur_type val)
{
    int sz = size();
    if((sz>=vert_id_src)&&
        (sz>=vert_id_dest))
    {
        _adj_matrix_.at(vert_id_src).at(vert_id_dest) = val;              //true for (source node num)->(destination node num)
        _flags_&Gr_Directed?:_adj_matrix_.
                            at(vert_id_dest).at(vert_id_src) = val;       //true for (source node num)<-(destination node num) - backtracking link for the Undirected graph
        return true;
    }
    return false;
}

int VectorGraph::size() const
{
    return static_cast<int>(_adj_matrix_.size());
}

void VectorGraph::clear()
{
    _adj_matrix_.clear();
    return;
}


int VectorGraph::addVertex()
{
    int sz(size());
    _adj_matrix_.push_back(vector<cur_type>(sz,DEF_INT_VAL));
    for(vector<cur_type>& edges: _adj_matrix_)
    {
        edges.push_back(DEF_INT_VAL);
    }
    return sz;
}

int VectorGraph::addVerts(int count, cur_type def_val, int at)
{
    MatrIter it;
    if(at>=0)
    {
        it = std::next(_adj_matrix_.begin(),at);
    }
    else
    {
        it = _adj_matrix_.end();
    }
    _adj_matrix_.insert(it,count, std::vector<cur_type>(size(),def_val));
    for(vector<cur_type>& edges: _adj_matrix_)
    {

        vector<cur_type>::iterator jt;
        if(at>=0)
        {
            jt = std::next(edges.begin(),at);
        }
        else
        {
            jt = edges.end();
        }
        edges.insert(jt,count,def_val);
    }
    return (at>=0)?at:size();
}

void VectorGraph::deleteVert(int vert_id, int count)
{
    if(vert_id<size())
    {
        _adj_matrix_.erase(std::next(_adj_matrix_.begin(),vert_id),
                           (vert_id+count<size())?
                               std::next(_adj_matrix_.begin(),vert_id+count):
                               _adj_matrix_.end());
        for(vector<cur_type>& edges: _adj_matrix_)
        {
            edges.erase(std::next(edges.begin(),vert_id),
                        (vert_id+count<size())?
                            std::next(edges.begin(),vert_id+count):
                            edges.end());
        }
    }
}

cur_type& VectorGraph::operator()(int vert_id, int edge_id)
{
    if(vert_id<size() && edge_id<size())
    {
        return _adj_matrix_.at(vert_id).at(edge_id);
    }
    else
    {
        throw "Vector graph out of border";
    }
}

cur_type& VectorGraph::at(int vert_id, int edge_id)
{
    return operator()(vert_id,edge_id);
}

cur_type VectorGraph::value(int vert_id, int edge_id) const
{
    if(vert_id<size() && edge_id<size())
    {
        return _adj_matrix_.at(vert_id).at(edge_id);
    }
    else
    {
        return DEF_INT_VAL;
    }
}


std::vector<std::vector<cur_type>> VectorGraph::getMatrix() const
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
