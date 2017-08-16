#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <project.h>

#include "usb.h"
#include "wireless.h"
#include "motor.h"
#include "quad_dec.h"
#include "analog.h"
#include "debug.h"
#include "systime.h"


// void mux_test() {
//     if(SW_Read() == 0) {
//         led_set(LED_STATE_ON);
//         while(SW_Read() == 0) {
//         }
//         mux_next();
//     }
//     else {
//         led_set(LED_STATE_OFF);
//     }
// }

// void pk_drain_test() {
//     if(SW_Read() == 0) {
//         led_set(LED_STATE_ON);
//         while(SW_Read() == 0) {
//         }
//         PK_DRAIN_Write(~PK_DRAIN_Read());
//     }
//     else {
//         led_set(LED_STATE_OFF);
//     }
// }

// void light_array_test() {
//     if(switch_get()) {
//         led_set(LED_STATE_ON);
//         while(switch_get() == 0) {
//         }
//         adc_solo_collect();
//     }
//     else {
//         led_set(LED_STATE_OFF);
//     }
// }
static uint8_t pwm_mode = 0;
void pwm_test() {
    if(switch_get()) {
        while(switch_get() == 0) {
        }
        pwm_mode++;
        if (pwm_mode == 3) {
            pwm_mode = 0;
        }
        if (pwm_mode == 0) {
            motor_set(M_DRIFT, M_DRIFT);
        }
        else if (pwm_mode == 1) {
            motor_set(M_MAX / 2, M_MAX / 2);
        }
        else if (pwm_mode == 2) {
            motor_set(M_MAX, M_MAX);
        }
    }
}

void system_init() {
    systime_init();
    analog_init();
    USE_RF;
    motor_init();
    usb_init();
    quad_dec_init();
    CYGlobalIntEnable;
}

int main()
{
    system_init();
    for(;;) {
        pwm_test();
    }   
}


/* [] END OF FILE */
