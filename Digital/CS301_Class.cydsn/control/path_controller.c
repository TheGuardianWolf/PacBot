#include <project.h>
#include "path_controller.h"
#include "systime.h"
#include "graph.h"
#include "linked_list.h"
#include <stdlib.h>

void path_controller_init() {
    sensors_controller_init();
    motor_controller_init();
}

PCData path_controller_create(uint32_t sample_time) {
    PCData data = {
        .sample_time = 30,
        .sc_data = sc_data,
        .mc_data = mc_data,
        .prev_heading = initial_heading,
        .heading = initial_heading,
        .last_run = 0
    };
    motor_controller_set(data.mc_data, 0.15, 0, 0xFFFF);
    return data;
}


void path_controller_worker(PCData* data, MCData* mcd, SCData* scd, Graph* graph, LinkedList* path) {
    sensors_controller_worker(data->sc_data);
    
    uint32_t now = systime_ms();
    uint32_t time_diff = now - data->last_run;
    if (time_diff >= data->sample_time) {
        // Extract coordinate from path
        // Verify current position
        // Determine next heading
        // Set motor speed and mode
        // Stop if condition met
    }
    motor_controller_worker(data->mc_data);
}
