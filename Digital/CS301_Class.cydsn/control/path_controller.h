#include <cytypes.h>
#include <stdbool.h>

#include "motor_controller.h"
#include "sensors_controller.h"

#define GRAPH_BLOCK_WIDTH 13.219f
#define GRAPH_BLOCK_HEIGHT 12.833f

// 211.5 mm width for 16 blocks travel
// 26.5 mm width for 2 blocks travel
// 154mm height for 12 blocks travel
// 26 mm height for 2 blocks travel

typedef struct {
    uint32_t sample_time;
    int8_t heading; // -1 - Stay, 0 - North, 1 - East, 2 - South, 3 - West
    uint32_t last_run;
} PCData;

void path_controller_init();

PCData path_controller_create(int8_t initial_heading, bool use_wireless, bool use_line);

void path_controller_worker(PCData* data);
