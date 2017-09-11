#ifndef MOTOR_H
#define MOTOR_H
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

#include <cytypes.h>
#include <stdbool.h>

#define M_MAX  127  // Full forward
#define M_MIN  35  // Minimum drive speed
#define M_DRIFT 0  // Drift

void motor_init();

void motor_set_L(int8_t speed);

void motor_set_R(int8_t speed);

void motor_disable_L(bool disable);

void motor_disable_R(bool disable);

void motor_disable(bool disable);

/* [] END OF FILE */

#endif /* MOTOR_H */
