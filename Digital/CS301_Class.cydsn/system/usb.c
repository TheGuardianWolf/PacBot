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
//* ========================================
#include "usb.h"
//* ========================================

//* ========================================
static bool has_input = false;
static char line[USB_BUF_SIZE], entry[USB_BUF_SIZE];
static uint8 usbBuffer[USB_BUF_SIZE];
//* ========================================

void usb_init() {
    USBUART_Start(0,USBUART_5V_OPERATION);
}

bool usb_has_input() {
    return has_input;
}

char usb_get_char() {
    return USBUART_GetChar();
}


void usb_send_string(char *s)
{
// !! Assumes that *s is a string with allocated space >=64 chars     
//  Since USB implementation retricts data packets to 64 chars, this function truncates the
//  length to 62 char (63rd char is a '!')
    while (USBUART_CDCIsReady() == 0);
    s[63]='\0';
    s[62]='!';
    USBUART_PutData((uint8*)s,strlen(s));
}
//* ========================================
void usb_send_char(char c)
{ 
    while (USBUART_CDCIsReady() == 0);
    USBUART_PutChar(c);   
}

/* [] END OF FILE */
