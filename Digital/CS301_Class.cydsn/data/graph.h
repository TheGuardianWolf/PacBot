#ifndef GRAPH_H
#define GRAPH_H
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

/* [] END OF FILE */
#include "node.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

//graph created with 1 node pointer and more nodes can be added, their indices will be sequentially be numbered
typedef struct {
    
    Node** nodes;
    int8_t** data;
    size_t size;
} Graph;

Graph create_graph (size_t size);

void change_arc (Graph* g, uint8_t ind1, uint8_t ind2, int16_t length);

void add_node (Graph* g, Node* node_to_add);

Node* get_node (Graph* g, uint8_t ind)

//returns arc length between 2 nodes, if not connected return -1
int8_t get_arc (Graph* g, uint8_t ind1, uint8_t ind2);

void delete_graph (Graph* g);

#endif /* GRAPH_H */
