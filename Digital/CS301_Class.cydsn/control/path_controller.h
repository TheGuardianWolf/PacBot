#include <cytypes.h>
#include <stdbool.h>

#include "motor_controller.h"
#include "sensors_controller.h"

typedef struct {
    MCData mc_data;
    SCData sc_data;
    int8_t heading; // -1 - Stay, 0 - North, 1 - East, 2 - South, 3 - West
} PCData;

void path_controller_init();

PCData path_controller_create(int8_t initial_heading, bool use_wireless, bool use_line);

void path_controller_worker(PCData* data);
