#include <project.h>
#include <math.h>

#include "sensors_controller.h"
#include "systime.h"
#include "adc.h"

#define LINE(x) data->line_state[x]

// (s1-s0)/dt -> pulse/ms
static float calc_speed(int32_t curr, int32_t prev, uint32_t dt) {
    return (float) (curr - prev) / (float) dt;
}

void sensors_controller_init() {
    systime_init();
    wireless_init();
    sensors_init();
    adc_init();
}

SCData sensors_controller_create(uint32_t sample_time, bool use_wireless, bool use_line) {
    SCData data = {
        .sample_time = sample_time,
        .use_wireless = use_wireless,
        .use_line = use_line,
        .line_state = {true, false, false, false, false, true},
        .prev_intersection = 0,
        .curr_intersection = 0,
        .line_end = false,
        .curve = 0,
        .line_inversions = 0,
        .loc_valid = false,
        .curr_speed_L = 0.0f,
        .curr_speed_R = 0.0f,
        .last_run = 0
    };

    if (use_wireless) {
        RFData rf_data = wireless_get();
        data.loc_valid = (now - rf_data.timestamp < data.sample_time);
        while(!data.loc_valid) {
            rf_data = wireless_get();
            data.loc_valid = (now - rf_data.timestamp < data.sample_time);
        }
    }

    sensors_controller_reset(data);
    return data;
}

void sensors_controller_worker(SCData* data) {
    uint32_t now = systime_ms();
    uint32_t time_diff = now - data->last_run;

    if (time_diff >= data->sample_time) {
        QuadDecData qd_data = quad_dec_get();
        RFData rf_data;
        if (data->use_wireless) {
             rf_data = wireless_get();
        }
        uint8_t line_data;
        if (data->use_line) {
             line_data = sensors_line_get();
        }

        // QD section
        data->qd_prev = data->qd_dist;
        data->qd_dist.L = qd_data.L - data->qd_start.L;
        data->qd_dist.R = qd_data.R - data->qd_start.R;

        data->curr_speed_L = calc_speed(data->qd_dist.L, data->qd_prev.L, time_diff);
        data->curr_speed_R = calc_speed(data->qd_dist.R, data->qd_prev.R, time_diff);        

        // Wireless fusion section
        if (data->use_wireless) {
            data->loc_valid = (now - rf_data.timestamp < data->sample_time);

            data->curr_loc.x = rf_data.robot_xpos;
            data->curr_loc.y = rf_data.robot_ypos;
            data->curr_loc.orientation = rf_data.robot_orientation;
            data->rel_orientation = rf_data.robot_orientation - data->start_loc.orientation;
            if (data->rel_orientation < 0) {
                data->rel_loc.orientation += ORIENTATION_REV;
            }

            if (data->loc_valid) {
                data->rel_dist = dist2dec((int32)sqrtf(powf(rf_data.robot_xpos - data->start_loc.x, 2) + powf(rf_data.robot_ypos - data->start_loc.y, 2)));
            }
            else {
                data->rel_dist += (data->qd_dist.L - data->qd_prev.L) + (data->qd_dist.R - data->qd_prev.R)) / 2
            }
        }
        
        // Line section
        if (data->use_line) {
            uint8_t i;
            data->line_inversions = 0;
            for (i = 0; i < LINE_SENSORS; i++) {
                data->line_state[i] = (bool) ((line_data >> i) & 1);
                if (!line_state[i]) {
                    data->line_inversions++;
                } 
            }

            if (LINE(1)) {
                int8_t intersection = (int8_t) !LINE(2) + (int8_t) !LINE(3) * 2;
                if (intersection > 0) {
                    data->prev_intersection = data->sc_data->curr_intersection;
                    data->curr_intersection = intersection;
                }
                
                if (!LINE(0)) {
                    data->curve = (int8_t) !LINE(4) + ((int8_t) !LINE(5)) * 2;
                    if (LINE(4) && LINE(5)) {
                        data->line_end = true;
                    }
                }
            }
        }   
    }
}

void sensors_controller_reset(SCData* data) {
    data->last_run = systime_ms();
    RFData rf_data = wireless_get();
    QuadDecData qd_data = quad_dec_get();
    data->start_loc.x = rf_data.robot_xpos;
    data->start_loc.y = rf_data.robot_ypos;
    data->start_loc.orientation = rf_data.robot_orientation;
    data->curr_loc = data->start_loc; 
    data->qd_start = qd_data;
    data->qd_dist.L = 0;
    data->qd_dist.R = 0;
    data->last_qd_loc_valid = qd_data;
}
