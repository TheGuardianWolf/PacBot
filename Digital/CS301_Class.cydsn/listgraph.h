#ifndef LISTGRAPH_H
#define LISTGRAPH_H
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
#include "arc.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
    
#define N 1;
#define E 2;
#define S 3;
#define W 4;

//graph created with 1 node pointer and more nodes can be added, their indices will be sequentially be numbered
typedef struct {
    Node** nodes;
    Arc** data;
    size_t size;
} List;

List create_list (size_t size);

void add_arc (List* l, uint8_t node_ind, int8_t direction, Arc* a);

void add_node (List* l, Node* Node_to_add, uint8_t ind);

Node* get_node (List* l, uint8_t ind);

//returns arc length between 2 nodes, if not connected return -1
int8_t get_arc (List* l, uint8_t ind1, uint8_t ind2);

void delete_graph (List* l);

#endif /* GRAPH_H */
