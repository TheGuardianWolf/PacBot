#include <stdlib.h>
#include "graph.h"

Graph* graph_create(const uint8_t** grid, uint8_t grid_height, uint8_t grid_width) {
    Graph* graph = malloc(sizeof(Graph));
    if (graph != NULL) {
        uint8_t i, j;
        graph_size_t total_nodes = 0;

        // Destructive operation on the grid to help create the graph
        for (i = 0; i < grid_height; i++) {
            for (j = 0; j < grid_width; j++) {
                if (grid[i][j] == 0) {
                    grid[i][j] = total_nodes;
                    total_nodes++;
                }
                else {
                    grid[i][j] = NODE_INVALID;
                }
            }
        }
    
        graph->nodes = vector_create(sizeof(GraphNode*), total_nodes));
        graph->detatched_edges = vector_create(sizeof(GraphDetatchedEdge*), 1);
        graph->unique_edges = 0;

        GraphNode* node;

        for (i = 0; i < grid_height; i++) {
            for (j = 0; j < grid_width; j++) {
                if (grid[i][j] != 0xFFFF) {
                    node = malloc(sizeof(GraphNode));
                    if (node != NULL) {
                        node->pos_grid.x = j;
                        node->pos_grid.y = i;
                        node->edges = vector_create(sizeof(GraphEdge*), 1);
                        vector_append(graph->nodes, node)
                    }
                }
            }
        }

        // 1 - N; 2 - W; 3 - S; 4 - E;
        uint8_t direction[2][2] = {{1, 0}, {0, 1}};
        point_uint8_t grid_scan;
        graph_size_t target_node_id;
        graph_size_t unique_edges = 0;
        GraphNode* node_ptr, target_node_ptr;
        GraphEdge* edge;

        graph_size_t i;
        for (i = 0; i < graph->nodes->size; i++) {
            for (j = 1; j >= 0; j--) {
                node_ptr = vector_get(graph->nodes, i);
                if (node_ptr != NULL) {
                    grid_scan.x = node_ptr->pos_grid.x + direction[j][0];
                    grid_scan.y = node_ptr->pos_grid.y + direction[j][1];
                    if (grid_scan.x < grid_width && grid_scan.x >= 0 && grid_scan.y < grid_height && grid_scan.y >= 0) {
                        target_node_id = grid[grid_scan.y][grid_scan.x];
                        target_node_ptr = vector_get(graph->nodes, target_node_index);
                        if (target_node_ptr != NULL && target_node_index != NODE_INVALID) {
                            edge = malloc(sizeof(GraphEdge));
                            if (edge != NULL) {
                                edge->a1[0] = i;
                                edge->a1[1] = j + 2;
                                edge->a2[0] = target_node_id;
                                edge->a2[1] = j;
                                vector_append(node_ptr->edges, edge);
                                vector_append(target_node_ptr->edges, edge);
                                unique_edges++;
                            }
                        }
                    }
                }
            }
        }

        graph->unique_edges = unique_edges;

        return graph;
    }

    return NULL;
}

// Node id is just the index... for now
graph_size_t graph_grid2nodeid(const Graph* graph, point_uint8_t pos_grid) {
    graph_size_t i;
    GraphNode* node;
    for (i = 0; i < graph->nodes->size; i++) {
        node = vector_get(graph->nodes, i);
        if (node != NULL) {
            if (node->pos_grid.x == x && node->pos_grid.y == y) {
                return i;
            }
        }
    }
    return NODE_INVALID
}

point_uint8_t graph_nodeid2grid(const Graph* graph, graph_size_t node_id) {
    GraphNode* node = vector_get(graph->nodes, node_id);
    if (node == NULL) {
        return NULL;
    }
    return node->pos_grid;
}

graph_size_t graph_node_order(const Graph* graph, graph_size_t node_id) {
    GraphNode* node = vector_get(graph->nodes, node_id);
    if (node == NULL) {
        return 0;
    }
    return node->edges->size;
}

void graph_edge_remove(const Graph* graph, const GraphEdge* edge) {
    GraphDetatchedEdge* detatched_edge = malloc(sizeof(GraphDetatchedEdge));
    graph_size_t node_ids[2] = {edge->a1[0], edge->a2[0]};

    uint8_t i;
    graph_size_t j;
    GraphNode* node;
    GraphEdge* test_edge;
    for (i = 0; i < 2; i++) {
        node = vector_get(graph->nodes, node_ids[i]);
        if (node != NULL) {
            for (j = 0; j < node->edges->size; j++) {
                test_edge = vector_get(node->edges, j)
                // Comparing memory addresses
                if (test_edge != NULL && test_edge == edge) {
                    detatched_edge->node_edge_index[i] = j;
                    vector_remove(node->edges, j);
                    break;
                }
            }
        }
    }

    vector_append(graph->detatched_edges, detatched_edge);
}

void graph_edge_attach(const Graph* graph, const GraphDetatchedEdge* detatched_edge) {
    uint8_t j;
    graph_size_t i;
    GraphNode* node;
    GraphEdge* edge;
    GraphDetatchedEdge* test_detatched_edge;

    graph_size_t node_ids[2] = {detatched_edge->edge->a1[0], detatched_edge->edge->a2[0]};
    for (j = 0; j < 2; j++) {
        node = vector_get(graph->nodes, node_ids[j]);
        if (node != NULL) {
            vector_insert(node->edges, detatched_edge->node_edge_index, detatched_edge->edge);
        }
    }

    for (i = 0; i < graph->detatched_edges->size; i++) {
        test_detatched_edge = vector_get(graph->detatched_edges, i);
        if (test_detatched_edge != NULL && test_detatched_edge == detatched_edge) {
            vector_remove(graph->detatched_edges, i);
            break;
        }
    }

    free(detatched_edge);
}
