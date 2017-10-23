#include <project.h>
#include "path_controller.h"
#include "graph_travel_all.h"
#include "graph_astar.h"
#include "pathfinder.h"
#include "voidtypes.h"
#include "systime.h"
#include "interactive.h"
#include "map.h"
#include <math.h>
#include <stdlib.h>
#include <stddef.h>

#define USE_REVERSE 0

DiPoint rf2grid(DiPoint rf_loc) {
    DiPoint grid_pos = {
        .x = roundf(((float) rf_loc.x - GRID_OFFSET_X_PX) / GRID_BLOCK_WIDTH_PX),
        .y = roundf(((float) rf_loc.y - GRID_OFFSET_Y_PX) / GRID_BLOCK_HEIGHT_PX),
        .orientation = roundf((((float) rf_loc.orientation / RF_DEGREESPERUNIT) + 45) / 90) + 1
    };

    if (grid_pos.orientation > 4) {
        grid_pos.orientation = 1;
    }

    return grid_pos;
}

static void pathfinder_turn(PCData* data, int32_t turn) {
    MotorCommand* cmd;

    // Turn to face the next heading
    cmd = malloc(sizeof(MotorCommand));
    cmd->drive_mode = 1;
    cmd->arg = turn * 100;
    cmd->speed = PATHFINDER_TURN_SPEED;
    linked_list_add(data->command_queue, cmd);
    
    data->heading = data->next_heading;
}

static void pathfinder_straight(PCData* data, int32_t blocks) {
    MotorCommand* cmd;

    // Travel straight if already oriented
    cmd = malloc(sizeof(MotorCommand));
    cmd->drive_mode = 0;
    if (data->heading == G_N || data->heading == G_S) {
        cmd->arg = (int32_t) roundf(GRID_BLOCK_HEIGHT * blocks);
    }
    else {
        cmd->arg = (int32_t) roundf(GRID_BLOCK_WIDTH * blocks);
    }
    cmd->speed = PATHFINDER_STRAIGHT_SPEED;
    linked_list_add(data->command_queue, cmd);
}

static void pathfinder_update_path(PCData* data) {
    if (data->path->size > 0) {
        bool short_boost = false;
        int32_t straight_blocks = 0;
        int8_t turn = 0;

        // Pre-travel checks to make sure we're actually on the next node
        if (data->sc_data->use_wireless) {
            if (data->next_node_id != 0) {
                // Extract next node coordinate
                point_uint8_t grid_loc = graph_nodeid2grid(data->graph, data->next_node_id);
            
                // Calculate current grid position
                DiPoint real_grid_loc = rf2grid(data->sc_data->curr_loc);

                // Test: Check if currently in right grid or orientation with RF
                if (data->last_command->drive_mode == 1) {
                    if (data->heading != real_grid_loc.orientation) {
                        data->last_command->arg = ((int32_t) data->heading - real_grid_loc.orientation) * 35;
                        short_boost = true;
                    }
                }
                else {
                    if (grid_loc.x != real_grid_loc.x || grid_loc.y != real_grid_loc.y) {
                        short_boost = true;
                        if (data->heading == G_N) {
                            data->last_command->arg = ((int32_t) real_grid_loc.y - grid_loc.y) * 65;
                        }
                        else if (data->heading == G_S) {
                            data->last_command->arg = ((int32_t) grid_loc.y - real_grid_loc.y) * 65;
                        }
                        else if (data->heading == G_E) {
                            data->last_command->arg = ((int32_t) grid_loc.x - real_grid_loc.x) * 65;
                        }
                        else if (data->heading == G_W) {
                            data->last_command->arg = ((int32_t) real_grid_loc.x - grid_loc.x) * 65;
                        }
                        else {
                            short_boost = false;
                        }
                    }
                }
            }
        }

        if (!short_boost) {
            // Only pull from path if we're not adjusting orientations
            graph_size_t current_node_id = data->next_node_id;
            if (current_node_id == NODE_INVALID) {
                current_node_id = (graph_size_t)(uvoid_t) linked_list_pop(data->path);
            }
            data->current_node_id = current_node_id;
            
            GraphArc* travel_arc;
            graph_size_t next_node_id;

            while((graph_size_t)(uvoid_t) linked_list_peek(data->path) == current_node_id) {
                linked_list_pop(data->path);
            }
            
            while(true) {
                next_node_id = (graph_size_t)(uvoid_t) linked_list_peek(data->path);
                data->next_node_id = next_node_id;
                // Find the travel arc
                GraphEdge* travel_edge = graph_edge_find(data->graph, current_node_id, next_node_id, &travel_arc);

                if (travel_arc == NULL || travel_edge == NULL) {
                    // Break if unexpected nulls found
                    break;
                }

                turn = (data->heading - travel_arc->heading);

                if (turn == 0 && straight_blocks >= 0) {
                    // Straight line travel and concatenation if going forward
                    straight_blocks++;
                    current_node_id = (graph_size_t)(uvoid_t) linked_list_pop(data->path);
                }
                #if USE_REVERSE == 1
                else if (abs(turn) == 2 && straight_blocks <= 0) {
                    // Straight line travel and concatenation if going backwards
                    straight_blocks--;
                    current_node_id = (graph_size_t)(uvoid_t) linked_list_pop(data->path);
                }
                #endif
                else {
                    // Not a straight line, don't concatenate
                    break;
                }
            } 

            data->next_heading = travel_arc->heading;
            data->next_node_id = current_node_id;

            // Set motor speed and mode
            if (straight_blocks > 0) {
                pathfinder_straight(data, straight_blocks);
            }
            #if USE_REVERSE == 1
            else if (straight_blocks < 0) {
                pathfinder_straight(data, straight_blocks);
            }
            #endif
            else {
                turn = data->heading - data->next_heading;
                if (turn < -2 || turn > 2) {
                    turn = copysignf(1.0f, turn * -1);
                }
                pathfinder_turn(data, turn);
            }
        }
        else {
            // Apply short command boost to attempt to find a trigger
            MotorCommand cmd = {
                .drive_mode = data->last_command->drive_mode,
                .arg = data->last_command->arg,
                .speed = 0.3
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
        .last_command = NULL,
        .mc_data = mcd,
        .sc_data = scd,
        .last_run = 0
    };

    return data;
}

void path_controller_load_data(PCData* data, uint8_t* grid, uint8_t grid_height, uint8_t grid_width, uint8_t* food_list, uint8_t food_list_size, point_uint8_t start) {
    // Load the map in here
    data->graph = graph_create(grid, grid_height, grid_width);
    data->travel_path = pathfinder(data->graph, &graph_travel_all, start, start);

    // Parse and path the food list
    uint8_t i;
    LinkedList* astar_path = NULL;
    
    for (i = 0; i < food_list_size; i++) {
        point_uint8_t target = {
            .x = food_list[i * 2 + 0],
            .y = food_list[i * 2 + 1]
        };

        LinkedList* segment_path = pathfinder(data->graph, &graph_astar, start, target);
        if (astar_path == NULL || astar_path->size == 1) {
            astar_path = segment_path;
        }
        else {
            astar_path->last->prev->next = segment_path->first;
            segment_path->first->prev = astar_path->last->prev;
            astar_path->size--;
            astar_path->size += segment_path->size;
            free(astar_path->last);
            astar_path->last = segment_path->last;
            free(segment_path);
        }
        start.x = target.x;
        start.y = target.y;
    }
    data->astar_path = astar_path;

//    data->current_node_id = graph_grid2nodeid(data->graph, start);   
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
                    if (data->path != NULL) {
                        pathfinder_update_path(data);
                    }
                }
                else {
                    // Run next command

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
