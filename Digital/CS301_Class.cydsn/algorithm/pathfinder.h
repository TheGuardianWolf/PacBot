#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "graph.h"
#include "point.h"
#include "linked_list.h"

LinkedList* pathfinder(
    Graph* graph,
    LinkedList* (*path_fn)(Graph*, graph_size_t, graph_size_t),
    point_uint8_t start,
    point_uint8_t target);

#endif /* PATHFINDER_H */
