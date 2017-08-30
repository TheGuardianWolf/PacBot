#ifndef MOTOR_CONTROLLER_H
#define MOTOR_CONTROLLER_H

#include <cytypes.h>
#include <stdbool.h>
    
#include "quad_dec.h"
#include "pid.h"

#define MOTOR_MAX_SPEED 0.8 // encoder pulses per millisecond
#define WHEEL_CIRCUMFERENCE 204 // millimeters
#define PULSES_PER_REV 228 // Decoder x4

typedef struct {
    uint32_t sample_time;
    QuadDecData qd_dist;
    PIDData PID_L;
    PIDData PID_R;
    QuadDecData target;
    uint32_t last_run;
} MCData;

void motor_controller_init();

double motor_controller_measure_max_speed();

MCData motor_controller_create();

void motor_controller_worker(MCData* data);

void motor_controller_set(MCData* data, int32_t t_dist_L, int32_t t_dist_R);

void motor_controller_run_forward(MCData* data, int32_t t_dist_L, int32_t t_dist_R);

#endif /* MOTOR_CONTROLLER_H */
