#include <stdint.h>
#include "pathfinder.h"

LinkedList* pathfinder(
    Graph* graph,
    LinkedList* (*path_fn)(Graph*, graph_size_t, graph_size_t),
    point_uint8_t* start,
    point_uint8_t* target) {
        // Preprocessing
        graph_size_t start_node = graph_grid2nodeid(graph, *start);
        graph_size_t target_node = graph_grid2nodeid(graph, *target);
        // Processing
        return (*path_fn)(graph, start_node, target_node);
}
