#include <stdint.h>
#include "graph.h"
#include "point.h"

void pathfinder(
    const vector_t* node_path
    const Graph* graph,
    const void* path_fn,
    const point_uint8_t* start,
    const point_uint8_t* target) {
        // Preprocessing
        uint16_t start_node = grid2nodeid(start);
        uint16_t target_node = grid2nodeid(target);
        // Processing
        (&path_fn)(node_path, graph, start, target)
}
