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
#include "debug.h"
#include "pid.h"
#include "analog.h"
//* ========================================

static char displaystring[BUF_SIZE] = "CS301 2016\n";
static uint8_t channel = 0;

void mux_test() {
    if(SW_Read() == 0) {
        led_set(LED_STATE_ON);
        while(SW_Read() == 0) {
        }
        channel++;
        if (channel > 4) {
            channel = 0;
        }
        mux_select(channel);
    }
    else {
        led_set(LED_STATE_OFF);
    }
}

void system_init() {
    CYGlobalIntEnable;
    USE_RF;
    motor_init();
    usb_init();
    quad_dec_init();
    led_set(LED_STATE_OFF);
//    pid_timer_init();
    mux_init();
}

int main()
{
    system_init();
    //motor_set(33, 33);
//    M1_QuadDec_SetCounter(QUADDEC_MAX - 10);
//    M2_QuadDec_SetCounter(QUADDEC_MAX - 10);
    for(;;)
    {
//        if(M2_QuadDec_GetCounter()>= 228) {
//            
//        }

    }   
}


/* [] END OF FILE */
