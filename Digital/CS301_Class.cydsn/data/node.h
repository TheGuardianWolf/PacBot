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
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    int16_t pos_x;
    int16_t pos_y;
    int8_t index;
} Node;

Node create_node(int16_t x, int16_t y);
