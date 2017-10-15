#ifndef GRAPH_H
#define GRAPH_H

#include <stdint.h>
#include "point.h"
#include "vector.h"

#define NODE_INVALID 0xFFFF
#define G_N 1
#define G_W 2
#define G_S 3
#define G_E 4

typedef graph_size_t uint8_t;

typedef struct {
    graph_size_t destination;
    uint8_t heading;
} GraphArc;

typedef struct {
    graph_size_t id;
    GraphArc a1; // Arc 1
    GraphArc a2; // Arc 2
} GraphEdge;

typedef struct {
    graph_size_t node_edge_index[2];
    GraphEdge* edge;
} GraphDetatchedEdge;

typedef struct {
    point_uint8_t pos_grid;
    Vector* edges;
} GraphNode;

//graph created with 1 node pointer and more nodes can be added, their indices will be sequentially be numbered
typedef struct {
    Vector* nodes;
    Vector* detatched_edges;
    graph_size_t unique_edges;
} Graph;

Graph* graph_create(const uint8_t** grid, uint8_t grid_height, uint8_t grid_width);

graph_size_t graph_grid2nodeid(const Graph* graph, point_uint8_t pos_grid);

point_uint8_t graph_nodeid2grid(const Graph* graph, graph_size_t node_id);

graph_size_t graph_node_order(const Graph* graph, graph_size_t node_id);

void graph_edge_remove(const Graph* graph, const GraphEdge* edge);

void graph_edge_attach(const Graph* graph, const GraphDetatchedEdge* detatched_edge);

#endif /* GRAPH_H */
