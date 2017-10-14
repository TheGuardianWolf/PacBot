#ifndef GRAPH_TRAVEL_ALL_H
#define GRAPH_TRAVEL_ALL_H

#include "point.h"
#include "graph.h"
#include "linked_list.h"

LinkedList* graph_travel_all(const Graph* graph, graph_size_t start, graph_size_t target);

#endif /* GRAPH_TRAVEL_ALL_H */
