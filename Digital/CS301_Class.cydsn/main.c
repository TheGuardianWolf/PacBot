/* ========================================
 * Fully working code: 
 * PWM      : 
 * Encoder  : 
 * ADC      :
 * USB      : port displays speed and position.
 * CMD: "PW xx"
 * Copyright Univ of Auckland, 2016
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF Univ of Auckland.
 *
 * ========================================
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <project.h>
//* ========================================
#include "usb.h"
#include "vars.h"
#include "defines.h"
//* ========================================

static char displaystring[BUF_SIZE] = "CS301 2016\n";

void init() {
    CYGlobalIntEnable;
    USE_RF;
    usb_init();
}

int main()
{
    init();
    for(;;)
    {     
    }   
}


/* [] END OF FILE */
