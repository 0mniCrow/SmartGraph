#ifndef NODEGRAPH_H
#define NODEGRAPH_H
#include <memory>
#include <vector>
#include <queue>
#include <string>
#include <unordered_set>
#include <algorithm>

#define DEFAULT_NODE_VAL 0
#define DEFAULT_ID_VAL 0

using cur_node_type = int;
using cur_id_type = int;

using std::vector;
using std::pair;
using std::queue;

template< typename I,typename T>
struct GraphNode
{
    I _id_;
    T _value_;
    vector<std::pair<std::weak_ptr<GraphNode>,int>> _edges_;
    GraphNode():_id_(I()),_value_(T()){}
    GraphNode(I i):_id_(i),_value_(T()){}
    GraphNode(I i, T t):_id_(i),_value_(t){}
};

using ref_node = GraphNode<cur_id_type,cur_node_type>;
using shar_r_node = std::shared_ptr<ref_node>;
using nodevector = vector<shar_r_node>;
using nodepointer = std::weak_ptr<ref_node>;

class NodeIterator;

class NodeGraph
{
private:
    nodevector _nodes_;
    nodepointer _core_node_;
    char _flags_;

    void BFS_id_search(const cur_id_type& id, std::unordered_set<cur_id_type>& visited) const;
    bool BFS_revert_conn_search(cur_id_type& id, vector<cur_id_type>& container) const;
    void BFS_Separation(vector<std::unordered_set<cur_id_type>>& segment_list) const;
    int BFS_SegmentSize(const cur_id_type& id);
    void DFS_copy(const NodeGraph& other, cur_id_type& id, std::unordered_set<cur_id_type>& visited);
    void Object_complete_copy(const NodeGraph& other);
    void DFS_separated_copy(NodeGraph& cur_graph, cur_id_type& id,
                            std::unordered_set<cur_id_type>& visited) const;
    void Object_separated_copies(vector<NodeGraph>& container) const;

    void loadSubGraph(NodeGraph& res_graph, std::unordered_set<cur_id_type>& source) const;
    void DFS_fill(const vector<vector<cur_node_type>>& matrix, int node_num);
    void idSort();
    shar_r_node find_node(const cur_id_type& id);
    const shar_r_node& find_node(const cur_id_type& id) const;

public:
    enum GraphParams{ Gr_Unweighted_Undirected=0x00, Gr_Directed = 0x01, Gr_Weighted=0x02, Gr_SortingById =0x04};
    NodeGraph(char flags = Gr_Unweighted_Undirected);
    NodeGraph(const vector<vector<cur_node_type>>& matrix, char flags = Gr_Unweighted_Undirected);
    NodeGraph(const NodeGraph &other);
    NodeGraph& operator=(const NodeGraph& other);
    NodeGraph& operator=(const vector<vector<cur_node_type>>& matrix);
    ~NodeGraph();

    bool addVertex(const cur_id_type& id, cur_node_type val = DEFAULT_NODE_VAL);
    bool addEdge(const cur_id_type& from, const cur_id_type& to, int weight = 1);
    bool setCoreVertex(const cur_id_type& id);

    void removeVertex(const cur_id_type& id);
    void removeVertex(NodeIterator& it);
    void removeEdge(const cur_id_type& from, const cur_id_type& to);

    bool isExists(const cur_id_type& id) const;


    cur_node_type getValue(const cur_id_type& id) const;
    void getIDList(vector<cur_id_type>& container) const;
    bool getEdgeIDsAt(const cur_id_type& id, vector<cur_id_type>& container) const;
    bool getEdgesListAt(const cur_id_type& id,vector<pair<cur_id_type,int>>& container);
    NodeGraph getSubGraph(const cur_id_type& id) const;
    vector<NodeGraph> getSubGraphs() const;
    cur_id_type findCoreId() const;

    bool setValue(const cur_id_type& id, cur_node_type& value);
    void fill(const cur_node_type& def_val);
    bool fill(const vector<vector<cur_node_type>>& matrix);

    cur_node_type& operator()(const cur_id_type& id);
    cur_node_type& at(const cur_id_type& id);
    nodepointer& begin();
    nodepointer find(const cur_id_type& id);

    int size() const;
    int segmentCount() const;
    int findFreeID() const;

    void clear();

    char flags() const;
    void setFlags(char flags);


    std::string graphReport() const;
    friend class NodeIterator;
};

class NodeIterator
{
private:
    nodepointer _cur_node_;
    nodepointer findConn(const cur_id_type& id);
public:
    NodeIterator();
    NodeIterator(NodeGraph& graph);
    NodeIterator(NodeIterator& other);
    NodeIterator(nodepointer& pointer);
    ~NodeIterator();
    bool isValid() const;
    bool initiate(NodeGraph& graph);
    bool moveToID(const cur_id_type& id);
    bool moveToNum(int number);
    int edgesCount() const;
    bool getEdgesIDs(vector<cur_id_type>& container) const;
    bool getEdgesList(vector<pair<cur_id_type,int>>& container) const;
    bool hasLinkTo(const cur_id_type& id);
    cur_node_type& val();
    cur_id_type id() const;
    int weight(const cur_id_type& linked_id) const;
    nodepointer& operator->();
    NodeIterator& operator()(int number);
    NodeIterator& operator[](const cur_id_type& id);
    NodeIterator& operator=(NodeIterator& other);
    NodeIterator& operator=(nodepointer& other);

};

#endif // NODEGRAPH_H
