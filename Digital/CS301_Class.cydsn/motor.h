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

#define 
#define M_MAX  92// Full forward
#define M_MIN -93  // Full reverse
#define M_DRIFT -1  // Drift
#define ML_SET(x) M1_PWM_WriteCompare(x)
#define MR_SET(x) M2_PWM_WriteCompare(x)
#define ML_DISABLE(x) M1_D1_Write(x)
#define MR_DISABLE(x) M2_D1_Write(x)

void motor_init();

void motor_set(int8_t speed_L, int8_t speed_R);

void motor_disable(bool disable_L, bool disable_R);

/* [] END OF FILE */

#endif /* MOTOR_H */
