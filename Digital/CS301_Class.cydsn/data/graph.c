/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#include "graph.h"

Graph create_graph (size_t size) {
    Graph graph;
    graph.data = malloc(sizeof(int8_t*) * size);
    graph.nodes = malloc(sizeof(Node*) * size);
    graph.size = size;

    size_t i;
    for (i = 0; i < size; i++) {
        graph.data[i] = calloc(size, sizeof(int8_t));
    }
    
    return graph;
}

void add_node (Graph* g, Node* Node_to_add, uint8_t ind){
    g->nodes[ind] = Node_to_add;
    
//    int16_t ** new_matrix = realloc(g->data, (g->size + 1) * sizeof(uint16_t *));
//    node_to_add->index = g->size;
//    if(new_matrix != NULL) {
//        g->size ++;
//    }
//    
//    for(int i = 0; i < (g->size - 1); i++) {
//        new_matrix[i] = realloc(g->data[i], (g->size) * sizeof(uint16_t));
//        g->data[i] = new_matrix[i];
//        g->data[i][g->size-1] = -1;
//    }
//    
//    g->data = new_matrix;
//    
//    new_matrix[g->size] = malloc(g->size * sizeof(node));
//    
}

void change_arc (Graph* g, uint8_t ind1, uint8_t ind2, int16_t length){
    if (g->nodes[ind1]!= NULL && g->nodes[ind2] != NULL) {
    g->data[ind1][ind2] = length;
    }
}


//returns arc length between 2 nodes, if not connected or same node return -1
int8_t get_arc (Graph* g, uint8_t ind1, uint8_t ind2) {
    return g->data[ind1][ind2];
}

Node* get_node (Graph* g, uint8_t ind) {
    return g->nodes[ind];
}

void delete_graph (Graph* g) {
    int16_t s = g->size;
    for (int i = 0; i < s; ++i) {
        free(g->data[i]);
    }
    
    free(g->data);
}
