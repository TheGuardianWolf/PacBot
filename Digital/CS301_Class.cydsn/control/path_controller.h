#include <cytypes.h>
#include <stdbool.h>

#include "motor_controller.h"
#include "sensors_controller.h"
#include "graph.h"
#include "linked_list.h"

#define PATHFINDER_TURN_SPEED 0.15f
#define PATHFINDER_STRAIGHT_SPEED 0.2f
#define GRID_BLOCK_WIDTH 132.19f
#define GRID_BLOCK_HEIGHT 130.33f
#define GRID_BLOCK_WIDTH_PX 52.875f
#define GRID_BLOCK_HEIGHT_PX 51.333f
#define GRID_OFFSET_X 201.0f
#define GRID_OFFSET_Y 194.0f
#define GRID_OFFSET_X_PX 80.4f
#define GRID_OFFSET_Y_PX 77.6f

// 211.5 cm width for 16 blocks travel
// 26.5 cm width for 2 blocks travel
// 154 cm height for 12 blocks travel
// 26 cm height for 2 blocks travel

typedef struct {
    Graph* graph;
    LinkedList* path;
    LinkedList* astar_path;
    LinkedList* travel_path;
    uint32_t sample_time;
    LinkedList* command_queue;
    graph_size_t current_node_id;
    graph_size_t next_node_id;
    MotorCommand* last_command;
    uint8_t heading;
    uint8_t next_heading;
    SCData* sc_data;
    MCData* mc_data;
    uint32_t last_run;
} PCData;

DiPoint rf2grid(DiPoint rf_loc);

void path_controller_init();

PCData path_controller_create(uint32_t sample_time, SCData* scd, MCData* mcd);

void path_controller_load_data(PCData* data, uint8_t* grid, uint8_t grid_height, uint8_t grid_width, uint8_t* food_list, uint8_t food_list_size, point_uint8_t start);

void path_controller_add_command(PCData* data, MotorCommand* cmd);

void path_controller_worker(PCData* data);
