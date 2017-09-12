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
        .sc_data = sensors_controller_create(),
        .heading = initial_heading,
        .intersection = 0,
        .path_end = false
    };
    return data;
}

void path_controller_worker(PCData* data) {
    if (LINE(0)) {
        if (LINE(1)) {
            if (LINE(2)) {
                data->intersection = 3;
            }
            else {
                data->intersection = 1;
            }
        }
        if (LINE(2)) {
            data->intersection = 2;
        }
    }
    if (LINE(3) && LINE(4)) {
        data->path_end = true;
    }

    uint8_t bias_L = 0;
    uint8_t bias_R = 0;

    for (uint8_t i = 0; i < LINE_SENSORS; i++) {
        bias_L += (uint8_t) LINE(i) * line_bias[i][0];
        bias_R += (uint8_t) LINE(i) * line_bias[i][1];
    }

    data->mc_data.bias_L = bias_L;
    data->mc_data.bias_R = bias_R;
}
