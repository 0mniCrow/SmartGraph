#include "nodegraph.h"

bool NodeGraph::BFS_search(cur_id_type& id, nodepointer& container)
{
    // тут магчыма стварыць пошук, шырынны ці глыбінны, дзе шукаецца, якія вяршыні маюць сувязь з гэтым аб'ектам.
    // ці ёсць сэнс рабіць двунакіраваныя ноды, не толькі з наступнымі, але і з якімі мелі папярэднюю сувязь?
    return true;
}

bool NodeGraph::BFS_copy(const NodeGraph& other)
{
    this->clear();
    for(const std::shared_ptr<GraphNode<cur_id_type,cur_node_type>>& node: other._nodes_)
    {
        shar_r_node new_node = std::make_shared<ref_node>(node->_id_,node->_value_);
        _nodes_.push_back(new_node);
        //ненене, зараз магчыма стварыць Глыбінны пошук з логіцай "Ствараеш новы нод, глядзіш яго грані і калі не знаходзіш такі ў сьпісе, ствараеш яго. О.о.о!
    }
    return true;
}

int NodeGraph::BFS_SegmentCount()
{
    if(!_nodes_.size())
    {
        return 0;
    }
    vector<std::unordered_set<cur_id_type>> graphs;
    for(shar_r_node& node: _nodes_)
    {
        std::unordered_set<cur_id_type> cur_graph;
        std::queue<cur_id_type> BFS_queue;
        BFS_queue.push(node->_id_);
        while(!BFS_queue.empty())
        {
            cur_id_type cur_id = BFS_queue.front();
            BFS_queue.pop();
            cur_graph.insert(cur_id);
            auto cur_node = find_node(cur_id);
            for(size_t i = 0; i<cur_node->_edges_.size();i++)
            {
                cur_id_type edged_node = cur_node->_edges_.at(i).first.lock()->_id_;
                if(!cur_graph.count(edged_node))
                {
                    BFS_queue.push(edged_node);
                }
            }
        }
        graphs.push_back(cur_graph);
    }

    for(size_t i = 0; i<graphs.size();i++)
    {
        for(size_t j = i+1; j<graphs.size();j++)
        {
            auto it = graphs.at(i).begin();
            while(it!=graphs.at(i).end())
            {
                if(graphs.at(j).count(*it))
                {
                    graphs.at(i).merge(graphs.at(j));
                    graphs.at(j).clear();
                    break;
                }
                it++;
            }
        }
    }

    auto g_it = graphs.begin();
    while(g_it!=graphs.end())
    {
        if(g_it->empty())
        {
            g_it = graphs.erase(g_it);
        }
        else
        {
            g_it++;
        }
    }
    return graphs.size();
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
    if(_flags_&Gr_SortingById)
    {
        idSort();
    }
    return true;
}

bool NodeGraph::addEdge(const cur_id_type& from, const cur_id_type& to, int weight)
{
    if(!isExists(from)||!isExists(to))
    {
        return false;
    }
    nodepointer node_from(find(from));
    node_from.lock()->_edges_.push_back({find(to),weight});
    if(!(_flags_&Gr_Directed))
    {
        nodepointer node_to(find(to));
        node_to.lock()->_edges_.push_back({find(from),weight});
    }
    return true;
}

void NodeGraph::removeVertex(const cur_id_type& id)
{
    auto it = _nodes_.begin();
    while(it!=_nodes_.end())
    {
        if((*it)->_id_==id)
        {
            break;
        }
        it++;
    }
    if(it!=_nodes_.end())
    {
        if(!_core_node_.expired()&& _core_node_.lock()->_id_==id)
        {
            _nodes_.erase(it);
            if(_nodes_.size())
            {
                _core_node_ = _nodes_.at(0);
            }
        }
        else
        {
            _nodes_.erase(it);
        }
        for(it = _nodes_.begin();it!=_nodes_.end();it++)
        {
            for(auto jt = (*it)->_edges_.begin(); jt!=(*it)->_edges_.end();jt++)
            {
                if(jt->first.expired())
                {
                    (*it)->_edges_.erase(jt);
                    break;
                }
            }
        }
    }
    return;
}

void NodeGraph::removeVertex(NodeIterator& it)
{
    removeVertex(it.id());
    return;
}


void NodeGraph::removeEdge(const cur_id_type& from, const cur_id_type& to)
{
    if(!isExists(from) || !isExists(to))
    {
        return;
    }
    auto node_from(find(from).lock());
    auto node_iter (std::find_if(
                        node_from->_edges_.begin(),
                        node_from->_edges_.end(),
                        [to](std::pair<nodepointer,int>& val)
                    { return val.first.lock()->_id_==to;}));
    if(node_iter!=node_from->_edges_.end())
    {
        node_from->_edges_.erase(node_iter);
    }
    if(!(_flags_&Gr_Directed))
    {
        auto node_to(find(to).lock());
        node_iter = (std::find_if(
                         node_to->_edges_.begin(),
                         node_to->_edges_.end(),
                         [from](std::pair<nodepointer,int>& val)
                     { return val.first.lock()->_id_==from;}));
        if(node_iter!=node_to->_edges_.end())
        {
            node_to->_edges_.erase(node_iter);
        }
    }
    return;
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
    if(!isExists(id))
    {
        return DEFAULT_NODE_VAL;
    }
    for(const shar_r_node& node: _nodes_)
    {
        if(node->_id_==id)
        {
            return node->_value_;
        }
    }
    return DEFAULT_NODE_VAL;
}

void NodeGraph::getIDList(vector<cur_id_type>& container) const
{
    container.clear();
    for(const shar_r_node& node:_nodes_)
    {
        container.push_back(node->_id_);
    }
    return;
}

bool NodeGraph::getEdgeIDsAt(const cur_id_type& id, vector<cur_id_type>& container)
{
    if(!isExists(id))
    {
        return false;
    }
    container.clear();
    auto node(find_node(id));
    for(auto& edge: node->_edges_)
    {
        container.push_back(edge.first.lock()->_id_);
    }
    return true;
}

bool NodeGraph::getEdgesListAt(const cur_id_type& id,vector<pair<cur_id_type,int>>& container)
{
    if(!isExists(id))
    {
        return false;
    }
    container.clear();
    auto node(find_node(id));
    for(auto& edge: node->_edges_)
    {
        container.push_back({edge.first.lock()->_id_,edge.second});
    }
    return true;
}

bool NodeGraph::setValue(const cur_id_type& id, cur_node_type& value)
{
    if(!isExists(id))
    {
        return false;
    }
    auto node(find_node(id));
    node->_value_ = value;
    return true;
}

void NodeGraph::fill(const cur_node_type& def_val)
{
    for(shar_r_node& node:_nodes_)
    {
        node->_value_ = def_val;
    }
    return;
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
    if(!isExists(id))
    {
        throw "Node-graph can't find recieved id";
    }
    return find_node(id)->_value_;
}

cur_node_type& NodeGraph::at(const cur_id_type& id)
{
    return operator()(id);
}

nodepointer& NodeGraph::begin()
{
    return _core_node_;
}

nodepointer NodeGraph::find(const cur_id_type& id)
{
    return find_node(id);
}

shar_r_node NodeGraph::find_node(const cur_id_type& id)
{
    return *std::find_if(_nodes_.begin(),_nodes_.end(),[id](shar_r_node& node){return node->_id_==id;});
}

int NodeGraph::size() const
{
    return static_cast<int>(_nodes_.size());
}

int NodeGraph::findFreeID() const
{
    cur_id_type counter = DEFAULT_ID_VAL;
    while(isExists(counter))
    {
        counter++;
    }
    return counter;
}

void NodeGraph::clear()
{
    _core_node_.reset();
    _nodes_.clear();
}

char NodeGraph::flags() const
{
    return _flags_;
}
void NodeGraph::setFlags(char flags)
{
    _flags_ = flags;
    return;
}
