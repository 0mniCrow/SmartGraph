#ifndef GRAPH_ALGORITHMS_SUPPLY_H
#define GRAPH_ALGORITHMS_SUPPLY_H

#include <cstring>
#include <list>
#include <vector>
#include <string>

using std::vector;

struct LandNode
{
    int _land_type_;
    bool _visited_;

    LandNode()
    {
        _land_type_ = 0;
        _visited_ = false;
    }
};

struct Atl_Pac_Node
{
    int _value_;
    vector<bool> _visited_;
    Atl_Pac_Node()
    {
       _value_=0;
    }
};

template <typename T> class Vector2D
{
private:
    std::vector<std::vector<T>> _data_;
public:
    Vector2D()
    {
        return;
    }
    Vector2D(unsigned int r, unsigned int c, const T& val = T()):_data_(r,std::vector<T>(c,val))
    {
        return;
    }
    int rowCount() const
    {
        return static_cast<int>(_data_.size());
    }
    int colCount()const
    {
        if(rowCount())
        {
            return static_cast<int>(_data_.at(0).size());
        }
        return 0;
    }
    void resize(unsigned int r, unsigned int c)
    {
        _data_.resize(r);
        for(std::vector<T>& row:_data_)
        {
            row.resize(c);
        }
        return;
    }
    void clear()
    {
        _data_.clear();
        return;
    }
    T& operator()(int i, int j)
    {
        return _data_.at(i).at(j);
    }
    const std::vector<T>& operator[](int r)
    {
        return _data_.at(r);
    }
    void fill(const T& value)
    {
        for(auto row:_data_)
            for(T& cell:row)
                cell = value;
    }
    void fill(const std::vector<std::vector<T>>& data)
    {
        int data_size = static_cast<int>(data.size());
        int loc_data_size = static_cast<int>(_data_.size());
        if(data_size!=loc_data_size)
        {
            _data_.resize(data_size);
            for(int i = 0; i<data_size;i++)
            {
                if(_data_.at(i).size()!=data.at(i).size())
                {
                    _data_.at(i).resize(data.at(i).size());
                }
            }
        }
        for(int i = 0; i<data_size;i++)
        {
            int col_size = static_cast<int>(data.at(i).size());
            for(int j = 0; j<col_size;j++)
            {
                _data_.at(i).at(j) = data.at(i).at(j);
            }
        }
        return;
    }
    bool find(const T& val, int& row, int& col)
    {
        int r_size = static_cast<int>(_data_.size());
        if(r_size)
        {
            int c_size = static_cast<int>(_data_.at(0).size());
            if(c_size)
            {
                for(int i = 0; i<r_size;i++)
                    for(int j = 0; j<c_size;j++)
                        if(_data_.at(i).at(j)==val)
                        {
                            row = i;
                            col = j;
                            return true;
                        }
            }
        }
        return false;
    }
};

template<typename T>
struct ListNode
{
    T _value_;
    std::list<ListNode*> _connections_;
    ListNode():_value_(T()){}
    ListNode(const T& t):_value_(t){}
};


class TCGraph
{
    int size;
    bool** traverce_map;
    std::list<int>* adjacency;
    void DFSUtil(int u, int v, std::string& actions)
    {
        actions.append("Curr vertex ["+std::to_string(u)+"] now edged with ["+std::to_string(v)+"]\n");
        traverce_map[u][v] = true;
        actions.append("Vertex ["+std::to_string(v)+"] is");
        if(!adjacency[v].size())
        {
            actions.append("n't edged\n");
        }
        else
        {
            actions.append(" edged with: ");
            for(int i: adjacency[v])
            {
                actions.append("["+std::to_string(i)+"], ");
            }
            actions.pop_back();
            actions.pop_back();
            actions.append("\n");
        }

        for(int i: adjacency[v])
        {
            if(!traverce_map[u][i])
            {
                actions.append("The vertice ["+std::to_string(u)+"] isn't edged with ["+std::to_string(i)+"] yet\n");
                DFSUtil(u,i, actions);
                actions.append("Back to ["+std::to_string(u)+"]--["+std::to_string(v)+"] edge\n");
            }
            else
            {
                actions.append("The vertice ["+std::to_string(u)+"]already has been edged with ["+std::to_string(i)+"]\n");
            }

        }
        return;
    }
public:
    TCGraph(int vert)
    {
        size = vert;
        adjacency = new std::list<int>[vert];
        traverce_map = new bool* [vert];
        for(int i = 0; i<vert;i++)
        {
            traverce_map[i] = new bool[vert];
            memset(traverce_map[i],false,vert*sizeof(bool));
        }
        return;
    }
    ~TCGraph()
    {
        delete [] adjacency;
        for(int i = 0; i<size;i++)
        {
            delete [] traverce_map[i];
        }
        delete [] traverce_map;
        return;
    }
    void addEdge(int v, int w)
    {
        adjacency[v].push_back(w);
        return;
    }
    void transitiveClosure(std::string& actions)
    {
        for(int i = 0; i<size; i++)
        {
            DFSUtil(i,i, actions);
        }
        for(int i = 0; i<size; i++)
        {
            for(int j = 0; j<size; j++)
            {
                actions.append(std::to_string(traverce_map[i][j])+" ");
            }
            actions.pop_back();
            actions.append("\n");
        }
        return;
    }
};


#endif // GRAPH_ALGORITHMS_SUPPLY_H
