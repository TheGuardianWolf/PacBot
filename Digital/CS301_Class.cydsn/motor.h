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
    
#define M_MAX 127
#define M_MIN -127
#define ML_SET(x) M1_PWM_WritePeriod(x)
#define MR_SET(x) M2_PWM_WritePeriod(x)
#define ML_DISABLE M1_D1_Write(1)
#define MR_DISABLE M2_D1_Write(1)

void motor_init();

void motor_set(int8_t speed_L, int8_t speed_R);

/* [] END OF FILE */

#endif /* MOTOR_H */
