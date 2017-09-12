#include <cytypes.h>
#include <stdbool.h>

#include "motor_controller.h"
#include "sensors_controller.h"

#define LINE(x) data->sc_data.line_state[x]

typedef struct {
    MCData mc_data;
    SCData sc_data;
    int8_t heading; // -1 - Stay, 0 - North, 1 - East, 2 - South, 3 - West
    int8_t intersection; // 0 - None, 1 - Left only, 2 - Right only, 3 - Both
    bool path_end;
} PCData;

void path_controller_init();

PCData path_controller_create(int8_t initial_heading);

void path_controller_worker(PCData* data);
