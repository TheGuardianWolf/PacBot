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
//* ========================================

// The struct for data storage is -->available already<-- in wireless.h
// use that! (Or just tell me what you're up to here and I can optimise)

int16 Rec = 0;
int8 data1 = 0;
int8 data2 = 0;
int16 data = 0;
int16 ind = 0;
datatype dataPacket;

CY_ISR(rf_rx) {
    // Just load byte(s) here and clear buffer, do processing outside ISR
    Rec = UART_GetByte();
    if ((Rec>>8)==0) { // checks MSB if error occured
        data = Rec; //truncates 16 bit to 8 bits keeping LSB
    }
}
    
void wireless_init() {
    UART_Start();
    isrRF_RX_StartEx(rf_rx);
}

// SUrely the nested ifs are not nessecary...
void readByte() {
    int i;
    for(i = 0; i < UART_GetRxBufferSize(); i++) {
        if (flag_packet == 0) {
        Rec = UART_GetByte();
            if((Rec >> 8) == 0) {// check if byte is valid
                data1 = Rec; //first byte
                if(data1 == 0xa) { //if first byte is 0xa 
                    Rec = UART_GetByte();
                        if((Rec >> 8) == 0){
                            data2 = Rec;
                                if(data2 == 0xa){//if the 2 bytes are 0xaa
                                    data = (data2 << 8) | data1;//store it in uint16
                                    flag_packet = 1;
                                }
                        }    
                }
            }
       
    }
        else {
            Rec = UART_GetByte();
            if((Rec >> 8) == 0) {// check if byte is valid
                data1 = Rec; //first byte
                Rec = UART_GetByte();
                    if((Rec >> 8) == 0){
                        data2 = Rec;
                        data = (data2 << 8) | data1;//store it in int16
                        if(data == 1){//checks end of packet bytes
                            UART_ClearRxBuffer();
                            flag_packet = 0;
                      }
                      else{
                      dataPacket.a[ind] = data;
                      ind ++;
                      }
                    }    
            }
        }
    }
}
