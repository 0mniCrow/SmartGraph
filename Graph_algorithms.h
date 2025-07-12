#ifndef GRAPH_ALGORITHMS_H
#define GRAPH_ALGORITHMS_H

#include <string>
#include <queue>
#include <unordered_set>
#include <chrono>
#include <QColor>
#include <climits>
#include "listgraph.h"
#include "matrixmodel.h"
#include "vectorgraph.h"


struct PlayAction;

std::string Breadth_first_search(ListGraph& obj, int root_index);

std::string Breadth_first_search(VectorGraph& obj, int root_index);

std::string Depth_first_search(ListGraph& obj, int root_index);

std::string Depth_first_search(VectorGraph& obj, int root_index);

bool szBordCheck(int cur_row, int cur_col, int rows_size, int cols_size);

void addAction(std::vector<std::vector<int>>& matrix,
               int active_row, int active_col,
               std::vector<PlayAction>& actions);



int orangesRotting_iteration(std::vector<std::vector<int>>& matrix, long long &time);
int orangesRotting_DFS(std::vector<std::vector<int>>& matrix, long long &time, std::string& actions);
int orangesRotting_BFS(std::vector<std::vector<int>>& matrix,
                       long long& time, std::string& actions,
                       std::vector<PlayAction>& vec_actions);

struct LandNode
{
    char _land_type_;
    bool _visited_;
    LandNode()
    {
        _land_type_ = 0;
        _visited_ = false;
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

void addAction(int active_row, int active_col,
               std::vector<PlayAction>& actions,
               std::string info,
               PlayAction::PlayActionsType actionType =
        PlayAction::PAct_Safe,
               bool is_changing = false,
               int new_val = 0);

void countIslands_DFS(Vector2D<LandNode>& matrix, int row, int col, std::vector<PlayAction>& actions);
int countIslands(Vector2D<LandNode>& matrix, std::vector<PlayAction>& actions);

void countLand_OPT_DFS(Vector2D<char>& matrix, int row, int col, std::vector<PlayAction>& actions);
int countLand_OPT(Vector2D<char>& matrix, std::vector<PlayAction>& actions);

void countLand_BFS(Vector2D<LandNode>&matrix, int row, int col, std::vector<PlayAction>& actions);
int countLand_BFS_based(Vector2D<LandNode>&matrix, std::vector<PlayAction>& actions);

void countLand_BFS_OPT(Vector2D<char>&matrix, int row, int col, std::vector<PlayAction>& actions);
int countLand_BFS_OPT_based(Vector2D<char>&matrix, std::vector<PlayAction>& actions);

void floodFill_DFS(Vector2D<int>& matrix, int row, int col, int oldColour, int newColour, std::vector<PlayAction>& actions);
void floodFill_DFS_Base(Vector2D<int>& matrix, int row, int col, int newColour, std::vector<PlayAction>& actions);

int floodFill_BFS(Vector2D<int>& matrix, int row, int col, int newColour, std::vector<PlayAction>& actions);


bool isBipartite_BFS(ListGraph& obj, string &actions);

bool isBipartite_DFS(int cur_index,int cur_val, ListGraph& obj, string& actions);
bool isBipartite_DFS_Base(ListGraph& obj, string& actions);

int wordLadder_Backtrack(string& start, string& target, vector<string>& variants, string &actions);
int wordLadder_BFS(string& start, string& target, vector<string>& variants, string& actions);

int SnakesNLadders_minDiceThrow_BFS(ListGraph& obj, string& actions);

int SnakesNLadders_minDiceThrow_DFS(ListGraph& obj, string& actions);

#endif // GRAPH_ALGORITHMS_H
