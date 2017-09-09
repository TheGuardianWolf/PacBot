#include <project.h>
#include <stdio.h>
#include "motor_controller.h"
#include "systime.h"
#include "motor.h"
#include "usb.h"

// mm -> pulse
static int32_t dist2dec(int32_t dist) {
    return (int32_t) ((float) dist * PULSES_PER_REV / WHEEL_CIRCUMFERENCE);
}

// pulse/ms -> % of max
static float speed2pidin (float speed) {
    float pidin = speed / MOTOR_MAX_SPEED;
    return pidin;
}

// (s1-s0)/dt -> pulse/ms
static float calc_speed(int32_t curr, int32_t prev, uint32_t dt) {
    return (float) (curr - prev) / (float) dt;
}

// These need to be dynamically calculated based on the sensors controller
static float calc_setpoint_L(bool reverse) {
    float setpoint = 1; // Run at 95% max speed
    if (reverse) {
        setpoint = -setpoint;
    }
    return setpoint;
}

static float calc_setpoint_R(bool reverse) {
    float setpoint = 1;
    if (reverse) {
        setpoint = -setpoint;
    }
    return setpoint;
}

void motor_controller_init() {
    systime_init();
    motor_init();
    quad_dec_init();
}

double motor_controller_measure_max_speed() {
    uint32_t curr_time = 0;
    quad_dec_clear();
    uint32_t start_time = systime_ms();
    while (curr_time < start_time + 3000) {
        curr_time = systime_ms();
        motor_set_L(M_MAX);
        motor_set_R(M_MAX);
    }
    motor_set_L(M_DRIFT);
    motor_set_R(M_DRIFT);
    QuadDecData qd = quad_dec_get();
    return (((double) qd.L / (double) (curr_time - start_time)) + ((double) qd.R / (double) (curr_time - start_time))) / 2;
}

MCData motor_controller_create() {
    MCData data = {
        .sample_time = 50,
        .qd_dist = {
            .L = 0,
            .R = 0
        },
        .PID_L = pid_create(0.5, 0.50, 0.025, 1, -1, 50, false), // kp_crit = 2
        .PID_R = pid_create(0.5, 0.50, 0.025, 1, -1, 50, false), // set 2 1.8, 0, 5.0
        .target = {
            .L = 0,
            .R = 0
        },
        .last_run = 0
    };
    return data;
}

void motor_controller_worker(MCData* data) {
    uint32_t now = systime_ms();
    uint32_t time_diff = now - data->last_run;
    static uint32_t debug = 0;

    if (time_diff >= data->sample_time) {
        // Speed calculations
        float mspeedL, mspeedR = 0;

        QuadDecData qd = quad_dec_get();
        data->PID_L.input = speed2pidin(calc_speed(qd.L, data->qd_dist.L, time_diff));
        data->PID_R.input = speed2pidin(calc_speed(qd.R, data->qd_dist.R, time_diff));
        
        
        // if (now - debug >= 500) {
        //     debug = now;
        //     // Debug by output to USB
        //     char buffer[64];
        //     sprintf(buffer, 
        //         "input: %d * 10^-2 output: %d * 10^-2 set: %d * 10^-2\n", 
        //         (int) (data->PID_L.input * 100), 
        //         (int) (data->PID_L.output * 100),
        //         (int) (data->PID_L.setpoint * 100)
        //     );
        //     usb_send_string(buffer);
        //     sprintf(buffer,
        //         "qd.L: %d qd.R: %d qd_dist.L: %d qd_dist.R: %d\n",
        //         (int) qd.L,
        //         (int) qd.R,
        //         (int) data->qd_dist.L,
        //         (int) data->qd_dist.R
        //     );
        //     usb_send_string(buffer);
        // }

        data->qd_dist = qd;
          
        // Calculate desired setpoint
        

        // Run PID algorithm
        if (data->target.L != 0) {
            pid_compute(&(data->PID_L));
            mspeedL = data->PID_L.input + data->PID_L.output;
        }
        if (data->target.R != 0) {
            pid_compute(&(data->PID_R));
            mspeedR = data->PID_R.input + data->PID_R.output;
        }

        if (mspeedL > M_MAX) {
            mspeedL = M_MAX;
        }
        else if (mspeedL < M_MIN) {
            mspeedL = M_MIN;
        }

        if (mspeedR > M_MAX) {
            mspeedR = M_MAX;
        }
        else if (mspeedR < M_MIN) {
            mspeedR = M_MIN;
        }

        motor_set_L((int8_t) (mspeedL * (float) M_MAX));
        motor_set_R((int8_t) (mspeedR * (float) M_MAX));

        data->last_run = now;
    }
}

void motor_controller_set(MCData* data, int32_t t_dist_L, int32_t t_dist_R) {
    data->target.L = dist2dec(t_dist_L);
    data->target.R = dist2dec(t_dist_R);
    data->PID_L.setpoint = calc_setpoint_L(false);
    data->PID_R.setpoint = calc_setpoint_R(false);
    data->qd_dist.L = 0;
    data->qd_dist.R = 0;
    quad_dec_clear();
}

void motor_controller_run_forward(MCData* data, int32_t t_dist_L, int32_t t_dist_R) {
    motor_controller_set(data, t_dist_L, t_dist_R);

    bool left_finished = false, right_finished = false;
    while (!left_finished && !right_finished) {
        if (data->qd_dist.L >= data->target.L) {
            data->PID_L.setpoint = 0;
            left_finished = true;
        }

        if (data->qd_dist.R >= data->target.R) {
            data->PID_R.setpoint = 0;
            right_finished = true;
        }

        motor_controller_worker(data);
    }
    motor_set_L(0);
    motor_set_R(0);
}
