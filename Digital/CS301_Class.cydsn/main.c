/* ========================================
 * Fully working code: 
 * PWM      : 
 * Encoder  : 
 * ADC      :
 * USB      : port displays speed and position.
 * CMD: "PW xx"
 * Copyright Univ of Auckland, 2016
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF Univ of Auckland.
 *
 * ========================================
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <project.h>
//* ========================================
#include "usb.h"
#include "wireless.h"
#include "motor.h"
#include "quad_dec.h"
#include "analog.h"
#include "systime.h"
//* ========================================

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

void system_init() {
    USE_RF;
    motor_init();
    usb_init();
    quad_dec_init();
    analog_init();
    systime_init();
    CYGlobalIntEnable;
}

int main()
{
    //motor_set(33, 33);
//    M1_QuadDec_SetCounter(QUADDEC_MAX - 10);
//    M2_QuadDec_SetCounter(QUADDEC_MAX - 10);
    // uint16_t average;
    system_init();
    for(;;) {
        // light_array_test();
        // average = adc_solo_get();
    }   
}


/* [] END OF FILE */
