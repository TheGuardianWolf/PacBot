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

static uint8_t speed2period(int8_t speed) {
    if (speed < 0) {
        return (uint8_t) (128 + speed);
    }
    return (uint8_t) speed * 2;
}

void motor_init() {
    M1_PWM_Enable();
    M2_PWM_Enable();
}

void motor_set(int8_t speed_L, int8_t speed_R) {
    if (speed_L == 0) {
        ML_DISABLE;
    }
    ML_SET(speed2period(speed_L));

    if (speed_R == 0) {
        MR_DISABLE;
    }
    MR_SET(speed2period(speed_R));
}

/* [] END OF FILE */
