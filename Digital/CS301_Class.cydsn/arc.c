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
#include "arc.h"

Arc create_arc(int16_t length, uint8_t food_count, uint8_t init_dest){
    Arc newArc = {
        .cost = length,
        .food = food_count,
        .dest_index = init_dest,
    };
    return newArc;
}

void change_cost(Arc* a, int16_t newCost) {
    a->cost = newCost;
}

/* [] END OF FILE */