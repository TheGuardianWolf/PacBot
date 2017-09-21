#include <project.h>
#include "path_controller.h"

void path_controller_init() {
    sensors_controller_init();
    motor_controller_init();
}

PCData path_controller_create(int8_t initial_heading, bool use_wireless, bool use_line) {
    SCData sc_data = sensors_controller_create(30, use_wireless, use_line);
    MCData mc_data = motor_controller_create(30, &sc_data);
    PCData data = {
        .sc_data = sc_data,
        .mc_data = mc_data,
        .heading = initial_heading
    };
    return data;
}

void path_controller_worker(PCData* data) {
    sensors_controller_worker(&(data->sc_data));
    motor_controller_worker(&(data->mc_data));
}
