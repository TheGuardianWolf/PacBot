#include "graph_travel_all.h"
#include <stdint.h>
#include "priority_queue.h"
#include "vector.h"
#include "voidtypes.h"
#include "linked_list.h"
#include <math.h>

static Vector* make_acyclic(Graph* graph, graph_size_t start) {
    size_t i;
    Vector* node_orders = vector_create(graph->nodes->size);
    for (i = 0; i < node_orders->size; i++) {
        vector_append(node_orders, (void*)(uvoid_t) graph_node_order(graph, i));
    }
    LinkedList* frontier = linked_list_create();
    Vector* came_from = vector_create(graph->nodes->size);
    for (i = 0; i < graph->nodes->size; i++) {
        vector_append(came_from, NULL);
    }
    Vector* edge_priority = vector_create(graph->unique_edges);
    for (i = 0; i < graph->unique_edges; i++) {
        vector_append(edge_priority, (void*) UVOID_T_MAX);
    }
    void_t branch_length;

    linked_list_push(frontier, (void*)(uvoid_t) start);

    uvoid_t current_node_id, current_node_order;
    GraphEdge* current_edge;
    GraphNode* current_node;
    GraphEdge* came_from_edge;
    GraphArc* arc;
    while(frontier->size > 0) {
        current_node_id = (uvoid_t) linked_list_peek_stack(frontier);
        current_node = vector_get(graph->nodes, current_node_id);

        for (i = 0; i < current_node->edges->size; i++) {
            current_edge = vector_get(current_node->edges, i);
            arc = graph_arc_from(current_edge, current_node_id);

            came_from_edge = vector_get(came_from, arc->destination);
            if (came_from_edge == NULL) {
                vector_set(came_from, arc->destination, current_edge);
                linked_list_push(frontier, (void*)(uvoid_t) arc->destination);
                break;
            }
            else {
                if (current_node_id != start &&
                    vector_get(came_from, current_node_id) != current_edge &&
                    (arc->destination != start || came_from_edge != current_edge)) {
                        current_node_order = (uvoid_t) vector_get(node_orders, current_node_id);
                        while (current_node_order < 3) {
                            current_node_id = (uvoid_t) linked_list_pop(frontier);
                            current_edge = vector_set(came_from, current_node_id, NULL);
                        }
                        graph_edge_remove(graph, current_edge);
                }
            }

            if (i == current_node->edges->size - 1) {
                branch_length = frontier->size;
                current_node_id = (uvoid_t) linked_list_pop(frontier);
                came_from_edge = vector_get(came_from, current_node_id);
                if (came_from_edge != NULL) {
                    vector_set(edge_priority, came_from_edge->id, (void*)(uvoid_t) branch_length);
                }
            }
        }
    }

    linked_list_destroy(frontier);
    vector_destroy(came_from);
    vector_destroy(node_orders);

    return edge_priority;
}

static LinkedList* make_path(Graph* graph, graph_size_t start, Vector* edge_priority) {
    size_t i;
    LinkedList* path = linked_list_create();
    LinkedList* frontier = linked_list_create();
    Vector* came_from = vector_create(graph->nodes->size);
    for (i = 0; i < graph->nodes->size; i++) {
        vector_append(came_from, (void*) UVOID_T_MAX);
    }
    PriorityQueue* priority = priority_queue_create(4);
    graph_size_t nodes_visited = 1;

    linked_list_push(frontier, (void*)(uvoid_t) start);

    uvoid_t current_node_id, current_priority;
    GraphEdge* current_edge;
    GraphNode* current_node;
    uvoid_t came_from_node_id;
    GraphArc* arc;

    vector_set(came_from, start, (void*)(uvoid_t) start);

    while(frontier->size > 0) {
        current_node_id = (uvoid_t) linked_list_peek_stack(frontier);
        
        linked_list_add(path, (void*)(uvoid_t) current_node_id);
        if (nodes_visited == graph->nodes->size) {
            break;
        }

        current_node = vector_get(graph->nodes, current_node_id);

        for (i = 0; i < current_node->edges->size; i++) {
            current_edge = vector_get(current_node->edges, i);
            current_priority = (uvoid_t) vector_get(edge_priority, current_edge->id);
            priority_queue_add(priority, current_priority, current_edge, false);
        }

        while (!priority_queue_empty(priority)) {
            current_edge = priority_queue_remove(priority);
            arc = graph_arc_from(current_edge, current_node_id);

            came_from_node_id = (uvoid_t) vector_get(came_from, arc->destination);
            if (came_from_node_id == UVOID_T_MAX) {
                vector_set(came_from, arc->destination, (void*)(uvoid_t) current_node_id);
                linked_list_push(frontier, (void*)(uvoid_t) arc->destination);
                nodes_visited++;
                break;
            }
            if (priority_queue_empty(priority)) {
                linked_list_pop(frontier);
            }
        }

        while (!priority_queue_empty(priority)) {
            priority_queue_remove(priority);
        }
    }

    vector_destroy(came_from);
    linked_list_destroy(frontier);
    priority_queue_destroy(priority);

    return path;
}

LinkedList* graph_travel_all(Graph* graph, graph_size_t start, graph_size_t target) {
    Vector* edge_priority = make_acyclic(graph, start);
    LinkedList* path = make_path(graph, start, edge_priority);

    GraphDetatchedEdge* detatched_edge;
    while (graph->detatched_edges->size > 0) {
        detatched_edge = vector_get(graph->detatched_edges, graph->detatched_edges->size - 1);
        graph_edge_attach(graph, detatched_edge);
    }

    return path;
}
