#include <project.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#include "sensors_controller.h"
#include "systime.h"
#include "interactive.h"
// #include "adc.h"

#define USE_HERUSTICS 1

// (s1-s0)/dt -> pulse/ms
static float calc_speed(int32_t curr, int32_t prev, uint32_t dt) {
    return (float) (curr - prev) / (float) dt;
}

void sensors_controller_init() {
    systime_init();
    wireless_init();
    sensors_init();
    // adc_init();
}

SCData sensors_controller_create(uint32_t sample_time, bool use_wireless, bool use_line) {
    SCData data = {
        .sample_time = sample_time,
        .use_wireless = use_wireless,
        .use_line = use_line,
        .line_sensor_config = -1,
        .line_tracking = DI_N,
        .line_tracking_prev = DI_N,
        .line_intersection = {DI_N, DI_N},
        .line_front_lost = false,
        .line_end = false,
        .line_lost = false,
        .line_inversions = 0,
        .loc_valid = false,
        .qd_differential = 0,
        .reversed = false,
        .curr_speed_L = 0.0f,
        .curr_speed_R = 0.0f,
        .last_run = 0,
    };
    
    sensors_line_disable(0);
    sensors_line_disable(1);
    sensors_line_disable(2);
    sensors_line_disable(5);

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
//    sensors_controller_set_config(&data, LINE_TRACKING_CONFIG);
    sensors_controller_reset(&data);
    return data;
}

void sensors_controller_set_config(SCData* data, int8_t config) {
//    if (data->line_sensor_config != config) {
//        data->line_sensor_config = config;
//        switch(config) {
//            case LINE_DISABLE_CONFIG:
//            sensors_line_disable(3);
//            sensors_line_disable(4);
//            sensors_line_disable(1);
//            sensors_line_disable(2);
//            sensors_line_enable(0);
//            sensors_line_enable(5);
//            break;
//            case LINE_TRACKING_CONFIG:
//            sensors_line_disable(0);
//            sensors_line_enable(3);
//            sensors_line_enable(4);
//            sensors_line_disable(1);
//            sensors_line_disable(2);
//            sensors_line_disable(5);
//            break;
//            case LINE_INTERSECTION_CONFIG:
//            sensors_line_disable(0);
//            sensors_line_disable(3);
//            sensors_line_disable(4);
//            sensors_line_enable(1);
//            sensors_line_enable(2);
//            sensors_line_enable(5);
//            break;
//            default:
//            break;
//        }
//    }
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

        // Check if front two sensors are both off line.
        if (LINE_INV(3) && LINE_INV(4)) {
            data->line_front_lost = true;
            //led_set(0b001);
        }
        else {
            data->line_front_lost = false;
            data->line_herustic_turn = DI_N;
            //led_set(0b000);
        }

        // Straight line tracking / corrections
        if (LINE(1) && LINE(2)) {
            // If front sensors roll off the line, and side sensors aren't detecting
            // Try to correct.
            if ((LINE_INV(3) || LINE_INV(4)) && !data->reversed) {
                data->line_tracking_prev = data->line_tracking;
                data->line_tracking = (uint8_t) LINE_INV(3) * DI_R + (uint8_t) LINE_INV(4) * DI_L;

                if (data->line_tracking == DI_LR) {
                    // Use herustics submitted by another controller if available.
                    #if USE_HERUSTICS == 1
                    if (data->line_herustic_turn != DI_N) {
                        data->line_tracking = data->line_herustic_turn;
                        data->line_herustic_turn = DI_N;
                    }
                    else {
                    #endif
                        data->line_tracking = data->line_tracking_prev;
                    #if USE_HERUSTICS == 1
                    }
                    #endif
                }
            }
            else {
                data->line_tracking = DI_N;
            }
        }
        else {
            data->line_tracking = DI_N;
        }
       
        // Intersection detection
        data->line_intersection[0] = (uint8_t) LINE_INV(1) * DI_L + (uint8_t) LINE_INV(2) * DI_R;
        data->line_intersection[1] = (uint8_t) !data->line_front_lost * DI_F  + (uint8_t) LINE(5) * DI_B;
        
        // Check if lost or line has ended
        if ((LINE_INV(0) && LINE(1) && LINE(2) && LINE_INV(3) && LINE_INV(4))) {
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
    }
    
    // Wireless fusion section - Guard via interrupt flag
    if (data->use_wireless && wireless_check()) {
        RFData rf_data = wireless_get();
        data->loc_valid = (now - rf_data.timestamp < data->sample_time);

        data->curr_loc.x = rf_data.robot_xpos;
        data->curr_loc.y = rf_data.robot_ypos;
        data->rel_orientation = data->curr_loc.orientation - data->start_loc.orientation;
        if (data->rel_orientation < 0) {
            data->rel_orientation += ORIENTATION_REV;
        }

        if (data->loc_valid) {
            data->rel_dist = (int32) dist2dec(2.5 * sqrtf(powf(rf_data.robot_xpos - data->start_loc.x, 2) + powf(rf_data.robot_ypos - data->start_loc.y, 2)));
        }
        else {
            data->rel_dist += ((data->qd_dist.L - data->qd_prev.L) + (data->qd_dist.R - data->qd_prev.R)) / 2;
        }
    }
}

void sensors_controller_reverse(SCData* data) {
    if (!data->reversed) {
        data->reversed = true;
//        if (data->line_sensor_config == LINE_TRACKING_CONFIG) {
//            sensors_line_enable(5);
//        }
    }
    else {
        data->reversed = false;
//        if (data->line_sensor_config == LINE_TRACKING_CONFIG) {
//            sensors_line_disable(5);
//        }
    }
}

void sensors_controller_reset(SCData* data) {
    data->last_run = systime_ms();
    RFData rf_data = wireless_get();
    QuadDecData qd_data = quad_dec_get();
    data->start_loc.x = rf_data.robot_xpos;
    data->start_loc.y = rf_data.robot_ypos;
    data->start_loc.orientation = data->curr_loc.orientation;
    data->curr_loc = data->start_loc; 
    data->qd_start = qd_data;
    data->qd_dist.L = 0;
    data->qd_dist.R = 0;
}
