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
        return ((uint8_t) speed - 128);
    }
    return (uint8_t) speed + 128;
}

void motor_init() {
    M1_PWM_Enable();
    M2_PWM_Enable();
}

void motor_set(int8_t speed_L, int8_t speed_R) {
    ML_SET(speed2period(speed_L));
    MR_SET(speed2period(speed_R));
}

void motor_disable(bool disable_L, bool disable_R) {
    if (disable_L) {
        ML_DISABLE();
    }

    if (disable_R) {
        MR_DISABLE();
    }
}

/* [] END OF FILE */
