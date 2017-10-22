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

#define MAX_CMPS 60
#define START_ORIENTATION G_N
    
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

        //led_set(pcd.command_queue->size);
    }
}

static void maze_runner() {
    while (true) {
        sensors_controller_worker(&scd);
        path_controller_worker(&pcd);
        motor_controller_worker(&mcd);
    }
}

int main() {
    system_init();
    scd = sensors_controller_create(30, false, true);
    mcd = motor_controller_create(30, &scd);
    pcd = path_controller_create(30, &scd, &mcd);
    led_set(1);
    uint8_t grid PACMAN_MAP;
    uint8_t food_list PACMAN_FOOD_LIST;
    point_uint8_t start = {
        .x = PACMAN_START_X,
        .y = PACMAN_START_Y
    };
    path_controller_load_data(&pcd, (uint8_t*) &grid, PACMAN_MAP_HEIGHT, PACMAN_MAP_WIDTH, (uint8_t*) &food_list, PACMAN_FOOD_LIST_SIZE, start);
    while(true) {
        int8_t initial_heading = ((REG_DIP_Read() >> 2) & 0b0011) + 1;
        pcd.heading = initial_heading;
        uint8_t run_mode = REG_DIP_Read() & 0b0011;
        led_set(pcd.heading);
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
                pcd.pathfinder = false;
                command_test();
            }
        }
    }
    return 0; 
}

/* [] END OF FILE */

//0.15f 0.1 straight bias and 0
//
