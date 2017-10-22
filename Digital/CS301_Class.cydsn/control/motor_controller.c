#include <project.h>
#include <stdlib.h>
#include <math.h>
#include "motor_controller.h"
#include "systime.h"
#include "motor.h"
#include "interactive.h"

#define USE_DIFFERENTIAL 0
#define LINE(x) data->sc_data->line_state[x]
#define deg2dist(x)  M_PI * WHEEL_DISTANCE * ((float) x / 360)

// pulse/ms -> % of max
static float speed2pidin (float speed) {
    float pidin = speed / MOTOR_MAX_SPEED;
    return pidin;
}

static float calc_setpoint(int32_t target, int32_t now, int32_t ref, float speed, float bias) {
    int32_t difference = abs(target - ref) - abs(now - ref);
    int32_t sign = (int32_t) copysignf(1.0f, (float) target-ref); //(int32_t) copysignf(1.0f, (float) difference);
    if (difference > 0) {
        if (difference < 50) {
            return sign * ((float) (0.9f/100)*difference + 0.1f) * speed * (1 + bias);
        }
        return sign * speed * (1 + bias);
    }
    return 0.0f;
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

MCData motor_controller_create(uint32_t sample_time, SCData *sc_data) {
    float dead_band = (float) M_MIN / (float) M_MAX;
    MCData data = {
        .sample_time = sample_time,
        .sc_data = sc_data,
        .target_speed = 0.8,
        .bias_L = 0.0f,
        .bias_R = 0.0f,
        .PID_L = pid_create(1.0f, 10.0f, 0.025f,
        MOTOR_MAX_SPEED, -MOTOR_MAX_SPEED, dead_band, sample_time, true),
        .PID_R = pid_create(1.0f, 10.0f, 0.025f,
        MOTOR_MAX_SPEED, -MOTOR_MAX_SPEED, dead_band, sample_time, true),
        .target_dist = {
            .L = 0,
            .R = 0
        },
        .ref_dist = {
            .L = 0,
            .R = 0
        },
        .line_stop_tolerance = 100,
        .line_turn_tolerance = deg2dist(75),
        .drive_mode = 0,
        .idle = false,
        .last_run = 0
    };
    return data;
}

static void adjust_bias(MCData* data) {
    // Run bias calculations
    data->bias_L = 0.0f;
    data->bias_R = 0.0f;

    if (data->sc_data->use_line) {
        int32_t tolerance = 100;
        QuadDecData dist_to_target = {
            .L = data->target_dist.L - data->sc_data->qd_dist.L,
            .R = data->target_dist.R - data->sc_data->qd_dist.R
        };
        if (abs(dist_to_target.L) > tolerance && abs(dist_to_target.R) > tolerance) {
            switch(data->sc_data->line_tracking) {
            case DI_L:
                data->bias_L += -0.7f;
                data->bias_R += -0.5f;
                break;
            case DI_R:
                data->bias_L += -0.5f;
                data->bias_R += -0.7f;
                break;
            default:
                break;
            }
        }
//        float inversion_bias = -0.02 * data->sc_data->line_inversions;
//        data->bias_L += inversion_bias;
//        data->bias_R += inversion_bias;
    }
    // if (data->sc_data->use_wireless) {
    //     if (data->sc_data->rel_orientation < ORIENTATION_HREV) {
    //         data->bias_R += (float) data->sc_data->rel_orientation / 900;
    //         data->bias_L += (float) -data->sc_data->rel_orientation / 900;
    //     }
    //     else {
    //         data->bias_L += (float) (data->sc_data->rel_orientation - ORIENTATION_HREV) / 900;
    //         data->bias_R += (float) -(data->sc_data->rel_orientation - ORIENTATION_HREV) / 900;
    //     }
    // }

#if USE_DIFFERENTIAL == 1
    if (data->drive_mode == 0) {
        int32_t qd_differential = (data->sc_data->qd_dist.L - data->ref_dist.L) - (data->sc_data->qd_dist.R - data->ref_dist.R);
        if (data->bias_L == 0 && data->bias_R == 0) {
            // Quad Dec Differential P Bias
            data->bias_L += -0.01 * qd_differential;
            data->bias_R += 0.01 * qd_differential;
        }
    }
#endif

    data->bias_L = apply_limit(data->bias_L, -2.0f, 1.0f);
    data->bias_R = apply_limit(data->bias_R, -2.0f, 1.0f);
}

static void adjust_setpoint(MCData* data) {
    bool special = false;
    int32_t tolerance;
    // Run setpoint calculations
    if (data->drive_mode == 0) {
        if (data->sc_data->use_line) {
            tolerance = data->line_stop_tolerance;
            QuadDecData dist_to_target = {
                .L = data->target_dist.L - data->sc_data->qd_dist.L,
                .R = data->target_dist.R - data->sc_data->qd_dist.R
            };

            if (abs(dist_to_target.L) < tolerance && abs(dist_to_target.R) < tolerance) {
                sensors_controller_set_config(data->sc_data, LINE_INTERSECTION_CONFIG);
                if((data->sc_data->line_end || data->sc_data->line_intersection[0] > 0)) {
                    data->PID_L.setpoint = 0.0f;
                    data->PID_R.setpoint = 0.0f;
                    data->target_dist.L = data->sc_data->qd_dist.L;
                    data->target_dist.R = data->sc_data->qd_dist.R;
                    special = true;
                }
            }
            else {
                sensors_controller_set_config(data->sc_data, LINE_TRACKING_CONFIG);
            }
        }
    }
    else if (data->drive_mode == 1) {
        if (data->sc_data->use_line) {
            tolerance = data->line_turn_tolerance;
            QuadDecData dist_to_target = {
                .L = data->target_dist.L - data->sc_data->qd_dist.L,
                .R = data->target_dist.R - data->sc_data->qd_dist.R
            };
            if (abs(dist_to_target.L) < tolerance && abs(dist_to_target.R) < tolerance) {
                sensors_controller_set_config(data->sc_data, LINE_TRACKING_CONFIG);
                if (!data->sc_data->line_front_lost) {
                    data->PID_L.setpoint = 0.0f;
                    data->PID_R.setpoint = 0.0f;
                    data->target_dist.L = data->sc_data->qd_dist.L;
                    data->target_dist.R = data->sc_data->qd_dist.R;
                    special = true;
                }
            }
            sensors_controller_set_config(data->sc_data, LINE_DISABLE_CONFIG);
        }
    }
    else if (data->drive_mode == 3) {
        if (data->sc_data->use_line && (data->sc_data->line_end || data->sc_data->line_intersection[0] > 0)) {
            data->PID_L.setpoint = 0.0f;
            data->PID_R.setpoint = 0.0f;
            data->target_dist.L = data->sc_data->qd_dist.L;
            data->target_dist.R = data->sc_data->qd_dist.R;
            special = true;
        }
        else if (data->sc_data->use_wireless) {
            data->PID_L.setpoint = calc_setpoint(data->target_dist.L, data->sc_data->rel_dist, data->ref_dist.L, data->target_speed, data->bias_L);
            data->PID_R.setpoint = calc_setpoint(data->target_dist.R, data->sc_data->rel_dist, data->ref_dist.R, data->target_speed, data->bias_R);
            special = true;
        }
    }

    if (data->drive_mode >= 0 && !special) {
        data->PID_L.setpoint = calc_setpoint(data->target_dist.L, data->sc_data->qd_dist.L, data->ref_dist.L, data->target_speed, data->bias_L);
        data->PID_R.setpoint = calc_setpoint(data->target_dist.R, data->sc_data->qd_dist.R, data->ref_dist.R, data->target_speed, data->bias_R);
    }
}


void motor_controller_worker(MCData* data) {
    uint32_t now = systime_ms();
    uint32_t time_diff = now - data->last_run;
    if (time_diff >= data->sample_time) {
        data->last_run = now;
        data->PID_L.input = speed2pidin(data->sc_data->curr_speed_L);
        data->PID_R.input = speed2pidin(data->sc_data->curr_speed_R);

        adjust_bias(data);

        adjust_setpoint(data);

        if (data->PID_L.setpoint == 0 && data->PID_R.setpoint == 0) {
            data->idle = true;
        }
        else {
            data->idle = false;
        }

        int8_t mspeedL, mspeedR = 0;
        // Run PID algorithm
        pid_compute(&(data->PID_L));
        pid_compute(&(data->PID_R));
        mspeedL = (int8_t) (data->PID_L.output * M_MAX);
        mspeedR = (int8_t) (data->PID_R.output * M_MAX);

        motor_set_L(mspeedL);
        motor_set_R(mspeedR);
    }
}

void motor_controller_set(MCData* data, MotorCommand* cmd) {
    data->idle = false;
    data->target_speed = cmd->speed;
    data->drive_mode = cmd->drive_mode;
    data->ref_dist.L = data->target_dist.L;
    data->ref_dist.R = data->target_dist.R;
    data->sc_data->start_loc.orientation = data->sc_data->curr_loc.orientation;
    if (cmd->drive_mode == -1) {
        data->target_dist.L = data->sc_data->qd_dist.L;
        data->target_dist.R = data->sc_data->qd_dist.R;
    }
    if (cmd->drive_mode == 0) {
        // Forward/Back
        data->target_dist.L += dist2dec(cmd->arg);
        data->target_dist.R += dist2dec(cmd->arg);

        if (data->target_dist.L < data->ref_dist.L) {
            if (!data->sc_data->reversed) {
                sensors_controller_reverse(data->sc_data);
            }
        }
        else {
            if (data->sc_data->reversed) {
                sensors_controller_reverse(data->sc_data);
            }
        }
    }
    else if (cmd->drive_mode == 1) {
        // Point turn left/right
        int32_t arc_length = deg2dist(cmd->arg);
        data->target_dist.L += dist2dec(arc_length);
        data->target_dist.R += dist2dec(-arc_length);

        if (arc_length < 0) {
            data->sc_data->line_herustic_turn = DI_L;
        }
        else if (arc_length > 0) {
            data->sc_data->line_herustic_turn = DI_R;
        }
    }
    else if (cmd->drive_mode == 2) {
        // Arc turn left/right
        int32_t arc_length = 2.0f * deg2dist(cmd->arg);
        if (arc_length < 0.0f) {
            data->target_dist.L += dist2dec(arc_length);
            data->target_dist.R += 0;
        }
        else {
            data->target_dist.L += 0;
            data->target_dist.R += dist2dec(arc_length);
        }
    }
    else if (cmd->drive_mode == 3) {
        // Forward/Back with RF
        data->target_dist.L += dist2dec(cmd->arg);
        data->target_dist.R += dist2dec(cmd->arg);
    }
}
