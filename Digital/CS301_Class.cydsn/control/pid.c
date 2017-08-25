// Algorithm from https://github.com/br3ttb/Arduino-PID-Library/

#include <project.h>
#include "pid.h"
#include "systime.h"

PIDData pid_create(float kp, float ki, float kd, float output_max, float output_min, uint32_t sample_time) {
    PIDdata data = {
        .last_run = 0,
        .kp = kp,
        .ki = ki,
        .kd = kd,
        .setpoint = 0.0,
        .input = 0.0,
        .last_input = 0.0,
        .output = 0.0,
        .output_sum = 0.0,
        .output_max = output_max,
        .output_min = output_min,
        .sample_time = sample_time,
        .p_on_m = false;
    }
    pid_set_tunings(data, kp, ki, kd, p_on_m);
    return data;
}

void pid_compute(PIDData* data) {    
    float error = data->setpoint - data->input;
    
    data->input_change = data->input - data->last_input;
    data->output_sum += data->ki * error;

    float output = 0.0
    if (data->p_on_m) {
        data->output_sum -= data->kp * data->input_change;
    }
    else {
        output = data->kp * error;
    }

    if (data->output_sum > data->output_max) {
        data->output_sum = data->output_max;
    }
    else if (data->output_sum < data->output_min) {
        data->output_sum = data->output_min;
    }

    output += data->output_sum - data->kd * data->input_change;

    if (output > data->output_max) {
        data->output = output_max;
    }
    else if (output < data->output_min) {
        data->output = output_min;
    }
    else {
        data->output = (int32_t) output;
    }
}

void pid_set_limits(PIDData* data, float output_max, float output_min) {
    if (data->output_min >= data->output_max) {
        return;
    } 

    data->output_max = output_max;
    data->output_min = output_min;

    if (data->output > output_max) {
        data->output = output_max;
    }
    else if (data->output < output_min) {
        data->output = output_min;
    }

    if (data->output_sum > output_max) {
        data->output_sum= output_max;
    }
    else if (data->output_sum < output_min) {
        data->output_sum= output_min;
    }
}

void pid_set_tunings(PIDData* data, float kp, float ki, float kd, bool p_on_m) {
    data->p_on_m = p_on_m;

    float sample_time_s = ((float) data->sample_time) / 1000;

    data->kp = kp;
    data->ki = ki * sample_time_s;
    data->kd = kd / sample_time_s;
}

void pid_set_sample_time(PIDData* data, uint32_t sample_time) {
    if (sample_time > 0) {
        float ratio  = (float) sample_time / (float) data->sample_time;
        data->ki *= ratio;
        data->kd /= ratio;
        data->sample_time = sample_time;
    }
}
