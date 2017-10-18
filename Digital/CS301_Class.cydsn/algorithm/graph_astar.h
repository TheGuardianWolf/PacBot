#ifndef GRAPH_ASTAR_H
#define GRAPH_ASTAR_H

#include "point.h"
#include "graph.h"
#include "linked_list.h"

LinkedList* graph_astar(Graph* graph, graph_size_t start, graph_size_t target);

#endif /* GRAPH_ASTAR_H */
