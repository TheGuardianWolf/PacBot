#include "graph_astar.h"
#include <stdint.h>
#include "priority_queue.h"
#include "vector.h"
#include "voidtypes.h"
#include "linked_list.h"
#include <math.h>

#define NAN ((uvoid_t) 0 - 1);

static uvoid_t herustic(point_uint8_t node_grid_pos, point_uint8_t target_grid_pos, uint8_t heading, uint8_t last_heading) {
    uvoid_t dx = (uvoid_t) labs((int16_t) node_grid_pos.x - target_grid_pos.x);
    uvoid_t dy = (uvoid_t) labs((int16_t) node_grid_pos.y - target_grid_pos.y);

    uvoid_t D;
    if (heading != last_heading && last_heading < 5) {
        D = 2;
    }
    else {
        D = 1;
    }

    return D * (dx + dy);
}

LinkedList* graph_astar(const Graph* graph, graph_size_t start, graph_size_t target) {
    GraphNode* target_node = vector_get(graph->nodes, target);
    if (target_node != NULL || start == target) {
        PriorityQueue queue = priority_queue_create((size_t) ceilf(sqrtf(graph->nodes->size)));
        Vector* cost_so_far = vector_create(graph->nodes->size);
        Vector* came_from = vector_create(graph->nodes->size);

        priority_queue_push(queue, 0, (void*) start, false);

        graph_size_t i;
        for (i = 0; i < graph->nodes->size; i++) {
            vector_append(cost_so_far, NULL);
            vector_append(came_from, NULL);
        }
        vector_set(cost_so_far, start, (void*) NAN);
        vector_set(came_from, start, NAN);

        uvoid_t current_node_id;
        uvoid_t current_cost, new_cost;
        graph_size_t next;
        GraphEdge* current_edge;
        GraphNode* current_node;
        GraphEdge* came_from_edge;
        GraphArc* arc;
        uint8_t last_heading;
        size_t i;
        while (!priority_queue_empty(queue)) {
            current_node_id = (uvoid_t) priority_queue_remove(queue);
            current_node = vector_get(graph->nodes, current_node_id);

            came_from_edge = vector_get(came_from, current_node_id);
            if (came_from_edge != NULL) {
                arc = graph_arc_from(came_from_edge, current_node_id);
                last_heading = arc->heading;
            }
            else {
                last_heading = 5;
            }

            for (i = 0; i < current_node->edges->size; i++) {
                current_edge = vector_get(current_node->edges, i);
                arc = graph_arc_from(current_edge, current_node_id);

                new_cost = vector_get(cost_so_far, current_node_id);
                new_cost++;

                if (arc->destination == target) {
                    vector_set(cost_so_far, arc->destination, (void*)(uvoid_t) new_cost);
                    vector_set(came_from, arc->destination, current_edge);
                    break;
                }

                current_cost = vector_get(cost_so_far, arc->destination);
                if (current_cost == NULL || new_cost < current_cost) {
                    vector_set(cost_so_far, arc->destination, (void*)(uvoid_t) new_cost);
                    vector_set(came_from, arc->destination, current_edge);
                    current_node = vector_get(graph->nodes, arc->destination);
                    new_cost += heuristic(current_node->pos_grid, target_node->pos_grid, arc->heading, last_heading);
                    priority_queue_push(queue, new_cost, (void*)(uvoid_t) arc->destination, false);
                }
            }
        }

        priority_queue_destroy(queue);
        vector_destroy(cost_so_far);

        current_node_id = target;
        LinkedList* path = linked_list_create();
        linked_list_push(path, (void*)(uvoid_t) arc->target);
        while(current_node_id != start) {
            current_edge = vector_get(came_from, current_node_id);
            arc = graph_arc_from(current_edge, current_node_id);
            current_node_id = arc->destination;
            linked_list_push(path, (void*) current_node_id);
        } 

        vector_destroy(came_from);

        return path;
    }

    return NULL;
}
