#include "nodegraph.h"

void NodeGraph::BFS_id_search(const cur_id_type& id, std::unordered_set<cur_id_type>& visited) const
{

    std::queue<cur_id_type> BFS_queue;
    visited.insert(id);
    BFS_queue.push(id);
    while(!BFS_queue.empty())
    {
        cur_id_type cur_id = BFS_queue.front();
        BFS_queue.pop();
        vector<cur_id_type> linked_vertices;
        getEdgeIDsAt(cur_id,linked_vertices);
        for(size_t i = 0; i< linked_vertices.size();i++)
        {
            if(!visited.count(linked_vertices.at(i)))
            {
                visited.insert(linked_vertices.at(i));
                BFS_queue.push(linked_vertices.at(i));
            }
        }
    }
    return;
}

bool NodeGraph::BFS_revert_conn_search(cur_id_type& id, std::vector<cur_id_type> &container) const
{
    for(const shar_r_node& vertex: _nodes_)
    {
        auto it = vertex->_edges_.cbegin();
        while(it!= vertex->_edges_.cend())
        {
            cur_id_type cur_id = it->first.lock()->_id_;
            if(cur_id == id)
            {
                container.push_back(cur_id);
            }
        }
    }
    return true;
}

void NodeGraph::DFS_copy(const NodeGraph &other, cur_id_type& id, std::unordered_set<cur_id_type>& visited)
{
    auto cur_other_node =  other.find_node(id);
    visited.insert(id);
    shar_r_node new_node = std::make_shared<ref_node>(cur_other_node->_id_,cur_other_node->_value_);
    for(const std::pair<std::weak_ptr<ref_node>,int>& it: cur_other_node->_edges_)
    {
        cur_id_type linked_id = it.first.lock()->_id_;
        if(!visited.count(linked_id))
        {
            DFS_copy(other,linked_id,visited);
        }
        shar_r_node linked_node = find_node(linked_id);
        new_node->_edges_.push_back(std::make_pair(linked_node,it.second));
    }
    _nodes_.push_back(new_node);
    return;
}

void NodeGraph::Object_complete_copy(const NodeGraph &other)
{
    if(other._nodes_.empty())
    {
        return;
    }
    this->clear();
    vector<std::unordered_set<cur_id_type>> segments_list;
    std::unordered_set<cur_id_type> visited;
    other.BFS_Separation(segments_list);
    for(std::unordered_set<cur_id_type>& segment:segments_list)
    {
        for(cur_id_type id: segment)
        {
            if(!visited.count(id))
            {
                DFS_copy(other,id,visited);
            }
        }
    }
    if(!other._core_node_.expired())
    {
        _core_node_ = find_node(other._core_node_.lock()->_id_);
    }
    if(_flags_&Gr_SortingById)
    {
        idSort();
    }
    return;
}

void NodeGraph::DFS_separated_copy(NodeGraph& cur_graph, cur_id_type& id,
                        std::unordered_set<cur_id_type>& visited) const
{
    auto cur_main_node = find_node(id);
    visited.insert(id);
    cur_graph.addVertex(cur_main_node->_id_,cur_main_node->_value_);
    for(std::pair<nodepointer,int>& linked_node: cur_main_node->_edges_)
    {
        cur_id_type linked_id = linked_node.first.lock()->_id_;
        if(!visited.count(linked_id))
        {
            DFS_separated_copy(cur_graph,linked_id,visited);
        }
        cur_graph.addEdge(id,linked_id,linked_node.second);
    }
}

void NodeGraph::loadSubGraph(NodeGraph& res_graph, std::unordered_set<cur_id_type>& source) const
{
    std::unordered_set<cur_id_type> cur_visited;
    for(cur_id_type id:source)
    {
        if(!cur_visited.count(id))
        {
            DFS_separated_copy(res_graph,id,cur_visited);
        }
    }
    return;
}

void NodeGraph::Object_separated_copies(vector<NodeGraph>& container)
{
    container.clear();
    if(_nodes_.empty())
    {
        return;
    }
    vector<std::unordered_set<cur_id_type>> segments_list;
    BFS_Separation(segments_list);
    for(std::unordered_set<cur_id_type>& segment: segments_list)
    {
        if(segment.empty())
        {
            continue;
        }
        NodeGraph new_graph(_flags_);
        loadSubGraph(new_graph,segment);
        if(_flags_&Gr_SortingById)
        {
            new_graph.idSort();
        }
        container.push_back(new_graph);
//        NodeGraph new_graph(_flags_);
//        for(cur_id_type id: segment)
//        {
//            shar_r_node cur_node(find_node(id));
//            new_graph.addVertex(id,cur_node->_value_);
//        }
//        for(cur_id_type id: segment)
//        {
//            shar_r_node cur_node(find_node(id));
//            for(size_t i = 0; i<cur_node->_edges_.size();i++)
//            {
//                new_graph.addEdge(id,cur_node->_edges_.at(i).first.lock()->_id_,cur_node->_edges_.at(i).second);
//            }
//        }
//        container.push_back(new_graph);
    }
    return;
}

void NodeGraph::BFS_Separation(vector<std::unordered_set<cur_id_type>>& segment_list) const
{
    segment_list.clear();
    if(!_nodes_.size())
    {
        return;
    }
    for(const shar_r_node& node: _nodes_)
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
        segment_list.push_back(cur_graph);
    }

    for(size_t i = 0; i<segment_list.size();i++)
    {
        for(size_t j = i+1; j<segment_list.size();j++)
        {
            auto it = segment_list.at(i).begin();
            while(it!=segment_list.at(i).end())
            {
                if(segment_list.at(j).count(*it))
                {
                    segment_list.at(i).merge(segment_list.at(j));
                    segment_list.at(j).clear();
                    break;
                }
                it++;
            }
        }
    }

    auto g_it = segment_list.begin();
    while(g_it!=segment_list.end())
    {
        if(g_it->empty())
        {
            g_it = segment_list.erase(g_it);
        }
        else
        {
            g_it++;
        }
    }
    return;
}

int NodeGraph::BFS_SegmentSize(const cur_id_type& id)
{
    vector<std::unordered_set<cur_id_type>> segment_list;
    BFS_Separation(segment_list);
    for(std::unordered_set<cur_id_type>& segment:segment_list)
    {
        if(segment.count(id))
        {
            return static_cast<int>(segment.size());
        }
    }
    return 0;
}

int NodeGraph::segmentCount() const
{
    vector<std::unordered_set<cur_id_type>> segment_list;
    BFS_Separation(segment_list);
    return static_cast<int>(segment_list.size());
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

NodeGraph& NodeGraph::operator=(const NodeGraph &other)
{
    Object_complete_copy(other);
    return *this;
}

NodeGraph& NodeGraph::operator=(const vector<vector<cur_node_type>>& matrix)
{
    fill(matrix);
    return *this;
}

NodeGraph::NodeGraph(const NodeGraph &other):_flags_(other._flags_)
{
    Object_complete_copy(other);
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

bool NodeGraph::setCoreVertex(const cur_id_type& id)
{
    if(!isExists(id))
    {
        return false;
    }
    _core_node_ = find_node(id);
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

bool NodeGraph::getEdgeIDsAt(const cur_id_type& id, vector<cur_id_type>& container) const
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

NodeGraph NodeGraph::getSubGraph(const cur_id_type& id) const
{
    NodeGraph new_graph(_flags_);
    vector<std::unordered_set<cur_id_type>> segments_list;
    BFS_Separation(segments_list);
    for(std::unordered_set<cur_id_type>& segment: segments_list)
    {
        if(segment.count(id))
        {
            loadSubGraph(new_graph,segment);
            break;
        }
    }
    if(_flags_&Gr_SortingById)
    {
        new_graph.idSort();
    }
    return new_graph;
}

cur_id_type NodeGraph::findCoreId() const
{
    if(_nodes_.empty())
    {
        return -1;
    }
    vector<cur_id_type> id_vector;
    getIDList(id_vector);
    std::unordered_set<cur_id_type> id_set(id_vector.cbegin(),id_vector.cend());
    vector<std::unordered_set<cur_id_type>> marchroutes;
    for(size_t i = 0; i< id_vector.size();i++)
    {
        std::unordered_set<cur_id_type> cur_set;
        BFS_id_search(id_vector.at(i),cur_set);
        if(id_set == cur_set)
        {
            return id_vector.at(i);
        }
    }
    return -1;
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

const shar_r_node& NodeGraph::find_node(const cur_id_type& id) const
{
    return *std::find_if(_nodes_.cbegin(),_nodes_.cend(),[id](const shar_r_node& node){return node->_id_==id;});
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


//_____________Node iterator___________________


nodepointer NodeIterator::findConn(const cur_id_type& id)
{
    if(_cur_node_.expired())
    {
        return nodepointer();
    }
    for(std::pair<nodepointer,int>& edge: _cur_node_.lock()->_edges_)
    {
        if(edge.first.lock()->_id_==id)
        {
            return edge.first;
        }
    }
    return nodepointer();
}

NodeIterator::NodeIterator()
{
    return;
}

NodeIterator::NodeIterator(NodeGraph& graph)
{
    initiate(graph);
    return;
}

NodeIterator::NodeIterator(NodeIterator& other)
{
    if(!other._cur_node_.expired())
    {
        _cur_node_ = other._cur_node_;
    }
    return;
}

NodeIterator::NodeIterator(nodepointer& pointer)
{
    if(!pointer.expired())
    {
        _cur_node_ = pointer;
    }
    return;
}

NodeIterator::~NodeIterator()
{

}

bool NodeIterator::isValid() const
{
    return !_cur_node_.expired();
}

bool NodeIterator::initiate(NodeGraph& graph)
{
    if(graph._core_node_.expired())
    {
        if(graph._nodes_.size())
        {
            _cur_node_ = graph._nodes_.at(0);
            return true;
        }
    }
    else
    {
        _cur_node_ = graph._core_node_;
        return true;
    }
    return false;
}

bool NodeIterator::moveToID(const cur_id_type& id)
{
    if(_cur_node_.expired())
    {
        return false;
    }
    nodepointer next_pointer = findConn(id);
    if(!next_pointer.expired())
    {
        _cur_node_ = next_pointer;
        return true;
    }
    return false;
}

bool NodeIterator::moveToNum(int number)
{
    if(_cur_node_.expired())
    {
        return false;
    }
    auto & edges = _cur_node_.lock()->_edges_;
    if(number>=0 && number<static_cast<int>(edges.size()))
    {
        _cur_node_ = edges.at(number).first;
        return true;
    }
    return false;
}

int NodeIterator::edgesCount() const
{
    if(_cur_node_.expired())
    {
        return 0;
    }
    return static_cast<int>(_cur_node_.lock()->_edges_.size());
}

bool NodeIterator::getEdgesIDs(vector<cur_id_type>& container) const
{
    container.clear();
    if(_cur_node_.expired())
    {
        return false;
    }
    auto & edges = _cur_node_.lock()->_edges_;
    for(std::pair<nodepointer,int>& edge:edges)
    {
        container.push_back(edge.first.lock()->_id_);
    }
    return true;
}

bool NodeIterator::getEdgesList(vector<pair<cur_id_type,int>>& container) const
{
    container.clear();
    if(_cur_node_.expired())
    {
        return false;
    }
    auto & edges = _cur_node_.lock()->_edges_;
    for(std::pair<nodepointer,int>& edge:edges)
    {
        container.push_back(std::make_pair(edge.first.lock()->_id_,edge.second));
    }
    return true;
}

bool NodeIterator::hasLinkTo(const cur_id_type& id)
{
    if(_cur_node_.expired())
    {
        return false;
    }
    nodepointer pt = findConn(id);
    if(!pt.expired())
    {
        return true;
    }
    return false;
}

cur_node_type& NodeIterator::val()
{
    if(_cur_node_.expired())
    {
        throw "NodeIterator uninitialized value access error!";
    }
    return _cur_node_.lock()->_value_;
}

cur_id_type NodeIterator::id() const
{
    if(_cur_node_.expired())
    {
        throw "NodeIterator uninitialized id access error!";
    }
    return _cur_node_.lock()->_id_;
}

int NodeIterator::weight(const cur_id_type& linked_id) const
{
    if(_cur_node_.expired())
    {
        return -1;
    }
    for(std::pair<nodepointer,int>& edge: _cur_node_.lock()->_edges_)
    {
        if(edge.first.lock()->_id_==linked_id)
        {
            return edge.second;
        }
    }
    return -1;
}

nodepointer& NodeIterator::operator->()
{
    return _cur_node_;
}

NodeIterator& NodeIterator::operator()(int number)
{
    moveToNum(number);
    return *this;
}
NodeIterator& NodeIterator::operator[](const cur_id_type& id)
{
    moveToID(id);
    return *this;
}
NodeIterator& NodeIterator::operator=(NodeIterator& other)
{
    if(other._cur_node_.expired())
    {
        _cur_node_.reset();
    }
    else
    {
        _cur_node_ = other._cur_node_;
    }
    return *this;
}

NodeIterator& NodeIterator::operator=(nodepointer& other)
{
    if(other.expired())
    {
        _cur_node_.reset();
    }
    else
    {
        _cur_node_ = other;
    }
    return *this;
}
