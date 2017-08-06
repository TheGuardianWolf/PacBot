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

/* [] END OF FILE */

#include "pid.h"
#include <project.h>
#include "debug.h"

CY_ISR(pid_timer) {
    
}

void Compute()

void quad_dec_init() {
    isr_PID_timer_StartEx(pid_timer);
    isr_PID_timer_Start();
}