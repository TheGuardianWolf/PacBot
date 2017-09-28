#ifndef SENSORS_CONTROLLER_H
#define SENSORS_CONTROLLER_H

#include <cytypes.h>
#include <stdbool.h>
    
#include "sensors.h"
#include "wireless.h"
#include "quad_dec.h"

#define WHEEL_CIRCUMFERENCE 204 // millimeters
#define PULSES_PER_REV 228 // Decoder x4
#define LINE_SENSORS SIGMUX_MAX + 1
#define ORIENTATION_REV 360
#define ORIENTATION_HREV 180
#define DI_N 0
#define DI_L 1
#define DI_R 2
#define DI_LR 3
#define PI 3.14159265358979

typedef struct {
    uint32_t sample_time;
    bool use_wireless;
    bool use_line;
    int8_t prev_intersection;
    int8_t curr_intersection; // 0 - None, 1 - Left only, 2 - Right only, 3 - Both
    bool line_end;
    bool line_tracking;
    bool line_front_lost;
    bool line_track_centered;
    int8_t line_track;
    int8_t line_curve; // 0 - None, 1 - Left, 2 - Right
    uint8_t line_inversions;
    bool line_lost;
    DiPoint start_loc;
    DiPoint curr_loc;
    bool loc_valid;
    QuadDecData qd_start;
    QuadDecData qd_dist;
    QuadDecData qd_prev;
    int32_t rel_dist;
    int32_t rel_orientation;
    float curr_speed_L;
    float curr_speed_R;
    uint32_t last_run;
}  SCData;

void sensors_controller_init();

SCData sensors_controller_create(uint32_t sample_time, bool use_wireless, bool use_line);

void sensors_controller_worker(SCData* data);

void sensors_controller_reset(SCData* data);

#endif /* SENSORS_CONTROLLER_H */
