#include <stdint.h>
#include "linked_list.h"
#include "graph.h"
#include "point.h"

LinkedList* pathfinder(
    const Graph* graph,
    const void* path_fn,
    const point_uint8_t* start,
    const point_uint8_t* target) {
        // Preprocessing
        graph_size_t start_node = grid2nodeid(start);
        graph_size_t target_node = grid2nodeid(target);
        // Processing
        return (&path_fn)(graph, start, target)
}
