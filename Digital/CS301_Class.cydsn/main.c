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
#include "wireless.h"

#define MAX_CMPS 60

static SCData scd;
static MCData mcd;
static PCData pcd;
static Graph* graph;

static void system_init() {
    systime_init();
    sensors_controller_init();
    motor_controller_init();
    usb_init();
    wireless_init();
    CYGlobalIntEnable;
}

static float real_speed(float cmps) {
    //0.1 == 9 // 0.2 == 17 // 0.3 == 25 0.4 == 35 // 0.6 == 53 // 0.7 == 60
    return 0.01149265 * cmps + 0.0021600519;
}

static void maze_runner() {
    scd = sensors_controller_create(30, false, true);
    mcd = motor_controller_create(30, &scd);
    scd = path_controller_create(30);
    motor_controller_set(&mcd, 0.2f, 0, 0xEFFFFFF);
    while (true) {
        sensors_controller_worker(&scd);
        path_controller_worker(&pcd, &mcd, &scd);
        motor_controller_worker(&mcd);
    }
}

int main() {
    system_init();
    led_set(0b111);
    while(true) {
        uint8_t run_mode = REG_DIP_Read();
        REG_LED_Write(run_mode);
        if(btn_get()) {
            uint32_t time = systime_s();
            while(systime_s() - time < 2);
            if (run_mode == 1) {
                led_set(run_mode);
                maze_runner();
            }
            else {
                led_set(0b000);
            }
        }
    }
    return 0; 
}

/* [] END OF FILE */

//0.15f 0.1 straight bias and 0
//
