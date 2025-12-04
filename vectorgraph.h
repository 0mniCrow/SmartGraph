#ifndef VECTORGRAPH_H
#define VECTORGRAPH_H
#include <vector>
#include <string>

#define DEF_INT_EDGE 1
#define DEF_INT_VAL 0

using cur_type = int;
using std::vector;
using std::pair;


using Matrix = vector<vector<cur_type>>;
using MatrIter = Matrix::iterator;
using MatrCIter = Matrix::const_iterator;

class VectorGraph
{
private:
    Matrix _adj_matrix_;
    char _flags_;
    void readjustGraph(int rows, int cols);
    void resize(int quadr_size);
public:
    enum GraphParams{ Gr_Unweighted_Undirected=0x00, Gr_Directed = 0x01, Gr_Weighted=0x02};

    VectorGraph();
    VectorGraph(int quadr_size, char flags = Gr_Unweighted_Undirected);
    VectorGraph(int quadr_size, cur_type def_val, char flags = Gr_Unweighted_Undirected);
    VectorGraph(const vector<vector<cur_type>>& model, char flags = Gr_Unweighted_Undirected);
    ~VectorGraph();

    void fill(cur_type def_val);
    void fill(const vector<vector<cur_type>>&model);


    void setFlags(char flags);
    char flags() const;
    int size() const;
    void clear();

    bool addEdge(int vert_id_src, int vert_id_dest, cur_type val = DEF_INT_EDGE);
    bool setVal(int vert_id_src, int vert_id_dest, cur_type val);

    int addVertex();
    int addVerts(int count, cur_type def_val = DEF_INT_VAL, int at = -1);
    void deleteVert(int vert_id, int count = 1);

    cur_type& operator()(int vert_id, int edge_id);
    cur_type& at(int vert_id, int edge_id);
    cur_type value(int vert_id, int edge_id) const;


    std::vector<std::vector<cur_type>> getMatrix() const;
    std::string getAdjacencyMatrix() const;
    std::string getEdgeTable() const;

    friend std::string Breadth_first_search(VectorGraph& obj, int root_index);
    friend std::string Depth_first_search(VectorGraph& obj, int root_index);
};

#endif // VECTORGRAPH_H
