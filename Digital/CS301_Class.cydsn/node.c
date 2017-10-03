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

node create_node(int16_t x, int16_t y, int8_t i) {
    node newNode = {
        .pos_x = x,
        .pos_y = y,
        .index = i
    };
    return newNode;
}