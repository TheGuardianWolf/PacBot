#include <project.h>
#include "motor.h"

static uint8_t reg_m_d1 = 0b11;

static uint8_t speed2pwm(int8_t speed) {
    if (speed < 0) {
        return ((uint8_t) speed - 128 - 34);  // Dead zone compensation of -35
    }
    return (uint8_t) speed + 128 + 35;  // Dead zone compensation of 36
}

void motor_init() {
    motor_disable(true);
    M1_PWM_Start();
    M2_PWM_Start();
}

void motor_set_L(int8_t speed) {
    M1_PWM_WriteCompare(speed2pwm(speed));
    motor_disable_L(speed == M_DRIFT);
}

void motor_set_R(int8_t speed) {
    M2_PWM_WriteCompare(speed2pwm(speed));
    motor_disable_R(speed== M_DRIFT);
}

void motor_disable_L(bool disable) {
    if (disable) {
        reg_m_d1 |= 0b01;
    }
    else {
        reg_m_d1 &= 0b10;
    }
    REG_M_D1_Write(reg_m_d1);
}

void motor_disable_R(bool disable) {
    if (disable) {
        reg_m_d1 |= 0b10;
    }
    else {
        reg_m_d1 &= 0b01;
    }
    REG_M_D1_Write(reg_m_d1);
}

void motor_disable(bool disable) {
    if (disable) {
        reg_m_d1 |= 0b11;
    }
    else {
        reg_m_d1 &= 0b00;
    }
    REG_M_D1_Write(reg_m_d1);
}

/* [] END OF FILE */
