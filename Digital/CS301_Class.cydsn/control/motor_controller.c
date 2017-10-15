#include <project.h>
//#include <stdio.h>
#include <math.h>
#include "motor_controller.h"
#include "systime.h"
#include "motor.h"
#include "interactive.h"

#define LINE(x) data->sc_data->line_state[x]

// pulse/ms -> % of max
static float speed2pidin (float speed) {
    float pidin = speed / MOTOR_MAX_SPEED;
    return pidin;
}

static float calc_setpoint(int32_t target, int32_t now, float speed, float bias) {
    if (target > 0) {
        if (now < target) {
//            if (target - now > 200) {
//                return 0.3f* (speed * bias);
//            }
//            else {
            return speed * (1 + bias);
//            }
        }
    }
    else {
        if (now > target) {
//            if (target - now < -200) {
//                return 0.3f* (- speed * bias);
//            }
//            else {
            return speed * (-1 - bias);
//            }
        }
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
        MOTOR_MAX_SPEED, -MOTOR_MAX_SPEED, dead_band, 30, true),
        .PID_R = pid_create(1.0f, 10.0f, 0.025f,
        MOTOR_MAX_SPEED, -MOTOR_MAX_SPEED, dead_band, 30, true),
        .target_dist = {
            .L = 0,
            .R = 0
        },
        .drive_mode = 0,
        .last_run = 0
    };
    return data;
}

static void adjust_bias(MCData* data) {
    // Run bias calculations
    data->bias_L = 0.0f;
    data->bias_R = 0.0f;
    if (data->drive_mode == 0 || data->drive_mode == 3) {
        if (data->sc_data->use_line) {
            if(data->sc_data->wait_direction == 0 && data->sc_data->u_turn == 0) {
                REG_LED_Write(data->sc_data->line_tracking & data->sc_data->line_intersection << 2);
                switch(data->sc_data->line_tracking) {
                case DI_L:
                    if (data->sc_data->line_intersection) {
                        data->bias_L += -1.7f;
                        data->bias_R += -0.4f;
                        led_set(111);
                    }
                    else {
                        data->bias_L += -0.45f;
                        data->bias_R += 0.0f;
                        led_set(001);
                    }
                    break;
                case DI_R:
                    if (data->sc_data->line_intersection) {
                        data->bias_L += -0.4f;
                        data->bias_R += -1.7f;
                        led_set(111);
                    }
                    else {
                        data->bias_L += 0.0f;
                        data->bias_R += -0.15f;
                        led_set(001);
                    }
                    break;
                default:
                    break;
                }
                //float inversion_bias = -0.02 * data->sc_data->line_inversions;
                //data->bias_L += inversion_bias;
                //data->bias_R += inversion_bias;
        
            }
            else if (data->sc_data->u_turn == 1) {
                data->bias_L += -1.5f;
                data->bias_R += -0.5f;
            }
            else {
                data->bias_L += -1.0f;
                data->bias_R += -1.0f; 
                led_set(0b11);
            }
        }
        if (data->sc_data->use_wireless) {
            if (data->sc_data->rel_orientation < ORIENTATION_HREV) {
                data->bias_R += (float) data->sc_data->rel_orientation / 900;
                data->bias_L += (float) -data->sc_data->rel_orientation / 900;
            }
            else {
                data->bias_L += (float) (data->sc_data->rel_orientation - ORIENTATION_HREV) / 900;
                data->bias_R += (float) -(data->sc_data->rel_orientation - ORIENTATION_HREV) / 900;
            }
        }

        if (data->bias_L == 0 && data->bias_R == 0) {
            // Quad Dec Differential P Bias
            data->bias_L += -0.0 * data->sc_data->qd_differential;
            data->bias_R += 0.0 * data->sc_data->qd_differential;
        }
    }

    //data->bias_L = apply_limit(data->bias_L, -2.0f, 1.0f);
    //data->bias_R = apply_limit(data->bias_R, -2.0f, 1.0f);
}

static void adjust_setpoint(MCData* data) {
    bool special = false;

    // Run setpoint calculations
    if (data->drive_mode == 0) {
        if (data->sc_data->use_wireless) {
            data->PID_L.setpoint = calc_setpoint(data->target_dist.L, data->sc_data->rel_dist, data->target_speed, data->bias_L);
            data->PID_R.setpoint = calc_setpoint(data->target_dist.R, data->sc_data->rel_dist, data->target_speed, data->bias_R);
            special = true;
        }
    }
    else if (data->drive_mode == 1) {
        if (data->sc_data->use_line) {
            if (!data->sc_data->line_front_lost) {
                data->PID_L.setpoint = 0.0f;
                data->PID_L.setpoint = 0.0f;
                special = true;
            }
        }
    }
    else if (data->drive_mode == 3) {
        if (data->sc_data->use_line) {
            if (data->sc_data->line_end && data->sc_data->line_intersection == 0) {
                data->PID_L.setpoint = 0.0f;
                data->PID_L.setpoint = 0.0f;
                special = true;
            }
        }
    }

    if (!special) {
        data->PID_L.setpoint = calc_setpoint(data->target_dist.L, data->sc_data->qd_dist.L, data->target_speed, data->bias_L);
        data->PID_R.setpoint = calc_setpoint(data->target_dist.R, data->sc_data->qd_dist.R, data->target_speed, data->bias_R);
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
        
        if(data->sc_data->wait_direction == 1) {
            motor_controller_set(data, 0.0f,  0, 0xEFFFFFF);
        }

        adjust_setpoint(data);

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

void motor_controller_set(MCData* data, float speed, uint8_t drive_mode, int32_t arg) {
    data->target_speed = speed;
    data->drive_mode = drive_mode;
    if (drive_mode == 0) {
        // Forward/Back
        data->target_dist.L = dist2dec(arg);
        data->target_dist.R = dist2dec(arg);
    }
    else if (drive_mode == 1) {
        // Point turn left/right
        int32_t arc_length = M_PI * WHEEL_DISTANCE * ((float) arg / 360);
        data->target_dist.L = dist2dec(arc_length);
        data->target_dist.R = dist2dec(-arc_length);
    }
    else if (drive_mode == 2) {
        // Arc turn left/right
        int32_t arc_length = M_PI * 2.0f * WHEEL_DISTANCE * ((float) arg / 360);
        if (arc_length < 0.0f) {
            data->target_dist.L = dist2dec(arc_length);
            data->target_dist.R = 0;
        }
        else {
            data->target_dist.L = 0;
            data->target_dist.R = dist2dec(arc_length);
        }
    }
    else if (drive_mode == 3) {
        // Automatic
        data->target_dist.L = 0x0FFFFFFF;
        data->target_dist.R = 0x0FFFFFFF;
    }
}
