#include <project.h>

#include "sensors_controller.h"
#include "systime.h"
#include "sensors.h"
#include "adc.h"

void sensors_controller_init() {
    adc_init();
    sensors_init();
}

SCData sensors_controller_create() {
    SCData data = {
        .line_state = {true, false, false, false, false, true},
        .last_run = 0
    };
    return data;
}

void sensors_controller_worker(SCData* data) {
    uint32_t now = systime_ms();
    uint32_t time_diff = now - data->last_run;

    if (time_diff >= LINE_SAMPLE_RATE) {
        uint8_t line_data = sensors_line_get();
        for (uint8_t i = 0; i < LINE_SENSORS; i++) {
            data->line_state[i] = (bool) ((line_data >> i) & 1); 
        }
    }
}
