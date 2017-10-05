#ifndef ARC_H
#define ARC_H

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
#include "node.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct {
    int16_t cost;
    uint8_t food;
    uint8_t dest_index;
} Arc;

Arc create_arc(int16_t length, uint8_t food_count, uint8_t init_dest);

void change_cost(Arc* a, int16_t newCost);

#endif /* ARC_H */




/* [] END OF FILE */
