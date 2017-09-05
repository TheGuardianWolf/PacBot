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

#include <project.h>

#include "usb.h"

static bool usb_cdc_enabled = true;
static bool usb_enabled = true;

void usb_init() {
    USBUART_Start(0,USBUART_5V_OPERATION);
    //usb_enumerate();
}

void usb_enumerate() {
    if (true) {//USBUART_VBusPresent() == 1) {
        if (!usb_enabled) {
            USBUART_Start(0,USBUART_5V_OPERATION);
            usb_enabled = true;
        }
        if (!usb_cdc_enabled) {
            USBUART_PutData((uint8_t*)"", 0);
            if (USBUART_CDCIsReady()) {
                usb_cdc_enabled = true;
            }
            else {
                usb_cdc_enabled = false;
            }
        }
    }
    else {
        USBUART_Stop();
        usb_enabled = false;
    }
}

char usb_get_char() {
    if (usb_cdc_enabled) {
        return USBUART_GetChar();
    }
    return '\0';
}

void usb_send_string(char *s) {
    if (usb_cdc_enabled) {
    // !! Assumes that *s is a string with allocated space >=64 chars     
    //  Since USB implementation retricts data packets to 64 chars, this function truncates the
    //  length to 62 char (63rd char is a '!')
        s[63]='\0';
        s[62]='!';
        while (!USBUART_CDCIsReady());
        USBUART_PutData((uint8*)s,strlen(s));
    }
}
//* ========================================
void usb_send_char(char c) { 
    if (usb_cdc_enabled) {
        //while (USBUART_CDCIsReady() == 0);
        USBUART_PutChar(c);
    }
}

/* [] END OF FILE */
