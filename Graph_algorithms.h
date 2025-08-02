#ifndef GRAPH_ALGORITHMS_H
#define GRAPH_ALGORITHMS_H

#include <string>
#include <queue>
#include <stack>
#include <unordered_set>
#include <unordered_map>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <QColor>
#include <climits>
#include "listgraph.h"
#include "matrixmodel.h"
#include "vectorgraph.h"
#include "nodegraph.h"

#include "Graph_algorithms_supply.h"


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

int waterJigProblem_BFS(int right_jig, int left_jig, int desirable_value, string& actions, vector<PlayAction>& act_st);

int twoWayWaterFlow(Vector2D<Atl_Pac_Node>& matrix, vector<PlayAction>& actions);

int shortPathLength_DFS(Vector2D<LandNode>& matrix, int start_row, int start_col, int finish_row, int finish_col, vector<PlayAction>& actions);
int shortPathLength_BFS(Vector2D<LandNode>& matrix, int start_row, int start_col, int finish_row, int finish_col, vector<PlayAction>& actions);

using Snode = ListNode<cur_type>;

Snode * cloneGraph_BFS(Snode * original, string&actions);
void DFS_cloning(Snode * cur_node, std::unordered_map<Snode*,Snode*>& copied, string& actions);
Snode * cloneGraph_DFS(Snode * original, string& actions);

bool compareSnodeGraphs(Snode * first_node, Snode* sec_node, std::unordered_map<Snode*,Snode*>& visited);

bool hasCycles_DFS(ListGraph& graph, string& actions);
bool hasCycle_BFS_Kahn_alg(ListGraph& graph, string& actions);

bool hasCycle_Undirected_BFS(ListGraph& graph, string& actions);
bool hasCycle_Undirected_DFS(ListGraph& graph, string& actions);

bool hasCycle_Colour(ListGraph& graph, string& actions);

bool hasNegCycle(ListGraph& graph, int start_id, string& actions);

#endif // GRAPH_ALGORITHMS_H
