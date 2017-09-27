#include <project.h>
#include "path_controller.h"
#include "systime.h"

void path_controller_init() {
    sensors_controller_init();
    motor_controller_init();
}

PCData path_controller_create(int8_t initial_heading, bool use_wireless, bool use_line) {
    SCData sc_data = sensors_controller_create(30, use_wireless, use_line);
    MCData mc_data = motor_controller_create(30, &sc_data);
    PCData data = {
        .sample_time = 30,
        .sc_data = sc_data,
        .mc_data = mc_data,
        .line_intersect = DI_N,
        .line_intersect_prev = DI_N,
        .prev_heading = initial_heading,
        .heading = initial_heading,
        .last_run = 0
    };
    return data;
}
//
//void calculate_turn(PCData* data, SCData *sc_data) {
//    //heading east
//    if(sc_data->curr_loc.orientation < 15 || sc_data->curr_loc.orientation > 345) {
//        
//    }
//    //heading east
//    else if(sc_data->curr_loc.orientation < 15 || sc_data->curr_loc.orientation > 345) {
//        
//    }
//    //heading east
//    else if(sc_data->curr_loc.orientation < 15 || sc_data->curr_loc.orientation > 345) {
//        
//    }
//    //heading east
//    else if(sc_data->curr_loc.orientation < 15 || sc_data->curr_loc.orientation > 345) {
//        
//    }
//    else {
//    
//    }
//   
//    
//}

void path_controller_worker(PCData* data) {
    sensors_controller_worker(&(data->sc_data));
    
    uint32_t now = systime_ms();
    uint32_t time_diff = now - data->last_run;
    if (time_diff >= data->sample_time) {
        if (data->sc_data.line_end && data->sc_data.curr_intersection > 0 && data->mc_data.drive_mode != 1) {
            switch (data->sc_data.curr_intersection) {
                case 1:
                motor_controller_set(&(data->mc_data), 0.5, 1, 90);
                break;
                case 2:
                motor_controller_set(&(data->mc_data), 0.5, 1, -90);
                break;
                case 3:
                // Go where heading is.
                break;
                default:
                break;
            }
        }
        else if (data->mc_data.drive_mode != 3) {
            motor_controller_set(&(data->mc_data), 0.8, 3, 0);
        }
    }
    
    #define LINE(x) line_data.state[x]
    #define LINE_INV(x) !line_data.state[x]
    if (data->sc_data.use_line && sensors_line_check()) {
        LineData line_data;
        line_data = sensors_line_get();
        uint8_t i;
        data->sc_data.line_inversions = 0;
        for (i = 0; i < LINE_SENSORS; i++) {
            if (!LINE(i)) {
                data->sc_data.line_inversions++;
            } 
        }
        
//        // If wing sensors are inverted, check for intersection
//        if (LINE_INV(1) || LINE_INV(2)) {
//            // Is intersection
//            if (LINE(0) && LINE(5) && LINE_INV(1) && LINE_INV(2)) {
//                data->sc_data.line_tracking = true;
//                data->line_intersect_prev = data->line_intersect;
//                data->line_intersect = (int8_t) LINE_INV(1) * DI_L + (int8_t) LINE_INV(2) * DI_R;
//                if (data->line_intersect == DI_LR) {
//                    data->;
//                    
//                }
//            }
//
//            // Otherwise intersection
//            if (LINE(0) && LINE(5)) {
//                int8_t intersection = (int8_t) LINE_INV(1) * DI_L + (int8_t) LINE_INV(2) * DI_R;                
//                data->prev_intersection = data->curr_intersection;
//                data->curr_intersection = intersection;
//            }
//        }
    }
}
