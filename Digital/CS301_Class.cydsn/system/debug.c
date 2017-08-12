#include "usb.h"
#include <project.h>

void led_set(uint8_t state) {
    switch(state) {
        case 0:
        LED_Write(0);
        break;
        case 1:
        LED_Write(1);
        break;
        case 2:
        LED_Write(~LED_Read());
        break;
        default:
        LED_Write(0);
        break;
    }
}

void log_usb(char* message) {
    usb_send_string(message);
}

bool switch_get() {
    return !((bool) SW_Read());
}
