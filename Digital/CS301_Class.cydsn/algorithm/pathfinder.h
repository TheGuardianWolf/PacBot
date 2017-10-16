#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "graph.h"
#include "point.h"

LinkedList* pathfinder(
    const Graph* graph,
    const void* path_fn,
    const point_uint8_t* start,
    const point_uint8_t* target);

#endif /* PATHFINDER_H */
