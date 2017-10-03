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

graph create_graph (node* n) {
    int16_t ** matrix;
    matrix = malloc(sizeof(int16_t *));
    
    matrix[0] = malloc(sizeof(int16_t));
    
    n->index = 0;
    
    matrix[0][0] = -1;
    
    graph newGraph = {
        .data = matrix,
        .size = 1,
    };
    return newGraph;
}

void add_node (graph* g, node* node_to_add){
    int16_t ** new_matrix = realloc(g->data, (g->size + 1) * sizeof(uint16_t *));
    node_to_add->index = g->size;
    if(new_matrix != NULL) {
        g->size ++;
    }
    
    for(int i = 0; i < (g->size - 1); i++) {
        new_matrix[i] = realloc(g->data[i], (g->size) * sizeof(uint16_t));
        g->data[i] = new_matrix[i];
        g->data[i][g->size-1] = -1;
    }
    
    g->data = new_matrix;
    
    new_matrix[g->size] = malloc(g->size * sizeof(node));
    

}

void change_arc (graph* g, uint8_t ind1, uint8_t ind2, int16_t length){
    g->data[ind1][ind2] = length;
}


//returns arc length between 2 nodes, if not connected or same node return -1
int16_t check_length (graph* g, uint8_t ind1, uint8_t ind2) {
    return g->data[ind1][ind2];
}