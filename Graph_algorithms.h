#ifndef GRAPH_ALGORITHMS_H
#define GRAPH_ALGORITHMS_H

#include <string>
#include <queue>
#include <unordered_set>
#include <chrono>
#include "listgraph.h"
#include "vectorgraph.h"

std::string Breadth_first_search(ListGraph& obj, int root_index);

std::string Breadth_first_search(VectorGraph& obj, int root_index);

std::string Depth_first_search(ListGraph& obj, int root_index);

std::string Depth_first_search(VectorGraph& obj, int root_index);

bool szBordCheck(int cur_row, int cur_col, int rows_size, int cols_size);

int orangesRotting_iteration(std::vector<std::vector<int>>& matrix, long long &time);
int orangesRotting_DFS(std::vector<std::vector<int>>& matrix, long long &time, std::string &actions);
int orangesRotting_BFS(std::vector<std::vector<int>>& matrix, long long& time, std::string& actions);

#endif // GRAPH_ALGORITHMS_H
