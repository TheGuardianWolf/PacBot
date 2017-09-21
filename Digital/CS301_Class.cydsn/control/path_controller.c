#include <project.h>
#include "sensors.h"
#include "path_controller.h"

static float line_bias[6][2] = {
    {-0.2, -0.2},
    {-0.4, 0.2},
    {0.2, -0.4},
    {-0.2, 0.1},
    {0.1, -0.2},
    {-0.5, -0.5}
};

void path_controller_init() {
    sensors_controller_init();
    motor_controller_init();
}

PCData path_controller_create(int8_t initial_heading) {
    PCData data = {
        .mc_data = motor_controller_create(),
        .sc_data = sensors_controller_create()
    };
    return data;
}

void path_controller_worker(PCData* data) {
    sensors_controller_worker(&(data->sc_data));
    motor_controller_worker(&(data->mc_data));
}
