// Algorithm from https://github.com/br3ttb/Arduino-PID-Library/
// Friction and backlash compensation theory from http://www.wescottdesign.com/articles/Friction/friction.pdf
// Modifications by Jerry Fan

#include <project.h>
#include "pid.h"
#include "systime.h"

float apply_limit(float var, float min, float max) {
    if (var > max) {
        return max;
    }
    else if (var < min) {
        return min;
    }
    return var;
}

PIDData pid_create(float kp, float ki, float kd, float output_max, float output_min, float dead_band, uint32_t sample_time, bool p_on_m) {
    PIDData data = {
        .last_run = 0,
        .kp = kp,
        .ki = ki,
        .kd = kd,
        .setpoint = 0.0f,
        .input = 0.0f,
        .last_input = 0.0f,
        .output = 0.0f,
        .output_sum = 0.0f,
        .output_max = output_max,
        .output_min = output_min,
        .dead_band = dead_band,
        .sample_time = sample_time,
        .p_on_m = false,
        .active = true
    };
    pid_set_tunings(&data, kp, ki, kd, p_on_m);
    return data;
}

void pid_worker(PIDData* data) {  
    uint32_t now = systime_ms();
    if (now - data->last_run >= data->sample_time) {
        pid_compute(data);
        data->last_run = now;
    }
}

void pid_compute(PIDData* data) {
    // If not active, then ignore rest of function
    if (!data->active) {
        data->output = data->setpoint;
        return;
    }

    // Calculate error variables
    float error = data->setpoint - data->input;
    float input_change = data->input - data->last_input;
    data->output_sum += data->ki * error;

    // Motor control PID requires summation of input and output
    float output = data->input;
    
    // If P on M enabled, calculate
    if (data->p_on_m) {
        data->output_sum -= data->kp * input_change;
    }
    else {
        output += data->kp * error;
    }

    data->output_sum = apply_limit(data->output_sum, data->output_min, data->output_max);

    output += data->output_sum;

    // Temporaily disable override - not providing low speeds.
    // // Override PID when output falls into dead band
    // if (data->output < data->dead_band && data->output > -data->dead_band) {
    //     output = 0.0f;
    //     if (data->setpoint > 0.0f) {
    //         if (data->setpoint > data->output) {
    //             output = data->dead_band;
    //         }
    //     }
    //     else if (data->setpoint < 0.0f) {
    //         if (data->setpoint < data->output) {
    //             output = -data->dead_band;
    //         }
    //     } 
    // }
    
    // Always apply D effects
    output += - data->kd * input_change;

    data->last_input = data->input;

    data->output = apply_limit(output, data->output_min, data->output_max);
}

void pid_set_active(PIDData* data, bool active) {
    if(active && !data->active) {
        data->output_sum = apply_limit(data->output, data->output_min, data->output_max);
        data->last_input = data->input;
    }
   data->active = active;
}

void pid_set_limits(PIDData* data, float output_max, float output_min) {
    if (data->output_min >= data->output_max) {
        return;
    } 

    data->output_max = output_max;
    data->output_min = output_min;

    data->output = apply_limit(data->output, output_min, output_max);

    data->output_sum = apply_limit(data->output_sum, output_min, output_max);
}

void pid_set_tunings(PIDData* data, float kp, float ki, float kd, bool p_on_m) {
    data->p_on_m = p_on_m;

    float sample_time_s = ((float) data->sample_time) / 1000.0f;

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
