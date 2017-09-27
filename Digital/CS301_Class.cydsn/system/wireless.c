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
#include "systime.h"
#include "wireless.h"
#include "interactive.h"
#include "usb.h"
#include "string.h"
//* ========================================

// The struct for data storage is -->available already<-- in wireless.h
// use that! (Or just tell me what you're up to here and I can optimise)

static volatile RFBuffer receive_buffer, data_buffer;
static volatile uint8_t start_count = 0;
static volatile uint8_t data_count = 0;
static volatile bool rf_new = false;

CY_ISR(rf_rx) {
    while(UART_GetRxBufferSize()) {
        uint8_t data = UART_ReadRxData();
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
            if (data_count >= 24) {
                receive_buffer.data.timestamp = systime_ms();
                data_buffer = receive_buffer;
                start_count = 0;
                data_count = 0;
                rf_new = true;
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
    rf_new = false;
    return data_buffer.data;
}

bool wireless_check() {
    return rf_new;
}
