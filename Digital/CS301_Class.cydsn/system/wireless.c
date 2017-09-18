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
#include "interactive.h"
#include "usb.h"
//* ========================================

// The struct for data storage is -->available already<-- in wireless.h
// use that! (Or just tell me what you're up to here and I can optimise)

static volatile RFBuffer receive_buffer, data_buffer;
static volatile uint8_t start_count = 0;
static volatile uint8_t data_count = 0;
volatile uint8_t data;

CY_ISR(rf_rx) {
    while(UART_GetRxBufferSize()) {
        data = UART_ReadRxData();
        if (start_count < 2) {
            if(data == SOP) {
                start_count++;
            }
            else {
                start_count = 0;
            }
        }
        else {
            receive_buffer.bytes[data_count] = data;
            if (data_count >= 31) {
                data_buffer = receive_buffer;
                start_count = 0;
                data_count = 0;
            }
            else {
                data_count++;
            }
        }
    }
}

void wireless_init() {
    isrRF_RX_StartEx(rf_rx);
    UART_Start();
}

RFData wireless_get() {
    return data_buffer.data;
}
