#include <stdbool.h>
#include <project.h>
// #include <unistd.h>
#include <stdio.h>

#include "adc.h"
#include "motor_controller.h"
#include "sensors_controller.h"
#include "interactive.h"
#include "systime.h"
#include "usb.h"

void system_init() {
    systime_init();
    sensors_controller_init();
    motor_controller_init();
    CYGlobalIntEnable;
}

int main() {
    system_init();
    // SCData scd = sensors_controller_create(30, false, false);
    // MCData mcd = motor_controller_create(30, &scd);
    uint32_t td = 0;
    led_set(0b111);
    while(true) {
        if(btn_get()) {
            led_set(0b000);
            while(btn_get());
            if(!btn_get()) {
                td = systime_ms();
                // motor_controller_set(&mcd, 0.8f, 0,300);
                while (systime_ms() - td < 1000);
                while (true) {
                    // sensors_controller_worker(&scd);
                    // motor_controller_worker(&mcd);
                }
            }
            
        }
        else {
            //  if (systime_ms() - td >= 1000){
            //      td = systime_ms();
            //      RFData rf_data = wireless_get();
            //      char buffer[64];
            //      sprintf(buffer, "%d\r\n%d\r\n%d\r\n%d\r\n%d\r\n%d\r\n%d\r\n%d\r\n", (int)rf_data.rssi, (int)rf_data.index, (int)rf_data.g0_direction, (int)rf_data.g0_speed, (int)rf_data.g0_xpos, (int)rf_data.g0_ypos, (int)rf_data.g1_speed, (int)rf_data.g1_xpos);
            //      usb_send_string(buffer);
            //      sprintf(buffer, "%d\r\n%d\r\n%d\r\n%d\r\n%d\r\n%d\r\n%d\r\n%d\r\n%d\r\n", (int)rf_data.g1_ypos, (int)rf_data.g2_direction, (int)rf_data.g2_speed, (int)rf_data.g2_xpos, (int)rf_data.g2_ypos, (int)rf_data.robot_orientation, (int)rf_data.robot_xpos, (int)rf_data.robot_ypos, (int)rf_data.g1_direction);
            //      usb_send_string(buffer);
            //  }
                //  if (systime_ms() - td >= 1000){
            //      td = systime_ms();
            //      RFData rf_data = wireless_get();
            //      char buffer[64];
            //      sprintf(buffer, "%d\r\n%d\r\n%d\r\n%d\r\n%d\r\n%d\r\n%d\r\n%d\r\n", (int)rf_data.rssi, (int)rf_data.index, (int)rf_data.g0_direction, (int)rf_data.g0_speed, (int)rf_data.g0_xpos, (int)rf_data.g0_ypos, (int)rf_data.g1_speed, (int)rf_data.g1_xpos);
            //      usb_send_string(buffer);
            //      sprintf(buffer, "%d\r\n%d\r\n%d\r\n%d\r\n%d\r\n%d\r\n%d\r\n%d\r\n%d\r\n", (int)rf_data.g1_ypos, (int)rf_data.g2_direction, (int)rf_data.g2_speed, (int)rf_data.g2_xpos, (int)rf_data.g2_ypos, (int)rf_data.robot_orientation, (int)rf_data.robot_xpos, (int)rf_data.robot_ypos, (int)rf_data.g1_direction);
            //      usb_send_string(buffer);
            //  }
        }  
    }
    return 0;
}

/* [] END OF FILE */
