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

#define M_MAX  92// Full forward
#define M_MIN -93  // Full reverse
#define M_DRIFT -1  // Drift

typedef struct {
    int8_t L;
    int8_t R;
}  MotorData;

void motor_init();

MotorData motor_get();

void motor_set(MotorData data);

void motor_disable(bool disable_L, bool disable_R);

/* [] END OF FILE */

#endif /* MOTOR_H */
