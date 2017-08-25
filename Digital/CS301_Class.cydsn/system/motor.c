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

static MotorData motor_last_data = NULL;

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

MotorData motor_get() {
    return motor_last_data;
}

void motor_set(MotorData data) {
    M1_PWM_WriteCompare((speed2pwm(data.L));
    M2_PWM_WriteCompare(speed2pwm(data.R));
    
    motor_disable(data.L == M_DRIFT, data.R == M_DRIFT);

    motor_last_data = data;
}

void motor_disable(bool disable_L, bool disable_R) {
    M1_D1_Write((uint8_t) disable_L);
    M2_D1_Write((uint8_t) disable_R);
}

/* [] END OF FILE */
