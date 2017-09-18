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

static bool systime_started = false;

void systime_init() {
    systime_started = true;
    SystemClock_Start();
}

uint32_t systime_ms() {
    return ((uint32_t) SYSTIME_COUNTER_MAX_24 - SystemClock_ReadCounter()) / 10;
}

uint32_t systime_s() {
    return ((uint32_t) SYSTIME_COUNTER_MAX_24 - SystemClock_ReadCounter()) / 10000;
}
