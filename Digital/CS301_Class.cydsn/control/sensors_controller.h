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
#define DI_F 1
#define DI_B 2
#define DI_LR 3
#define DI_FB 3
#define PI 3.14159265358979

typedef struct {
    uint32_t sample_time;
    bool use_wireless;
    bool use_line;
    uint8_t line_tracking;
    uint8_t line_intersection[2];
    bool line_end;
    bool line_lost;
    uint8_t line_inversions;
    bool line_front_lost;
    DiPoint start_loc;
    DiPoint curr_loc;
    bool loc_valid;
    int32_t qd_differential;
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
