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
#include "map.h"
// #include "motor.h"

#define TOUCH_UI 1
#define MAX_CMPS 60

static uint8_t grid PACMAN_MAP;
static uint8_t food_list PACMAN_FOOD_LIST;

static SCData scd;
static MCData mcd;
static PCData pcd;

static void system_init() {
    systime_init();
    sensors_controller_init();
    motor_controller_init();
    usb_init();
    wireless_init();
    CYGlobalIntEnable;
}

//static float real_speed(float cmps) {
//    //0.1 == 9 // 0.2 == 17 // 0.3 == 25 0.4 == 35 // 0.6 == 53 // 0.7 == 60
//    return 0.01149265 * cmps + 0.0021600519;
//}

static void command_test();

static void maze_runner();

static void flash_invalid();

#if TOUCH_UI == 1
int main() {
    // Red light for initialisation
    led_set(1);
    system_init();
    scd = sensors_controller_create(15, false, true);
    mcd = motor_controller_create(15, &scd);
    pcd = path_controller_create(50, &scd, &mcd);

    bool pressed = false;
    bool held = false;
    uint32_t last_time = 0;
    uint8_t selection = 0;
    int8_t run_mode = -1;
    int8_t initial_heading = 0;
    uint8_t state = 0;
    bool ready = false;

    point_uint8_t start = {
        .x = PACMAN_START_X,
        .y = PACMAN_START_Y
    };

    path_controller_load_data(&pcd, (uint8_t*) &grid, PACMAN_MAP_HEIGHT, PACMAN_MAP_WIDTH, (uint8_t*) &food_list, PACMAN_FOOD_LIST_HEIGHT, start);

    led_set(0);
    while(true) {
        if(btn_get()) {
            last_time = systime_ms();
            while(btn_get()) {
                if (systime_ms() - last_time >= 1000) {
                    held = true;
                    pressed = false;
                    led_set(0b111);
                }
                else {
                    pressed = true;
                    held = false;
                }
            };
        }

        if (pressed) {
            if (selection >= 7) {
                selection = 0;
            }
            else {
                selection++;
            }
        }
        led_set(selection);

        switch(state) {
        case 0:
            if (held) {
                if (selection > 0 && selection < 4) {
                    run_mode = selection;
                    selection = 0;
                    state++;
                }
                else {
                    flash_invalid();
                }
            }
            break;
        case 1:
            if (held) {
                if (selection > 0 && selection < 5) {
                    initial_heading = selection;
                    selection = 0;
                    state++;
                }
                else {
                    flash_invalid();
                }
            }
            break;
        case 2:
            if (held) {
                ready = true;
            }
            else {
                led_set(0b010);
            }
            break;
        default:
            state = 0;
            break;
        }
        if (ready) {
            uint32_t td = 0;
            last_time = systime_ms();
            while (td <= 3000) {
                td = systime_ms() - last_time;
                if (td < 1000) {
                    led_set(0b001);
                }
                else if (td < 2000) {
                    led_set(0b100);
                }
                else {
                    led_set(0b010);
                }
            }
            led_set(0);
            pcd.heading = initial_heading;
            if (run_mode == 1) {
                pcd.path = pcd.astar_path;
                maze_runner();
            }
            if (run_mode == 2) {
                pcd.path = pcd.travel_path;
                maze_runner();
            }
            else if (run_mode == 3) {
                command_test();
            }
        }

        pressed = false;
        held = false;
    }
    return 0;
}
#else
int main() {
    led_set(1);
    system_init();
    scd = sensors_controller_create(15, false, true);
    mcd = motor_controller_create(15, &scd);
    pcd = path_controller_create(30, &scd, &mcd);
    uint8_t grid PACMAN_MAP;
    uint8_t food_list PACMAN_FOOD_LIST;
    point_uint8_t start = {
        .x = PACMAN_START_X,
        .y = PACMAN_START_Y
    };
    path_controller_load_data(&pcd, (uint8_t*) &grid, PACMAN_MAP_HEIGHT, PACMAN_MAP_WIDTH, (uint8_t*) &food_list, PACMAN_FOOD_LIST_HEIGHT, start);
    led_set(0);
    while(true) {
       int8_t initial_heading = ((REG_DIP_Read() >> 2) & 0b0011) + 1;
       uint8_t run_mode = REG_DIP_Read() & 0b0011;
       led_set(((((uint8_t)(initial_heading - 1) << 2) & (run_mode & 0b11));
        if(btn_get()) {
            uint32_t time = systime_s();
            while(systime_s() - time < 2);
           if (run_mode == 0) {
               pcd.path = pcd.astar_path;
               maze_runner();
           }
           if (run_mode == 1) {
               pcd.path = pcd.travel_path;
               maze_runner();
           }
           else if (run_mode == 2) {
               command_test();
               //motor_test();
           }
        }
    }
    return 0;
}
#endif

static void maze_runner() {
    while (true) {
        sensors_controller_worker(&scd);
        path_controller_worker(&pcd);
        motor_controller_worker(&mcd);
    }
}

static void command_test() {
    MotorCommand cmd = {
        .speed = 0.3f,
        .drive_mode = 0,
        .arg = GRID_BLOCK_WIDTH * 2
    };
    path_controller_add_command(&pcd, &cmd);

    cmd.drive_mode = 1;
    cmd.speed = 0.3f;
    cmd.arg = 85;
    path_controller_add_command(&pcd, &cmd);

    cmd.drive_mode = 0;
    cmd.speed = 0.3f;
    cmd.arg = GRID_BLOCK_HEIGHT * 2;
    path_controller_add_command(&pcd, &cmd);

    cmd.drive_mode = 1;
    cmd.speed = 0.3f;
    cmd.arg = 85;
    path_controller_add_command(&pcd, &cmd);

    cmd.drive_mode = 0;
    cmd.speed = 0.3f;
    cmd.arg = GRID_BLOCK_WIDTH * 2;
    path_controller_add_command(&pcd, &cmd);

    cmd.drive_mode = 1;
    cmd.arg = -85;
    path_controller_add_command(&pcd, &cmd);

    cmd.drive_mode = 0;
    cmd.arg = GRID_BLOCK_HEIGHT * 2;
    path_controller_add_command(&pcd, &cmd);

    cmd.drive_mode = 1;
    cmd.arg = -85;
    path_controller_add_command(&pcd, &cmd);

    cmd.drive_mode = 0;
    cmd.arg = GRID_BLOCK_WIDTH * 6;
    path_controller_add_command(&pcd, &cmd);

    cmd.drive_mode = 1;
    cmd.arg = -85;
    path_controller_add_command(&pcd, &cmd);

    cmd.drive_mode = 0;
    cmd.arg = GRID_BLOCK_HEIGHT * 2;
    path_controller_add_command(&pcd, &cmd);

    cmd.drive_mode = 1;
    cmd.arg = 85;
    path_controller_add_command(&pcd, &cmd);

    cmd.drive_mode = 0;
    cmd.arg = GRID_BLOCK_WIDTH * 10;
    path_controller_add_command(&pcd, &cmd);

    cmd.drive_mode = 1;
    cmd.arg = 185;
    path_controller_add_command(&pcd, &cmd);

    while (true) {
        sensors_controller_worker(&scd);
        path_controller_worker(&pcd);
        motor_controller_worker(&mcd);

        led_set(pcd.command_queue->size);
    }
}

static void flash_invalid() {
    uint32_t last_time = systime_ms();
    led_set(0);
    while(systime_ms() - last_time < 500);
    last_time = systime_ms();
    led_set(0b100);
    while(systime_ms() - last_time < 500);
    last_time = systime_ms();
    led_set(0);
    while(systime_ms() - last_time < 500);
    last_time = systime_ms();
    led_set(0b100);
    while(systime_ms() - last_time < 500);
    last_time = systime_ms();
    led_set(0);
}
