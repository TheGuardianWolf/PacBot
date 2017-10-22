#ifndef MOTOR_CONTROLLER_H
#define MOTOR_CONTROLLER_H

#include <cytypes.h>
#include <stdbool.h>

#include "sensors_controller.h"
#include "pid.h"

//#define M_PI 3.14159265358979323846
#define MOTOR_MAX_SPEED 1 // encoder pulses per millisecond
#define WHEEL_DISTANCE 114 // millimeter

typedef struct {
    uint32_t sample_time;
    SCData* sc_data;
    float target_speed; 
    float bias_L;
    float bias_R;
    PIDData PID_L;
    PIDData PID_R;
    PIDData PID_LR;
    QuadDecData target_dist;
    QuadDecData ref_dist;
    int32_t line_stop_tolerance;
    int32_t line_turn_tolerance;
    int8_t drive_mode;
    bool idle;
    uint32_t last_run;
} MCData;

typedef struct {
    int8_t drive_mode;
    int32_t arg;
    float speed;
} MotorCommand;

void motor_controller_init();

double motor_controller_measure_max_speed();

MCData motor_controller_create(uint32_t sample_time, SCData *sc_data);

void motor_controller_worker(MCData* data);

void motor_controller_set(MCData* data, MotorCommand* cmd);

#endif /* MOTOR_CONTROLLER_H */
