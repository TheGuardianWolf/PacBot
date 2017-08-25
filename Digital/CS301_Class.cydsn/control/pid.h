#ifndef PID_H
#define PID_H
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

#define PID_INTERVAL 100 // ms

void pid_init();

void pid_worker();

void pid_timer_init();

void Compute();

float getExpectedRotations();

void setPIDL(float KpL, float KiL, float KdL);

void setPIDR(float KpR, float KiR, float KdR);

unsigned long getTime();

#endif /* PID_H */
