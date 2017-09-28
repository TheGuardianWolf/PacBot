#include <project.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#include "sensors_controller.h"
#include "systime.h"
#include "adc.h"

// (s1-s0)/dt -> pulse/ms
static float calc_speed(int32_t curr, int32_t prev, uint32_t dt) {
    return (float) (curr - prev) / (float) dt;
}

static void calculate_orientation(SCData* data, RFData* rf_data) {
    if (data->use_wireless) {
        int16_t x_change, y_change;
        float orientation_buffer = 0.0f;
            
        x_change = rf_data->robot_xpos - data->start_loc.x; 
        y_change = rf_data->robot_ypos - data->start_loc.y;
        
        x_change = abs(x_change);
        y_change = abs(y_change);
        
        if(x_change != 0) {
            orientation_buffer = atanf((float)y_change/x_change);
            //4th quadrant
            if(y_change > 0 && x_change > 0){
                orientation_buffer *= 180/PI;
                orientation_buffer = 360 - orientation_buffer;
            }
            //1st quadrant
                else if(y_change < 0 && x_change > 0){
                orientation_buffer *= 180/PI;
            }
            //2nd quadrant
            else if( y_change < 0 && x_change < 0){
                orientation_buffer *= 180/PI;
                orientation_buffer = 180 - orientation_buffer;
            }
            //3rd quadrant
            else if( y_change > 0 && x_change < 0){
                orientation_buffer *= 180/PI;
                orientation_buffer += 180;
            }
            else if(x_change > 0) {
                orientation_buffer = 0;
            }
            else if(x_change < 0) {
                orientation_buffer = 180;
            }
        }
        else if (y_change > 0) {
            orientation_buffer = 270;
        }
        else if (y_change < 0){
           orientation_buffer = 90;
        }
    data->curr_loc.orientation = (int16_t)orientation_buffer;
    }
    return;
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
        .line_tracking = DI_N,
        .line_tracking_aggressive = false,
        .line_intersection = DI_N,
        .line_intersection_prev = DI_N,
        .line_front_lost = false,
        .line_end = false,
        .line_lost = false,
        .line_inversions = 0,
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

        if (LINE_INV(3) && LINE_INV(4)) {
            data->line_front_lost = true;
        }
        else {
            data->line_front_lost = false;
        }

        uint8_t line_tracking = DI_N;
        bool line_tracking_aggressive = false;

        if (LINE_INV(3) || LINE_INV(4)) {
            uint8_t line_tracking_prev = data->line_tracking;
            line_tracking = (uint8_t) LINE_INV(3) * DI_R + (uint8_t) LINE_INV(4) * DI_L;
            if (line_tracking == DI_LR) {
                line_tracking = line_tracking_prev;
                data->line_front_lost = true;
            }
        }

        if (data->line_front_lost) {
            line_tracking_aggressive = true;
            if (LINE_INV(1) || LINE_INV(2)) {
                uint8_t line_tracking_prev = data->line_tracking;
                line_tracking = (uint8_t) LINE_INV(1) * DI_L + (uint8_t) LINE_INV(2) * DI_R;
                if (line_tracking == DI_LR) {
                    line_tracking = line_tracking_prev;
                }
            }
        } else if (LINE_INV(0)) {
            if (LINE_INV(1) || LINE_INV(2)) {
                uint8_t line_intersection = (uint8_t) LINE_INV(1) * DI_L + (uint8_t) LINE_INV(2) * DI_R;
                if (line_intersection > 0) {
                    data->line_intersection_prev = data->line_intersection;
                } 
                data->line_intersection = line_intersection;
            }
        }
        
        if (LINE(3) || LINE(4)) {
            line_tracking_aggressive = false;
        }

        data->line_tracking = line_tracking;
        data->line_tracking_aggressive = line_tracking_aggressive;
        
        // Check if lost or line has ended
        if ((LINE_INV(0) && LINE_INV(1) && LINE_INV(2) && LINE_INV(3) && LINE_INV(4))) {
            if (LINE(5)) {
                data->line_end = true;
            }
            else {
                data->line_lost = true;
            }
        }
        else {
            data->line_end = false;
            data->line_lost = false;
        }

        // Automatic sensor enable/disable for higher switching speed
         if (LINE(0)) {
             sensors_line_disable(5);
         }
         else {
             sensors_line_enable(5);
         }

         if (LINE(3) || LINE(4)) {
             sensors_line_disable(1);
             sensors_line_disable(2);
         }
         else {
             sensors_line_enable(1);
             sensors_line_enable(2);
         }
    }
    
    // Wireless fusion section - Guard via interrupt flag
    if (data->use_wireless && wireless_check()) {
        RFData rf_data = wireless_get();
        data->loc_valid = (now - rf_data.timestamp < data->sample_time);

        data->curr_loc.x = rf_data.robot_xpos;
        data->curr_loc.y = rf_data.robot_ypos;
        calculate_orientation(data, &rf_data);
        data->rel_orientation = data->curr_loc.orientation - data->start_loc.orientation;
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
    calculate_orientation(data, &rf_data);
    data->start_loc.orientation = data->curr_loc.orientation;
    data->curr_loc = data->start_loc; 
    data->qd_start = qd_data;
    data->qd_dist.L = 0;
    data->qd_dist.R = 0;
}
