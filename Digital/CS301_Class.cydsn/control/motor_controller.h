#ifndef MOTOR_CONTROLLER_H
#define MOTOR_CONTROLLER_H

#include <cytypes.h>
#include <stdbool.h>

typedef struct {
    uint32_t last_run;
    PIDData PID_mspeed_L;
    PIDData PID_mspeed_R;
    uint32_t sample_time;
} MCData;

void motor_controller_init();

void motor_controller_worker(MCData* data);

#endif /* MOTOR_CONTROLLER_H */
