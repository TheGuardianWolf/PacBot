#include <stdbool.h>
#include <project.h>
// #include <unistd.h>
#include <stdio.h>
#include "motor_controller.h"
#include "sensors_controller.h"
#include "path_controller.h"
#include "interactive.h"
#include "systime.h"
#include "usb.h"

#define MAX_CMPS 70

static void system_init() {
    systime_init();
    sensors_controller_init();
    motor_controller_init();
    usb_init();
    CYGlobalIntEnable;
}

static float straight_line_speed(float cmps) {
    return 0.01149265 * cmps + 0.0021600519;
}

int main() {
    system_init();
    led_set(0b111);
    while(true) {
        if(btn_get()) {
            led_set(0b000);
            if(!btn_get()) {
                uint32_t time = systime_s();
                while(systime_s() - time <= 2);
                uint8_t run_mode = REG_DIP_Read() >> 2 & 0b11;
                bool use_wireless = dipsw_get(2);
                bool use_line = dipsw_get(3); 
                SCData scd;
                MCData mcd;
                PCData pcd;
                switch(run_mode) {
                    case 0:
                    // Curves
                    scd = sensors_controller_create(30, use_wireless, use_line);
                    mcd = motor_controller_create(30, &scd);
                    motor_controller_set(&mcd, 0.15f, 0, 0xEFFFFFF); //0.1 == 9 // 0.2 == 17 // 0.3 == 25 0.4 == 35 // 0.6 == 53 // 0.7 == 60
                    break;
                    case 1:
                    // Intersections
                    pcd = path_controller_create(0, use_wireless, use_line);
                    break;
                    case 2:
                    // Straight line
                    scd = sensors_controller_create(30, use_wireless, use_line);
                    mcd = motor_controller_create(30, &scd);
                    motor_controller_set(&mcd, straight_line_speed(50), 0, 0xEFFFFFF);
                    break;
                    case 3:
                    break;
                    default:
                    break;
                }
                while (true) {
                    sensors_controller_worker(&scd);
                    motor_controller_worker(&mcd);
                }
                led_set(0b111);
            }
        }
        else {
//            if (dipsw_get(3)) {
//                if (time - td >= 1000) {
//                    td = time;
//                    char buffer[64];
//                    LineData line_data = sensors_line_get();
//                    sprintf(buffer, "0:%d 1:%d 2:%d 3:%d 4:%d 5:%d\r\n",
//                            (int) (line_data.state[0]),
//                            (int) (line_data.state[1]),
//                            (int) (line_data.state[2]),
//                            (int) (line_data.state[3]),
//                            (int) (line_data.state[4]),
//                            (int) (line_data.state[5])
//                            );
//                    usb_send_string(buffer);
//                }
//            }
        }
    }
    return 0;
}

/* [] END OF FILE */
