#ifndef NODEGRAPH_H
#define NODEGRAPH_H
#include <memory>
#include <vector>

using cur_node_type = int;
using cur_id_type = int;
using std::vector;

template< typename I,typename T>
struct GraphNode
{
    I _id_;
    T _value_;
    vector<std::weak_ptr<GraphNode>> _edges_;
    GraphNode():_value_(T()){}
    GraphNode(T t):_value_(t){}
};

using nodevector = vector<std::shared_ptr<GraphNode<cur_id_type,cur_node_type>>>;
using nodepointer = std::weak_ptr<GraphNode<cur_id_type, cur_node_type>>;


class NodeGraph
{
private:
    nodevector _nodes_;
    nodepointer _core_node_;
    bool BFS_search(cur_id_type& id, nodepointer& container);
public:
    NodeGraph();
    NodeGraph(const vector<vector<cur_node_type>>& matrix);
    ~NodeGraph();
    bool addVertex(cur_id_type& id);
    bool addEdge(cur_id_type& from, cur_id_type& to);
    void removeVertex(cur_id_type& id);
    void removeEdge(cur_id_type& from, cur_id_type& to);


    friend class NodeIterator;
};

class NodeIterator
{
private:
    nodepointer _cur_node_;
public:
    NodeIterator();
    NodeIterator(NodeGraph& graph);
    NodeIterator(NodeIterator& iter);
    NodeIterator(nodepointer& pointer);
    ~NodeIterator();
    bool isValid() const;
    bool initiate(NodeGraph& graph);
    bool moveTo(cur_id_type& id);
    bool edges(vector<cur_id_type>& container);
    cur_node_type& val();
    bool operator()(cur_id_type& id);
    cur_node_type& operator[](cur_id_type& id);

};

#endif // NODEGRAPH_H
