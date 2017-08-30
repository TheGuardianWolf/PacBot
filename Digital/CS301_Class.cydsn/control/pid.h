#ifndef PID_H
#define PID_H

#include <cytypes.h>
#include <stdbool.h>

typedef struct {
    uint32_t last_run;
    float kp;
    float ki;
    float kd;
    float setpoint;
    float input;
    float last_input;
    float output;
    float output_sum;
    float output_max;
    float output_min;
    uint32_t sample_time;
    bool p_on_m;
    bool active;
}  PIDData;

PIDData pid_create(float kp, float ki, float kd, float output_max, float output_min, uint32_t sample_time);

void pid_worker(PIDData* data);

void pid_compute(PIDData* data);

void pid_set_limits(PIDData* data, float output_max, float output_min);

void pid_set_tunings(PIDData* data, float kp, float ki, float kd, bool p_on_m);

void pid_set_sample_time(PIDData* data, uint32_t sample_time);

#endif /* PID_H */
