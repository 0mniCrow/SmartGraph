#ifndef GRAPH_ALGORITHMS_H
#define GRAPH_ALGORITHMS_H

#include <string>
#include <queue>
#include <unordered_set>
#include "listgraph.h"
#include "vectorgraph.h"

std::string Breadth_first_search(ListGraph& obj, int root_index);

std::string Breadth_first_search(VectorGraph& obj, int root_index);

std::string Depth_first_search(ListGraph& obj, int root_index);

std::string Depth_first_search(VectorGraph& obj, int root_index);


#endif // GRAPH_ALGORITHMS_H
