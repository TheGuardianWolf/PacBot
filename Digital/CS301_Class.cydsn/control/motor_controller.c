#include <project.h>

#include "motor_controller.h"
#include "systime.h"
#include "motor.h"
#include "quad_dec.h"
#include "pid.h"

// mm -> pulse
static int32_t dist2dec(float dist) {
    // Units: mm
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
static float calc_setpoint_L() {
    return 0.2;
}

static float calc_setpoint_R() {
    return 0.2;
}

void motor_controller_init() {
    systime_init();
    motor_init();
    quad_dec_init();
}

MCData motor_controller_create() {
    MCData data = {
        .sample_time = 50,
        .qd_dist = {
            .L = 0,
            .R = 0
        }
        .PID_L = pid_create(0.1, 0.1, 0.1, 0.2, 0, 100),
        .PID_R = pid_create(0.1, 0.1, 0.1, 0.2, 0, 100),
        .target = {
            .L = 0,
            .R = 0
        }
        .last_run = 0
    }
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

void motor_controller_run_forward(MCData* data, float t_dist_L, float t_dist_R) {
    data->target.L = dist2dec(t_dist_L);
    data->target.R = dist2dec(t_dist_R);
    data->PID_L.setpoint = calc_setpoint_L();
    data->PID_R.setpoint = calc_setpoint_R();
    data->qd_dist.L = 0;
    data->qd_dist.R = 0;
    quad_dec_clear();

    if (data->qd_dist.L < data->target.L || data->qd_dist.R < data->target.R) {
        if (data->qd_dist.L >= data->target.L) {
            data->PID_L.setpoint = 0;
        }

        if (data->qd_dist.R >= data->target.R) {
            data->PID_R.setpoint = 0;
        }

        motor_controller_worker(data);
    }
}
