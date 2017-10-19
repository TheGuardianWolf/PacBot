#include <stdlib.h>
#include "graph.h"

Graph* graph_create(uint8_t* grid, uint8_t grid_height, uint8_t grid_width) {
    Graph* graph = malloc(sizeof(Graph));
    if (graph != NULL) {
        uint8_t i, j;
        graph_size_t total_nodes = 0;

        // Destructive operation on the grid to help create the graph
        for (i = 0; i < grid_height; i++) {
            for (j = 0; j < grid_width; j++) {
                if (grid[i * grid_width + j] == 0) {
                    grid[i * grid_width + j] = total_nodes;
                    total_nodes++;
                }
                else {
                    grid[i * grid_width + j] = NODE_INVALID;
                }
            }
        }
    
        graph->nodes = vector_create(total_nodes);
        graph->detatched_edges = vector_create(1);
        graph->unique_edges = 0;

        GraphNode* node;

        for (i = 0; i < grid_height; i++) {
            for (j = 0; j < grid_width; j++) {
                if (grid[i * grid_width + j] != NODE_INVALID) {
                    node = malloc(sizeof(GraphNode));
                    if (node != NULL) {
                        node->pos_grid.x = j;
                        node->pos_grid.y = i;
                        node->edges = vector_create(1);
                        vector_append(graph->nodes, node);
                    }
                }
            }
        }

        // 1 - N; 2 - W; 3 - S; 4 - E;
        uint8_t direction[2][2] = {{1, 0}, {0, 1}};
        point_uint8_t grid_scan;
        graph_size_t target_node_id;
        GraphNode* node_ptr;
        GraphNode* target_node_ptr;
        GraphEdge* edge;

        graph_size_t k;
        for (k = 0; k < graph->nodes->size; k++) {
            for (j = 1; j <= 1; j--) {
                node_ptr = vector_get(graph->nodes, k);
                if (node_ptr != NULL) {
                    grid_scan.x = node_ptr->pos_grid.x + direction[j][0];
                    grid_scan.y = node_ptr->pos_grid.y + direction[j][1];
                    if (grid_scan.x < grid_width && grid_scan.x >= 0 && grid_scan.y < grid_height && grid_scan.y >= 0) {
                        target_node_id = grid[grid_scan.y * grid_width + grid_scan.x];
                        target_node_ptr = vector_get(graph->nodes, target_node_id);
                        if (target_node_ptr != NULL && target_node_id != NODE_INVALID) {
                            edge = malloc(sizeof(GraphEdge));
                            if (edge != NULL) {
                                edge->id = graph->unique_edges;
                                edge->a1.destination = k;
                                edge->a1.heading = j + 2;
                                edge->a2.destination = target_node_id;
                                edge->a2.heading = j;
                                vector_append(node_ptr->edges, edge);
                                vector_append(target_node_ptr->edges, edge);
                                graph->unique_edges++;
                            }
                        }
                    }
                }
            }
        }
        return graph;
    }

    return NULL;
}

// Node id is just the index... for now
graph_size_t graph_grid2nodeid(Graph* graph, point_uint8_t pos_grid) {
    graph_size_t i;
    GraphNode* node;
    for (i = 0; i < graph->nodes->size; i++) {
        node = vector_get(graph->nodes, i);
        if (node != NULL) {
            if (node->pos_grid.x == pos_grid.x && node->pos_grid.y == pos_grid.y) {
                return i;
            }
        }
    }
    return NODE_INVALID;
}

point_uint8_t graph_nodeid2grid(Graph* graph, graph_size_t node_id) {
    GraphNode* node = vector_get(graph->nodes, node_id);
    if (node == NULL) {
        point_uint8_t null = {
            .x = NODE_INVALID,
            .y = NODE_INVALID
        };
        return null;
    }
    return node->pos_grid;
}

graph_size_t graph_node_order(Graph* graph, graph_size_t node_id) {
    GraphNode* node = vector_get(graph->nodes, node_id);
    if (node == NULL) {
        return 0;
    }
    return node->edges->size;
}

void graph_edge_remove(Graph* graph, GraphEdge* edge) {
    GraphDetatchedEdge* detatched_edge = malloc(sizeof(GraphDetatchedEdge));
    graph_size_t node_ids[2] = {edge->a1.destination, edge->a2.destination};

    uint8_t i;
    graph_size_t j;
    GraphNode* node;
    GraphEdge* test_edge;
    for (i = 0; i < 2; i++) {
        node = vector_get(graph->nodes, node_ids[i]);
        if (node != NULL) {
            for (j = 0; j < node->edges->size; j++) {
                test_edge = vector_get(node->edges, j);
                // Comparing memory addresses
                if (test_edge == edge) {
                    detatched_edge->node_edge_index[i] = j;
                    vector_remove(node->edges, j);
                    break;
                }
            }
        }
    }

    detatched_edge->edge = edge;
    vector_append(graph->detatched_edges, detatched_edge);
}

void graph_edge_attach(Graph* graph, GraphDetatchedEdge* detatched_edge) {
    uint8_t j;
    graph_size_t i;
    GraphNode* node;
    GraphDetatchedEdge* test_detatched_edge;

    graph_size_t node_ids[2] = {detatched_edge->edge->a1.destination, detatched_edge->edge->a2.destination};
    for (j = 0; j < 2; j++) {
        node = vector_get(graph->nodes, node_ids[j]);
        if (node != NULL) {
            vector_insert(node->edges, detatched_edge->node_edge_index[j], detatched_edge->edge);
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

GraphArc* graph_arc_to(GraphEdge* edge, graph_size_t node_id) {
    if (edge->a1.destination == node_id) {
        return &(edge->a1);
    }
    else {
        return &(edge->a2);
    }
}

GraphArc* graph_arc_from(GraphEdge* edge, graph_size_t node_id) {
    if (edge->a1.destination == node_id) {
        return &(edge->a2);
    }
    else {
        return &(edge->a1);
    }
}

void graph_destroy(Graph* graph) {
    graph_size_t i;
    GraphNode* node;
    GraphEdge* edge;
    GraphDetatchedEdge* detatched_edge;

    for (i = 0; i < graph->nodes->size; i++) {
        node = vector_get(graph->nodes, i);
        while (node->edges->size > 0) {
            edge = vector_get(node->edges, node->edges->size - 1);
            graph_edge_remove(graph, edge);
        }
        
    }

    for (i = 0; i < graph->nodes->size; i++) {
        node = vector_get(graph->nodes, i);
        vector_destroy(node->edges);
        free(node);
    }
    vector_destroy(graph->nodes);

    for (i = 0; i < graph->detatched_edges->size; i++) {
        detatched_edge = vector_get(graph->detatched_edges, i);
        free(detatched_edge->edge);
        free(detatched_edge);
    }
    vector_destroy(graph->detatched_edges);
    free(graph);
}