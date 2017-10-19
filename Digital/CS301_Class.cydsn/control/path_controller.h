#include <cytypes.h>
#include <stdbool.h>

#include "motor_controller.h"
#include "sensors_controller.h"
#include "graph.h"
#include "linked_list.h"

#define GRID_WIDTH 2379.4f
#define GRID_HEIGHT 1796.6f
#define GRID_BLOCK_WIDTH 132.19f
#define GRID_BLOCK_HEIGHT 128.33f
#define GRID_START_X 0
#define GRID_START_Y 0

// 211.5 mm width for 16 blocks travel
// 26.5 mm width for 2 blocks travel
// 154mm height for 12 blocks travel
// 26 mm height for 2 blocks travel

typedef struct {
    Graph* graph;
    LinkedList* path;
    uint32_t sample_time;
    LinkedList* command_queue;
    graph_size_t current_node_id;
    graph_size_t next_node_id;
    MotorCommand* last_command;
    uint8_t heading;
    SCData* sc_data;
    MCData* mc_data;
    uint32_t last_run;
    bool pathfinder;
} PCData;



void path_controller_init();

PCData path_controller_create(uint32_t sample_time, SCData* scd, MCData* mcd);

void path_controller_load_data(PCData* data, uint8_t* grid, uint8_t grid_height, uint8_t grid_width, point_uint8_t start, point_uint8_t end, int8_t initial_heading);

void path_controller_add_command(PCData* data, MotorCommand* cmd);

void path_controller_worker(PCData* data);
