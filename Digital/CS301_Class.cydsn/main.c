#include <stdbool.h>

#include "usb.h"
#include "wireless.h"
#include "motor.h"
#include "quad_dec.h"
#include "analog.h"
#include "debug.h"
#include "systime.h"

void system_init() {
    systime_init();
    signals_init();
    motor_init();
    usb_init();
    quad_dec_init();
    USE_RF;
    adc_init();
    CYGlobalIntEnable;
    adc_wait_ready();
}

int main() {
    system_init();
    while(true) {

    }   
}


/* [] END OF FILE */
