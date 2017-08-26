#ifndef MOTOR_CONTROLLER_H
#define MOTOR_CONTROLLER_H

#include <cytypes.h>
#include <stdbool.h>

#define MOTOR_MAX_SPEED // encoder pulses per millisecond

typedef struct {
    uint32_t sample_time;
    QuadDecData qd_dist;
    PIDData PID_L;
    PIDData PID_R;
    QuadDecData target;
    uint32_t last_run;
} MCData;

void motor_controller_init();

void motor_controller_worker(MCData* data);

void motor_controller_run_forward(MCData* data, float t_dist_L, float t_dist_R);

#endif /* MOTOR_CONTROLLER_H */
