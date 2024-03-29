#ifndef USB_H
#define USB_H
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

#include <stdbool.h>

// USBUART
#define USB_BUF_SIZE 64 // USBUART fixed buffer size
#define CHAR_NULL '0'
#define CHAR_BACKSP 0x08
#define CHAR_DEL 0x7F
#define CHAR_ENTER 0x0D
#define LOW_DIGIT '0'
#define HIGH_DIGIT '9'

void usb_init();
void usb_enumerate();
char usb_get_char();
void usb_send_string(char *s);
void usb_send_char(char c);
//* ========================================

/* [] END OF FILE */

#endif /* USB_H */
