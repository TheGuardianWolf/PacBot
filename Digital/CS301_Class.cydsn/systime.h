#ifndef SYSTIME_H
#define SYSTIME_H

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
    
#define SYSTIME_COUNTER_MAX 0xFFFFFFFF

void system_time_init();

uint32_t system_time_ms();

uint32_t system_time_s();

/* [] END OF FILE */

#endif /* SYSTIME_H */
