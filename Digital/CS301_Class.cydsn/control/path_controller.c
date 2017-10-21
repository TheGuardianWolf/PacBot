#include <project.h>
#include "path_controller.h"
#include "graph_travel_all.h"
#include "graph_astar.h"
#include "pathfinder.h"
#include "voidtypes.h"
#include "systime.h"
#include "interactive.h"
#include <math.h>
#include <stdlib.h>
#include <stddef.h>

static bool check_start(Graph* graph, graph_size_t node_id) {
    point_uint8_t start_grid = graph_nodeid2grid(graph, node_id);
    return start_grid.x == GRID_START_X && start_grid.y == GRID_START_Y;
}

static point_uint8_t real2grid(DiPoint real_loc) {
    point_uint8_t grid_pos = {
        .x = real_loc.x,
        .y = real_loc.y
    };

    return grid_pos;
}

static void pathfinder_update_path(PCData* data) {
    if (data->current_node_id == NODE_INVALID && data->graph->nodes->size > 1) {
        bool short_boost = false;

        // Pre-travel checks to make sure we're actually on the next node
        if (data->next_node_id != NODE_INVALID) {
            uint8_t node_order = graph_node_order(data->graph, data->next_node_id);
            // Test 1: Check for matching node orders
            uint8_t real_node_order = 0;

            uint8_t i;
            for (i = 0; i < 2; i++) {
                switch(data->sc_data->line_intersection[i]) {
                    case 1:
                    real_node_order++;
                    break;
                    case 2:
                    real_node_order++;
                    break;
                    case 3:
                    real_node_order += 2;
                    break;
                    default:
                    break;
                }
            }
            
            if (real_node_order == node_order) {
                if (data->sc_data->use_wireless) {
                    // Extract next node coordinate
                    point_uint8_t grid_loc = graph_nodeid2grid(data->graph, data->next_node_id);
                
                    // Calculate current grid position
                    point_uint8_t real_grid_loc = real2grid(data->sc_data->curr_loc);
    
                    // Test 2: Check if currently in next grid position via RF
                    if (grid_loc.x != real_grid_loc.x || grid_loc.y != real_grid_loc.y) {
                        short_boost = true;
                    }
                }
            }
            else {
                short_boost = true;
            }
        }

        if (!short_boost) {
            MotorCommand* cmd;
            
            if (data->last_command->drive_mode != 1) {
                // Only pull from path if we're not adjusting orientations
                graph_size_t current_node_id = (graph_size_t)(uvoid_t) linked_list_pop(data->path);
                graph_size_t next_node_id = (graph_size_t)(uvoid_t) linked_list_peek(data->path);
                
                data->current_node_id = current_node_id;
                data->next_node_id = next_node_id;

                // Find the travel arc
                GraphNode* current_node = vector_get(data->graph->nodes, data->current_node_id);
                graph_size_t i;
                for (i = 0; i < current_node->edges->size; i++) {
                    GraphEdge* travel_edge = vector_get(current_node->edges, i);
                    GraphArc* travel_arc = graph_arc_from(travel_edge, data->current_node_id);
                    if (travel_arc != NULL && travel_arc->destination == next_node_id) {
                        data->next_heading = travel_arc->heading;
                        break;
                    }
                }
            }

            // Set motor speed and mode
            if (data->next_heading != data->heading) {
                // Turn to face the travel arc
                cmd = malloc(sizeof(MotorCommand));
                cmd->drive_mode = 1;
                cmd->arg = (int32_t) (data->next_heading - data->heading) * 90;
                cmd->speed = 0.15;
                linked_list_add(data->command_queue, cmd);
                data->heading = data->next_heading;
            }
            else {
                // Travel straight if already oriented
                cmd = malloc(sizeof(MotorCommand));
                cmd->drive_mode = 0;
                if (data->next_heading == G_N || data->next_heading == G_S) {
                    cmd->arg = (int32_t) roundf(GRID_BLOCK_HEIGHT);
                }
                else {
                    cmd->arg = (int32_t) roundf(GRID_BLOCK_WIDTH);
                }
                cmd->speed = 0.15;
                linked_list_add(data->command_queue, cmd);
            }
        }
        else {
            // Apply short command boost to attempt to find a trigger
            MotorCommand cmd = {
                .drive_mode = data->last_command->drive_mode,
                .arg = (int32_t) (data->last_command->arg * 0.1),
                .speed = 0.15
            };
            
            motor_controller_set(data->mc_data, &cmd);
        }
    }
}

PCData path_controller_create(uint32_t sample_time, SCData* scd, MCData* mcd) {
    PCData data = {
        .sample_time = sample_time,
        .heading = -1,
        .next_heading = -1,
        .current_node_id = NODE_INVALID,
        .next_node_id = NODE_INVALID,
        .command_queue = linked_list_create(),
        .mc_data = mcd,
        .sc_data = scd,
        .pathfinder = false,
        .last_run = 0
    };

    return data;
}

void path_controller_load_data(PCData* data, uint8_t* grid, uint8_t grid_height, uint8_t grid_width, point_uint8_t start, point_uint8_t end, int8_t initial_heading) {
    // Load the map in here
    data->heading = initial_heading;
    data->graph = graph_create(grid, grid_height, grid_width);
    data->path = pathfinder(data->graph, &graph_travel_all, start, end);
    data->pathfinder = true;
}

void path_controller_add_command(PCData* data, MotorCommand* cmd) {
    MotorCommand* mc = malloc(sizeof(MotorCommand));
    *mc = *cmd;
    linked_list_add(data->command_queue, mc);
}

void path_controller_worker(PCData* data) {
    uint32_t now = systime_ms();
    uint32_t time_diff = now - data->last_run;
    if (time_diff >= data->sample_time) {
        // If robot is idle now
        if (data->sc_data->curr_speed_L == 0 && data->sc_data->curr_speed_R == 0 && 
             data->mc_data->idle) {
                if (data->command_queue->size == 0) {
                    // Update path if out of commands
                    if (data->pathfinder) {
                        pathfinder_update_path(data);
                    }
                }
                else {
                    // Run next command
                    // sensors_controller_reset(data->sc_data);
                    // data->mc_data->target_dist.L = 0;
                    // data->mc_data->target_dist.R = 0;
                    // data->mc_data->ref_dist.L = 0;
                    // data->mc_data->ref_dist.R = 0;
                    MotorCommand* cmd = linked_list_pop(data->command_queue);
                    
               
                    motor_controller_set(data->mc_data, cmd);
                    if (data->last_command != NULL) {
                        free(data->last_command);
                    }
                    data->last_command = cmd;
                }
         }
    }
}
