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
//* ========================================

static char displaystring[BUF_SIZE] = "CS301 2016\n";
static uint8_t channel = 0;
void init() {
    CYGlobalIntEnable;
    USE_RF;
    motor_init();
    usb_init();
    quad_dec_init();
    SIGMUX_Start();
    SIGMUX_FastSelect(0);
//    pid_timer_init();
}

int main()
{
    init();
    //motor_set(33, 33);
//    M1_QuadDec_SetCounter(QUADDEC_MAX - 10);
//    M2_QuadDec_SetCounter(QUADDEC_MAX - 10);
    for(;;)
    {
//        if(M2_QuadDec_GetCounter()>= 228) {
//            
//        }
        if(SW_Read() == 1) {
            led_set(LED_STATE_ON);
            while(SW_Read() != 0) {
            }
            channel++;
            if (channel > 4) {
                channel = 0;
            }
            SIGMUX_Select(channel);
        }
        else {
            led_set(LED_STATE_OFF);
        }
    }   
}


/* [] END OF FILE */
