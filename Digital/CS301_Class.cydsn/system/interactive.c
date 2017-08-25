#include "usb.h"
#include <project.h>

void led_set(uint8_t state) {
    REG_LED_Write(0b00000111 & state);
}

bool btn_get() {
    return (bool) REG_BTN_Read();
}

uint8_t dipsw_get() {
    return REG_DIP_Read();
}
