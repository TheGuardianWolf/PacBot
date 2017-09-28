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
    motor_controller_set(&mcd, 0.15f, 0, 0xEFFFFFF);
    while (true) {
        motor_controller_worker(&mcd);
    }
}

static void straight_line() {
    scd = sensors_controller_create(30, false, true);
    mcd = motor_controller_create(30, &scd);
    motor_controller_set(&mcd, straight_line_speed(30/*cms^-1*/), 0, 1000/*mm*/);
    while (true) {
        sensors_controller_worker(&mcd);
        motor_controller_worker(&mcd);
    }
}


static void line_intersections() {
    static uint32_t last_run = 0;
    scd = sensors_controller_create(30, false, true);
    mcd = motor_controller_create(30, &scd);
    while(true) {
        sensors_controller_worker(&mcd);

        uint32_t now = systime_ms();
        uint32_t time_diff = now - line_intersections_last_run;
        if (time_diff >= 30) {
            last_run = now;
            if (scd.line_curve == 0 && scd.line_front_lost && scd.curr_intersection > 0 && mcd.drive_mode != 1) {
                switch (scd.curr_intersection) {
                case DI_L:
                    motor_controller_set(&mcd, 0.2f, 1, -90);
                    break;
                case DI_R:
                    motor_controller_set(&mcd, 0.2f, 1, 90);
                    break;
                case DI_LR:
                    motor_controller_set(&mcd, 0.15f, 0, 100);
                    break;
                default:
                    break;
                }
            }
            else {
                motor_controller_set(&mcd, 0.15f, 0, 0xFFFF);
            }
        }

        motor_controller_worker(&mcd);
    }
}

int main() {
    system_init();
    led_set(0b111);
    while(true) {
        if(btn_get()) {
            led_set(0b000);
            uint32_t time = systime_s();
            while(systime_s() - time < 2);
            uint8_t run_mode = REG_DIP_Read() & 0b111;
            switch(run_mode) {
            case 1:
                // Curves
                curved_line_tracking();
                break;
            case 2:
                // Intersections
                line_intersections();
                break;
            case 3:
                // Straight line
                straight_line();
                break;
            default:
                break;
            }
            led_set(0b111);
        }
    }
    else {
    }
    return 0;
}

/* [] END OF FILE */
