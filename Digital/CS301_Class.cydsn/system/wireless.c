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
#include <project.h>
//* ========================================
#include "wireless.h"
//* ========================================

uint16 Rec = 0;
uint8 data = 0;

CY_ISR(quad_dec_L) {
    Rec = UART_GetByte();
    if ((Rec>>8)==0) { // checks MSB if error occured
        data = Rec; //truncates 16 bit to 8 bits keeping LSB
        
    }
}

void uart_init() {
    UART_Start();
    vtype1 dataPacket;
}