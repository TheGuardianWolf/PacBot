#include <project.h>
#include "motor.h"

static uint8_t speed2pwm(int8_t speed) {
    if (speed < 0) {
        return ((uint8_t) speed - 128 - 35);  // Dead zone compensation of -35
    }
    return (uint8_t) speed + 128 + 36;  // Dead zone compensation of 36
}

void motor_init() {
    motor_disable_L(true);
    motor_disable_R(true);
    M1_PWM_Start();
    M2_PWM_Start();
}

void motor_set_L(int8_t speed) {
    M1_PWM_WriteCompare(speed2pwm(speed));
    motor_disable_L(speed == M_DRIFT)
}

void motor_set_R(int8_t speed) {
    M2_PWM_WriteCompare(speed2pwm(speed));
    motor_disable_R(speed== M_DRIFT);
}

void motor_disable_L(bool disable) {
    M1_D1_Write((uint8_t) disable);
}

void motor_disable_R(bool disable) {
    M2_D1_Write((uint8_t) disable);
}

/* [] END OF FILE */
