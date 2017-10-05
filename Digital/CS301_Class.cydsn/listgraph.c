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

#include "listgraph.h"

List create_list (size_t size) {
    List list;
    list.data = malloc(sizeof(int8_t*) * size);
    list.nodes = malloc(sizeof(Node*) * size);
    list.size = size;

    size_t i;
    for (i = 0; i < size; i++) {
        list.data[i] = calloc(4, sizeof(Arc));
    }
    
    return list;
}

void add_arc (List* l, uint8_t node_ind, int8_t direction, Arc* a) {
    l->data[node_ind][direction] = *a;
}

void add_node (List* l, Node* Node_to_add, uint8_t ind){
    l->nodes[ind] = Node_to_add;
}

//returns arc length between 2 nodes, if not connected or same node return -1
int8_t get_arc (List* l, uint8_t ind1, uint8_t ind2) {
    int8_t i;
    for ( i = 0; i < 4; i++) {
        if ( &(l->data[ind1][i]) != NULL ) {
            if( l->data[ind1][i].dest_index == ind2 ) { 
                return l->data[ind1][i].cost;
            }
        }
    }
    return -1;
}

Node* get_node (List* l, uint8_t ind) {
    return l->nodes[ind];
}

void delete_list (List* l) {
    int16_t s = l->size;
    for (int i = 0; i < s; ++i) {
        free(l->data[i]);
    }
    
    free(l->data);
}
