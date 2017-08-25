#include "usb.h"
#include <project.h>

void led_set(uint8_t state) {

}

bool switch_get() {
    return !((bool) SW_Read());
}
