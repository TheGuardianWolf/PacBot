#ifndef SENSORS_CONTROLLER_H
#define SENSORS_CONTROLLER_H

#include <cytypes.h>
#include <stdbool.h>
    
#include "sensors.h"
    
#define LINE_SAMPLE_RATE 20
#define LINE_SENSORS SIGMUX_MAX + 1

typedef struct {
    bool line_state[6]; // True for on the line, false for off the line
    uint32_t last_run;
}  SCData;

void sensors_controller_init();

SCData sensors_controller_create();

void sensors_controller_worker(SCData* data);

#endif /* SENSORS_CONTROLLER_H */
