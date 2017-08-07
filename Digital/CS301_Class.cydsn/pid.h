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

#define timeInterval 1000    
    
CY_ISR_PROTO(isr_PID_timer_Interrupt);

void pid_timer_init();

void Compute();

double getExpectedRotations();

void setPIDL(double KpL, double KiL, double KdL);

void setPIDR(double KpR, double KiR, double KdR);

unsigned long getTime();

#endif /* PID_H */