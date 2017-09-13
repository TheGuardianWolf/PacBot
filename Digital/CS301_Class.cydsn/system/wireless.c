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

static volatile int16 Rec = 0;
static volatile int8 data1 = 0;
static volatile int8 data2 = 0;
static volatile int16 data = 0;
static volatile int16 ind = 0;
static volatile datatype dataPacket;
//uint8_t flag_rx = 0;
static volatile uint8_t start_count = 0;
static volatile uint8_t data_count = 0;
volatile uint8_t the_char;

//uint8_t dataready_flag =0;

//uint8_t flag_rf_transmission_active = RF_UNKNOWN; // [TRUE if receieving data, FALSE is not, UNKNOWN at startup]

static uint8_t uart_count = 0;
CY_ISR(rf_rx) {
    the_char = UART_ReadRxData();
    if (start_count < 2) {
        if(the_char == SOP) {
            start_count++;
        }
        else {
            start_count = 0;
        }
    }
    else {
        dataPacket.bytes[data_count] = the_char;
        if (data_count >= 31) {
            start_count = 0;
            data_count = 0;
        }
        else {
            data_count++;
        }
    }
}

void wireless_init() {
    isrRF_RX_StartEx(rf_rx);
    UART_Start();
}

RFData wireless_get() {
    return dataPacket.rf_data;
}
