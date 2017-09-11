#include <project.h>
#include <stdio.h>
#include <math.h>
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

static float calc_setpoint(int32_t target, int32_t now) {
    if (target > 0) {
        if (now < target) {
            return 0.7f;
        }
    }
    else {
        if (now > target) {
            return -0.7f;
        }
    }

    return 0f;
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
    float dead_band = (float) M_MIN / (float) M_MAX;
    MCData data = {
        .sample_time = 50,
        .qd_dist = {
            .L = 0,
            .R = 0
        },
        .PID_L = pid_create(0.5f, 0.50f, 0.025f, 
            MOTOR_MAX_SPEED, -MOTOR_MAX_SPEED, dead_band, 50, false), // kp_crit = 2
        .PID_R = pid_create(0.5f, 0.50f, 0.025f, 
            MOTOR_MAX_SPEED, -MOTOR_MAX_SPEED, dead_band, 50, false), // set 2 1.8, 0, 5.0
        .target = {
            .L = 0,
            .R = 0
        },
        .last_run = 0,
        .auto = false
    };
    return data;
}

void motor_controller_worker(MCData* data) {
    uint32_t now = systime_ms();
    uint32_t time_diff = now - data->last_run;
    static uint32_t debug = 0;

    if (time_diff >= data->sample_time) {
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

        // Run setpoint calculations
        data->PID_L.setpoint = calc_setpoint(data->target.L, qd.L);
        data->PID_R.setpoint = calc_setpoint(data->target.R, qd.R);

        int8_t mspeedL, mspeedR = 0;
        // Run PID algorithm
        pid_compute(&(data->PID_L));
        pid_compute(&(data->PID_R));
        mspeedL = (int8_t) (data->PID_L.output * (float) M_MAX);
        mspeedR = (int8_t) (data->PID_R.output * (float) M_MAX);

        motor_set_L(mspeedL);
        motor_set_R(mspeedR);

        data->last_run = now;
    }
}

void motor_controller_reset(MCData* data) {
    data->target.L = 0;
    data->target.R = 0;
    data->qd_dist.L = 0;
    data->qd_dist.R = 0;
    quad_dec_clear();
}

void motor_controller_set(MCData* data, uint8_t drive_mode, int32_t arg) {
    motor_controller_reset();

    if (drive_mode == 0) {
        // Forward/Back
        data->target.L = dist2dec(arg);
        data->target.R = dist2dec(arg);
    }
    else if (drive_mode == 1) {
        // Point turn left/right
        int32_t arc_length = 2f * M_PI * (WHEEL_DISTANCE / 2) * ((float) arg / 360f)
        data->target.L = dist2dec(arc_length);
        data->target.R = dist2dec(-arc_length);
    }
    else if (drive_mode == 2) {
        // Arc turn left/right
        int32_t arc_length = 2f * M_PI * WHEEL_DISTANCE * ((float) arg / 360f)
        if (arc_length < 0f) {
            data->target.L = dist2dec(arc_length);
            data->target.R = 0;
        }
        else {
            data->target.L = 0;
            data->target.R = dist2dec(arc_length);
        }
    }
}
