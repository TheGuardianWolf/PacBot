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

#include "systime.h"
#include <project.h>

void systime_init() {
    SystemClock_Start();
}

uint32_t systime_ms() {
    return ((uint32_t) SystemClock_ReadCounter() - SYSTIME_COUNTER_MAX) / 10;
}

uint32_t systime_s() {
    return ((uint32_t) SystemClock_ReadCounter() - SYSTIME_COUNTER_MAX) / 10000;
}