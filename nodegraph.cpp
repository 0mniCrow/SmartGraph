#include "nodegraph.h"

bool NodeGraph::BFS_search(cur_id_type& id, nodepointer& container)
{
    return true;
}

bool NodeGraph::BFS_copy(const NodeGraph& other)
{
    return true;
}

void NodeGraph::DFS_fill(const vector<vector<cur_node_type>>& matrix, int node_num)
{
    _nodes_.push_back(std::make_shared<ref_node>(static_cast<cur_id_type>(node_num)));
    for(size_t i = 0; i<matrix.at(node_num).size();i++)
    {
        if(matrix.at(node_num).at(i))
        {
            if(!isExists(i))
            {
                DFS_fill(matrix,i);
            }
            _nodes_.at(node_num)->_edges_.push_back({_nodes_.at(i),matrix.at(node_num).at(i)});
        }
    }
    return;
}

void NodeGraph::idSort()
{
    if(_nodes_.size())
    {
        std::sort(_nodes_.begin(), _nodes_.end(),
                  [](shar_r_node& a, shar_r_node& b){return a->_id_<b->_id_;});
    }
    return;
}

NodeGraph::NodeGraph(char flags):_flags_(flags)
{
    return;
}


NodeGraph::NodeGraph(const vector<vector<cur_node_type>>& matrix, char flags):_flags_(flags)
{
    fill(matrix);
    return;
}

NodeGraph::NodeGraph(const NodeGraph& other):_flags_(other._flags_)
{
    BFS_copy(other);
    return;
}

NodeGraph::~NodeGraph()
{
    //не ўпэўнены, што мне тут нешта трэба...
}

bool NodeGraph::addVertex(const cur_id_type& id,cur_node_type val)
{
    if(isExists(id))
    {
        return false;
    }
    _nodes_.push_back(std::make_shared<ref_node>(id,val));
    idSort();
    return true;
}

bool NodeGraph::addEdge(const cur_id_type& from, const cur_id_type& to, int weight)
{

}

void NodeGraph::removeVertex(const cur_id_type& id)
{

}

void NodeGraph::removeVertex(NodeIterator& it)
{

}
void NodeGraph::removeEdge(const cur_id_type& from, const cur_id_type& to)
{

}

bool NodeGraph::isExists(const cur_id_type& id) const
{
    auto it = _nodes_.begin();
    while(it!= _nodes_.end())
    {
        if((*it)->_id_==id)
        {
            return true;
        }
        it++;
    }
    return false;
}


cur_node_type NodeGraph::getValue(const cur_id_type& id) const
{

}

void NodeGraph::getIDList(vector<cur_id_type>& container)
{

}

bool NodeGraph::getEdgeIDsAt(const cur_id_type& id, vector<cur_id_type>& container) const
{

}

bool NodeGraph::getEdgesListAt(const cur_id_type& id,vector<pair<cur_id_type,int>>& container) const
{

}

bool NodeGraph::setValue(const cur_id_type& id, cur_node_type& value)
{

}

void NodeGraph::fill(const cur_node_type& def_val)
{

}
bool NodeGraph::fill(const vector<vector<cur_node_type>>& matrix)
{
    clear();
    if(!matrix.size())                          return false;
    if(matrix.size() != matrix.at(0).size())    return false;

    DFS_fill(matrix,0);
    idSort();
    return true;
}

cur_node_type& NodeGraph::operator()(const cur_id_type& id)
{

}
cur_node_type& NodeGraph::at(const cur_id_type& id)
{

}
nodepointer& NodeGraph::begin()
{

}
nodepointer& NodeGraph::find(const cur_id_type& id)
{

}

int NodeGraph::size() const
{

}
int NodeGraph::findFreeID() const
{

}

void NodeGraph::clear()
{

}

char NodeGraph::flags() const
{

}
void NodeGraph::setFlags(char flags)
{

}
