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

#include <project.h>
#include "motor.h"

static uint8_t speed2pwm(int8_t speed) {
    if (speed < 0) {
        return ((uint8_t) speed - 128 - 35);  // Dead zone compensation of -35
    }
    return (uint8_t) speed + 128 + 35;  // Dead zone compensation of 35
}

void motor_init() {
    M1_PWM_Start();
    M2_PWM_Start();
    motor_set(M_DRIFT, M_DRIFT);
}

void motor_set(int8_t speed_L, int8_t speed_R) {
    ML_SET(speed2pwm(speed_L));
    MR_SET(speed2pwm(speed_R));
    
    motor_disable(speed_L == M_DRIFT, speed_R == M_DRIFT);
}

void motor_disable(bool disable_L, bool disable_R) {
    ML_DISABLE((uint8_t) disable_L);
    MR_DISABLE((uint8_t) disable_R);
}

/* [] END OF FILE */
