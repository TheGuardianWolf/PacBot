#include <stdbool.h>
#include <project.h>
// #include <unistd.h>
#include <stdio.h>

#include "adc.h"
#include "motor_controller.h"
#include "interactive.h"
#include "motor.h"

#include "systime.h"
#include "usb.h"
#include "wireless.h"

void system_init() {
    // Starting the IAMP opamp only. If issues are encountered, set the input
    // of IAMP to pull-down resistive.
    systime_init();
    wireless_init();
    usb_init();
    motor_controller_init();
    CYGlobalIntEnable;
}

int main() {
    system_init();
    MCData mcd = motor_controller_create();
    uint32_t td = 0;
    led_set(0b001);
    while(true) {
        // Without button press, default to RF and ADC output mode
        // With button press, go to straight line test
        if(btn_get()) {
            led_set(0b000);
            while(btn_get());
            if(!btn_get()) {
                td = systime_ms();
                motor_controller_set(&mcd, 1, 90);
                while (systime_ms() - td < 1000);
                while (true) {
                    // Signature is MCData, left wheel distance (mm), right wheel distance (mm)
                    motor_controller_worker(&mcd);
                }
            }
            
        }
        else {
             if (systime_ms() - td >= 1000){
                 td = systime_ms();
                 RFData rf_data = wireless_get();
                 char buffer[64];
                 sprintf(buffer, "%d\r\n%d\r\n%d\r\n%d\r\n%d\r\n%d\r\n%d\r\n%d\r\n", (int)rf_data.rssi, (int)rf_data.index, (int)rf_data.g0_direction, (int)rf_data.g0_speed, (int)rf_data.g0_xpos, (int)rf_data.g0_ypos, (int)rf_data.g1_speed, (int)rf_data.g1_xpos);
                 usb_send_string(buffer);
                 sprintf(buffer, "%d\r\n%d\r\n%d\r\n%d\r\n%d\r\n%d\r\n%d\r\n%d\r\n%d\r\n", (int)rf_data.g1_ypos, (int)rf_data.g2_direction, (int)rf_data.g2_speed, (int)rf_data.g2_xpos, (int)rf_data.g2_ypos, (int)rf_data.robot_orientation, (int)rf_data.robot_xpos, (int)rf_data.robot_ypos, (int)rf_data.g1_direction);
                 usb_send_string(buffer);
             }
        }  
    }
    return 0;
}

/* [] END OF FILE */
