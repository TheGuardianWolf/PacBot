#include <project.h>
#include <math.h>
#include <string.h>

#include "sensors_controller.h"
#include "systime.h"
#include "adc.h"

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
        .prev_intersection = DI_N,
        .curr_intersection = DI_N,
        .line_end = false,
        .line_track = DI_N,
        .line_curve = DI_N,
        .line_inversions = 0,
        .line_lost = false,
        .loc_valid = false,
        .curr_speed_L = 0.0f,
        .curr_speed_R = 0.0f,
        .last_run = 0
    };

    if (use_wireless) {
        uint32_t now = systime_ms();
        RFData rf_data = wireless_get();
        data.loc_valid = (now - rf_data.timestamp < data.sample_time);
        while(!data.loc_valid) {
            now = systime_ms();
            rf_data = wireless_get();
            data.loc_valid = (now - rf_data.timestamp < data.sample_time);
        }
    }

    sensors_controller_reset(&data);
    return data;
}

void sensors_controller_worker(SCData* data) {
    // Time guarded section
    uint32_t now = systime_ms();
    uint32_t time_diff = now - data->last_run;

    if (time_diff >= data->sample_time) {
        data->last_run = now;
        QuadDecData qd_data = quad_dec_get();
        

        // QD section
        data->qd_prev = data->qd_dist;
        data->qd_dist.L = qd_data.L - data->qd_start.L;
        data->qd_dist.R = qd_data.R - data->qd_start.R;

        data->curr_speed_L = calc_speed(data->qd_dist.L, data->qd_prev.L, time_diff);
        data->curr_speed_R = calc_speed(data->qd_dist.R, data->qd_prev.R, time_diff);        
    }
    
    
    // Line section - Guard via interrupt flag
    #define LINE(x) line_data.state[x]
    #define LINE_INV(x) !line_data.state[x]
    if (data->use_line && sensors_line_check()) {
        LineData line_data;
        line_data = sensors_line_get();
        uint8_t i;
        data->line_inversions = 0;
        for (i = 0; i < LINE_SENSORS; i++) {
            if (!LINE(i)) {
                data->line_inversions++;
            } 
        }
        
        // If wing sensors are inverted
        if (LINE_INV(1) || LINE_INV(2)) {
            // If center sensor is not inverted, it's an intersection
            if (LINE(0)) {
                int8_t intersection = (int8_t) LINE_INV(1) * DI_L + (int8_t) LINE_INV(2) * DI_R;
                data->prev_intersection = data->curr_intersection;
                data->curr_intersection = intersection;
            }
            // Otherwise, it's a curve
            else {
                data->line_curve = (int8_t) LINE_INV(1) * DI_L + (int8_t) LINE_INV(2) * DI_R;
            }
        }
        // Check if lost or line has ended
        else if ((LINE_INV(0) && LINE(1) && LINE(2) && LINE(3) && LINE(4))) {
            if (LINE(5) && data->line_curve == 0) {
                data->line_end = true;
                data->line_tracking = true;
            }
            else if (data->line_inversions == 6) {
                data->line_lost = true;
            }
            else {
                data->line_end = false;
                data->line_lost = false;
            }
        }
        else {
            data->line_end = false;
            data->line_lost = false;
        }
        
        // Use tail to restore normality whilst curving
        if (data->line_curve > 0) {
            
            if (LINE(5)) {
                data->line_curve = DI_N;
            }
            else {
                data->line_tracking = true;
            }
        }

        // Use feelers for line tracking
        if (LINE_INV(5)) {
            data->line_track = (int8_t) LINE_INV(3) * DI_L + (int8_t) LINE_INV(4) * DI_R;
        }
            
        // Whilst tracking, use tail and center to restore normality            
        if (data->line_track > 0) {
            if (LINE(5) && LINE(0)) {
                data->line_track = DI_N;
                data->line_tracking = false;
            }
        }
    }
    
    // Wireless fusion section - Guard via interrupt flag
    if (data->use_wireless && wireless_check()) {
        RFData rf_data = wireless_get();
        data->loc_valid = (now - rf_data.timestamp < data->sample_time);

        data->curr_loc.x = rf_data.robot_xpos;
        data->curr_loc.y = rf_data.robot_ypos;
        data->curr_loc.orientation = rf_data.robot_orientation;
        data->rel_orientation = rf_data.robot_orientation - data->start_loc.orientation;
        if (data->rel_orientation < 0) {
            data->rel_orientation += ORIENTATION_REV;
        }

        if (data->loc_valid) {
            data->rel_dist = dist2dec((int32)sqrtf(powf(rf_data.robot_xpos - data->start_loc.x, 2) + powf(rf_data.robot_ypos - data->start_loc.y, 2)));
        }
        else {
            data->rel_dist += ((data->qd_dist.L - data->qd_prev.L) + (data->qd_dist.R - data->qd_prev.R)) / 2;
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
}
