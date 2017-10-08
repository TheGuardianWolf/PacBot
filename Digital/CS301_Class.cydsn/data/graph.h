#ifndef GRAPH_H
#define GRAPH_H

#include "point.h"
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    pointu16_t pos_real;
    pointu16_t pos_grid;
} GraphNode;

typedef struct {
    int8_t cost;
    uint8_t food;
    uint16_t destination;
} GraphArc;

typedef struct {
    union {
        GraphArc n, s, w, e;
        GraphArc arcs[4];
    }
} GraphCons;

//graph created with 1 node pointer and more nodes can be added, their indices will be sequentially be numbered
typedef struct {
    Node** nodes;
    int8_t** data;
    size_t size;
} Graph;

Graph create_graph (size_t size);

void change_arc (Graph* g, uint8_t ind1, uint8_t ind2, int16_t length);

void add_node (Graph* g, Node* Node_to_add, uint8_t ind);

Node* get_node (Graph* g, uint8_t ind);

//returns arc length between 2 nodes, if not connected return -1
int8_t get_arc (Graph* g, uint8_t ind1, uint8_t ind2);

void delete_graph (Graph* g);

#endif /* GRAPH_H */
