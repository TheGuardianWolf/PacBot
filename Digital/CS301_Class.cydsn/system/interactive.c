#include "usb.h"
#include <project.h>

void led_set(uint8_t state) {
    REG_LED_Write(0b00000111 & state);
}

bool btn_get() {
    return (bool) REG_BTN_Read();
}

bool dipsw_get(uint8_t s) {
    return (bool) ((REG_DIP_Read() >> s) & 1);
}
