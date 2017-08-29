#include <project.h>

#include "motor_controller.h"
#include "systime.h"
#include "motor.h"

// mm -> pulse
static int32_t dist2dec(int32_t dist) {
    return (int32_t) ((float) dist * PULSES_PER_REV / WHEEL_CIRCUMFERENCE);
}

// pulse/ms -> % of max
static float speed2pidin (float speed) {
    float pidin = speed / MOTOR_MAX_SPEED;

    if (pidin > 1) {
        pidin = 1;
    }
    else if (pidin < -1) {
        pidin = -1;
    }

    return pidin;
}

// (s1-s0)/dt -> pulse/ms
static float calc_speed(int32_t prev, int32_t curr, uint32_t dt) {
    return (float) (curr - prev) / dt;
}

// These need to be dynamically calculated based on the sensors controller
static float calc_setpoint_L(bool reverse) {
    float setpoint = 0.95; // Run at 95% max speed
    if (reverse) {
        setpoint = -setpoint;
    }
    return setpoint;
}

static float calc_setpoint_R(bool reverse) {
    float setpoint = 0.95;
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
        .sample_time = 25,
        .qd_dist = {
            .L = 0,
            .R = 0
        },
        // Don't set any of the .95s to 1, it bugs for some reason
        .PID_L = pid_create(0.5, 1, 0.001, 0.95, -0.95, 50),
        .PID_R = pid_create(0.5, 1, 0.001, 0.95, -0.95, 50),
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

    if (time_diff >= data->sample_time) {
        int8_t mspeedL, mspeedR = 0;

        QuadDecData qd = quad_dec_get();
        data->PID_L.input = speed2pidin(calc_speed(qd.L, data->qd_dist.L, time_diff));
        data->PID_R.input = speed2pidin(calc_speed(qd.R, data->qd_dist.R, time_diff));
        data->qd_dist = qd;
        
        if (data->target.L != 0) {
            pid_worker(&(data->PID_L));
            mspeedL = (int8_t)(data->PID_L.output * M_MAX);
        }
        if (data->target.R != 0) {
            pid_worker(&(data->PID_R));
            mspeedR = (int8_t)(data->PID_R.output * M_MAX);     
        }

        motor_set_L(mspeedL);
        motor_set_R(mspeedR);
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

    bool disable_L = false;
    bool disable_R = false;
    while (true) {//!disable_L || !disable_R) {
        if (data->qd_dist.L >= data->target.L) {
            data->PID_L.setpoint = 0;
            disable_L = true;
        }

        if (data->qd_dist.R >= data->target.R) {
            data->PID_R.setpoint = 0;
            disable_R = true;
        }

        motor_controller_worker(data);
    }
}
