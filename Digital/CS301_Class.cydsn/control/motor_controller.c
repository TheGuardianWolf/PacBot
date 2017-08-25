#include <project.h>

#include "systime.h"
#include "motor.h"
#include "quad_dec.h"


void motor_controller_init() {
    systime_init();
    motor_init();
    quad_dec_init();
}

void motor_controller_worker(MCData* data) {
    uint32_t now = systime_ms();
    if (now - data->last_run >= data->sample_time) {
        pid_compute(data);
        data->last_run = now;
    }
}
