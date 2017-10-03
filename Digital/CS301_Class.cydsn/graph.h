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
    int16_t** data;
    int16_t size;
} graph;

graph create_graph (node* n);

void change_arc (graph* g, uint8_t ind1, uint8_t ind2, int16_t length);

void add_node (graph* g, node* node_to_add);

//returns arc length between 2 nodes, if not connected return -1
int16_t check_length (graph* g, uint8_t ind1, uint8_t ind2);