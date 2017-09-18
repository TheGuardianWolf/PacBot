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
#include <stdbool.h>
    
#define SYSTIME_COUNTER_MAX_32 0xFFFFFFFF      // Max count is about one century or something
#define SYSTIME_COUNTER_MAX_24 0xFFFFFF        // Max count is about 30 minutes

void systime_init();

uint32_t systime_ms();

uint32_t systime_s();

/* [] END OF FILE */

#endif /* SYSTIME_H */
