#ifndef MOTOR_CONTROLLER_H
#define MOTOR_CONTROLLER_H

#include <cytypes.h>
#include <stdbool.h>
    
#include "quad_dec.h"
#include "pid.h"

#define M_PI 3.14159265358979323846
#define MOTOR_MAX_SPEED 1 // encoder pulses per millisecond
#define WHEEL_CIRCUMFERENCE 204 // millimeters
#define PULSES_PER_REV 228 // Decoder x4
#define WHEEL_DISTANCE 114 // millimeter

typedef struct {
    uint32_t sample_time;
    QuadDecData qd_dist;
    float bias_L;
    float bias_R;
    PIDData PID_L;
    PIDData PID_R;
    QuadDecData target;
    uint32_t last_run;
    bool automatic;
} MCData;

void motor_controller_init();

double motor_controller_measure_max_speed();

MCData motor_controller_create();

void motor_controller_worker(MCData* data);

void motor_controller_reset(MCData* data);

void motor_controller_set(MCData* data, uint8_t drive_mode, int32_t arg);

#endif /* MOTOR_CONTROLLER_H */
