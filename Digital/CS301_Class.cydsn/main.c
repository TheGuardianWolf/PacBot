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

#define MAX_CMPS 60
#define STRAIGHT_LINE_SPEED 48
#define STRAIGHT_LINE_DISTANCE 1000

static SCData scd;
static MCData mcd;

static void system_init() {
    systime_init();
    sensors_controller_init();
    motor_controller_init();
    usb_init();
    CYGlobalIntEnable;
}

static float straight_line_speed(float cmps) {
    //0.1 == 9 // 0.2 == 17 // 0.3 == 25 0.4 == 35 // 0.6 == 53 // 0.7 == 60
    return 0.01149265 * cmps + 0.0021600519;
}

static void curved_line_tracking() {
    scd = sensors_controller_create(30, false, true);
    mcd = motor_controller_create(30, &scd);
    motor_controller_set(&mcd, 0.3f, 0, 0xEFFFFFF);
    while (true) {
        sensors_controller_worker(&scd);
        motor_controller_worker(&mcd);
    }
}

static void straight_line() {
    scd = sensors_controller_create(30, true, false);
    mcd = motor_controller_create(30, &scd);
    motor_controller_set(&mcd, straight_line_speed(STRAIGHT_LINE_SPEED/*cms^-1*/), 0, STRAIGHT_LINE_DISTANCE/*mm*/);
    while (true) {
        sensors_controller_worker(&scd);
        motor_controller_worker(&mcd);
    }
}

static void straight_line_no_line() {
    scd = sensors_controller_create(30, false, false);
    mcd = motor_controller_create(30, &scd);
    motor_controller_set(&mcd, straight_line_speed(STRAIGHT_LINE_SPEED/*cms^-1*/), 0, STRAIGHT_LINE_DISTANCE/*mm*/);
    while (true) {
        sensors_controller_worker(&scd);
        motor_controller_worker(&mcd);
    }
}


static void line_intersections() {
    static uint32_t last_run = 0;
    scd = sensors_controller_create(30, false, true);
    mcd = motor_controller_create(30, &scd);
    while(true) {
        sensors_controller_worker(&scd);

        // uint32_t now = systime_ms();
        // uint32_t time_diff = now - line_intersections_last_run;
        // if (time_diff >= 30) {
        //     last_run = now;
        //     if (scd.line_curve == 0 && scd.line_front_lost && scd.curr_intersection > 0 && mcd.drive_mode != 1) {
        //         switch (scd.curr_intersection) {
        //         case DI_L:
        //             motor_controller_set(&mcd, 0.2f, 1, -90);
        //             break;
        //         case DI_R:
        //             motor_controller_set(&mcd, 0.2f, 1, 90);
        //             break;
        //         case DI_LR:
        //             motor_controller_set(&mcd, 0.15f, 0, 100);
        //             break;
        //         default:
        //             break;
        //         }
        //     }
        //     else {
        //         motor_controller_set(&mcd, 0.15f, 0, 0xFFFF);
        //     }
        // }

        motor_controller_worker(&mcd);
    }
}

int main() {
    system_init();
    led_set(0b111);
    while(true) {
        uint8_t run_mode = REG_DIP_Read() >> 1 & 0b111;
        REG_LED_Write(run_mode);
        if(btn_get()) {
            led_set(0b000);
            uint32_t time = systime_s();
            while(systime_s() - time < 2);
            if (dipsw_get(1)) {
                led_set(0b001);
                curved_line_tracking();
            }
            else if (dipsw_get(2)) {
                led_set(0b010);
                straight_line(); 
            }
            else if (dipsw_get(3)) {
                led_set(0b100);
                straight_line_no_line();
            }
            led_set(0b111);
        }
    }
    return 0; 
}

/* [] END OF FILE */

//0.15f 0.1 straight bias and 0
//