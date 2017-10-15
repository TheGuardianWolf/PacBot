#include <project.h>
#include "path_controller.h"
#include "systime.h"
#include <stdlib.h>

void path_controller_init() {
    sensors_controller_init();
    motor_controller_init();
}

PCData path_controller_create(int8_t initial_heading, bool use_wireless, bool use_line) {
    SCData* sc_data = malloc(sizeof(SCData));
    *sc_data = sensors_controller_create(30, use_wireless, use_line);
    MCData* mc_data = malloc(sizeof(MCData));
    *mc_data = motor_controller_create(30, sc_data);
    PCData data = {
        .sample_time = 30,
        .sc_data = sc_data,
        .mc_data = mc_data,
        .prev_heading = initial_heading,
        .heading = initial_heading,
        .last_run = 0
    };
    motor_controller_set(data.mc_data, 0.15, 0, 0xFFFF);
    return data;
}

//void calculate_turn(PCData* data, SCData *sc_data) {
//    //heading east
//    if(sc_data->curr_loc.orientation < 15 || sc_data->curr_loc.orientation > 345) {
//        
//    }
//    //heading east
//    else if(sc_data->curr_loc.orientation < 15 || sc_data->curr_loc.orientation > 345) {
//        
//    }
//    //heading east
//    else if(sc_data->curr_loc.orientation < 15 || sc_data->curr_loc.orientation > 345) {
//        
//    }
//    //heading east
//    else if(sc_data->curr_loc.orientation < 15 || sc_data->curr_loc.orientation > 345) {
//        
//    }
//    else {
//    
//    }
//   
//    
//}

void path_controller_worker(PCData* data) {
    sensors_controller_worker(data->sc_data);
    
    // uint32_t now = systime_ms();
    // uint32_t time_diff = now - data->last_run;
    // if (time_diff >= data->sample_time) {
    //     if (data->sc_data.line_curve == 0 && data->sc_data.line_end && data->sc_data.curr_intersection > 0 && data->mc_data.drive_mode != 1) {
    //         switch (data->sc_data.curr_intersection) {
    //             case DI_L:
    //             REG_LED_Write(0b010);
    //             motor_controller_set(&(data->mc_data), 0.3, 1, 90);
    //             break;
    //             case DI_R:
    //             REG_LED_Write(0b001);
    //             motor_controller_set(&(data->mc_data), 0.3, 1, -90);
    //             break;
    //             case DI_LR:
    //             motor_controller_set(&(data->mc_data), 0.15, 0, 100);
    //             REG_LED_Write(0b111);
    //             break;
    //             default:
    //             REG_LED_Write(0b100);
    //             break;
    //         }
    //     }
    //     else {
    //         REG_LED_Write(0b101);
    //         motor_controller_set(&(data->mc_data), 0.15, 0, 0xFFFF);
    //     }
    // }
    motor_controller_worker(data->mc_data);
}
