#include <project.h>
#include "path_controller.h"
#include "systime.h"

void path_controller_init() {
    sensors_controller_init();
    motor_controller_init();
}

PCData path_controller_create(int8_t initial_heading, bool use_wireless, bool use_line) {
    SCData sc_data = sensors_controller_create(30, use_wireless, use_line);
    MCData mc_data = motor_controller_create(30, &sc_data);
    PCData data = {
        .sample_time = 30,
        .sc_data = sc_data,
        .mc_data = mc_data,
        .heading = initial_heading,
        .last_run = 0
    };
    return data;
}

void path_controller_worker(PCData* data) {
    sensors_controller_worker(&(data->sc_data));

    uint32_t now = systime_ms();
    uint32_t time_diff = now - data->last_run;
    if (time_diff >= data->sample_time) {
        if (data->sc_data.line_end && data->sc_data.curr_intersection > 0 && data->mc_data.drive_mode != 1) {
            switch (data->sc_data.curr_intersection) {
                case 1:
                motor_controller_set(&(data->mc_data), 0.5, 1, 90);
                break;
                case 2:
                motor_controller_set(&(data->mc_data), 0.5, 1, -90);
                break;
                case 3:
                // Go where heading is.
                break;
                default:
                break;
            }
        }
        else if (data->mc_data.drive_mode != 3) {
            motor_controller_set(&(data->mc_data), 0.8, 3, 0);
        }
    }
    
    motor_controller_worker(&(data->mc_data));
}
