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
static volatile uint8_t flag_packet = 0;
volatile char the_char;

//uint8_t dataready_flag =0;

//uint8_t flag_rf_transmission_active = RF_UNKNOWN; // [TRUE if receieving data, FALSE is not, UNKNOWN at startup]

static uint8_t uart_count = 0;
CY_ISR(rf_rx) {
    the_char = UART_GetChar();
    usb_send_char(the_char);
//    if (uart_count > 1) {
//        if (flag_packet == 0) {
//            Rec = UART_GetByte();
//            if((Rec >> 8) == 0) {// check if byte is valid
//                data1 = Rec; //first byte
//                if(data1 == 0xa) { //if first byte is 0xa
//                    Rec = UART_GetByte();
//                    if((Rec >> 8) == 0) {
//                        data2 = Rec;
//                        if(data2 == 0xa) { //if the 2 bytes are 0xaa
//                            data = (data2 << 8) | data1;//store it in uint16
//                            flag_packet = 1;
//                        }
//                    }
//                }
//            }
//
//        }
//        else {
//            Rec = UART_GetByte();
//            if((Rec >> 8) == 0) {// check if byte is valid
//                data1 = Rec; //first byte
//                Rec = UART_GetByte();
//                if((Rec >> 8) == 0) {
//                    data2 = Rec;
//                    data = (data2 << 8) | data1;//store it in int16
//                    if(ind >= 16) { //checks end of packet bytes
//                        flag_packet = 0;
//                        ind = 0;
//                    }
//                    else {
//                        dataPacket.a[ind] = data;
//                        ind ++;
//                    }
//                }
//            }
//        }
//        UART_ClearRxBuffer();
//    }
//    else {
//        uart_count++;
//    }
}

void wireless_init() {
    isrRF_RX_StartEx(rf_rx);
    UART_Start();
}

RFData wireless_get() {
    return dataPacket.rf_data;
}
