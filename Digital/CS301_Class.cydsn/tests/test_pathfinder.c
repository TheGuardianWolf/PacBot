#include <stdio.h>
#include "minunit.h"
#include "graph.h"
#include <stddef.h>
#include <voidtypes.h>
#include "graph_travel_all.h"
#include "graph_astar.h"
#include "pathfinder.h"

int tests_run = 0;

#define PACMAN_MAP [15][19] = {\
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},\
    {1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},\
    {1,0,1,0,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1},\
    {1,0,1,0,1,0,1,0,0,0,0,0,1,0,0,0,0,0,1},\
    {1,0,1,0,1,0,0,0,1,1,1,0,1,0,1,1,1,0,1},\
    {1,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,0,0,1},\
    {1,1,1,0,1,1,1,0,1,0,1,0,1,0,1,1,1,0,1},\
    {1,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1},\
    {1,1,1,0,1,0,1,0,1,1,1,0,1,0,1,0,1,1,1},\
    {1,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,1},\
    {1,0,1,1,1,0,1,0,1,0,1,0,1,1,1,1,1,0,1},\
    {1,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1},\
    {1,0,1,1,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1},\
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},\
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},\
}

#define MATLAB_TRAVEL_ALL_PATH_SIZE 234
#define MATLAB_ASTAR_PATH_SIZE 33

#define MATLAB_START {.x = 1, .y = 1 }
#define MATLAB_TARGET {.x = 17, .y = 13}

static char * test_graph_travel_all() {
    size_t i;
    uint8_t test_map PACMAN_MAP;
    
    Graph* graph = graph_create((uint8_t*) test_map, 15, 19);
    mu_assert("test_graph_travel_all: error, graph == NULL", graph != NULL);
    mu_assert("test_graph_travel_all: error, graph->nodes->size != 140", graph->nodes->size == 140);
    mu_assert("test_graph_travel_all: error, graph->unique_edges != 154", graph->unique_edges == 154);
    
    point_uint8_t start = MATLAB_START;
    point_uint8_t end = MATLAB_TARGET;

    LinkedList* ll = pathfinder(graph, &graph_travel_all, start, end);
    mu_assert("test_graph_travel_all: error, ll->size != MATLAB_TRAVEL_ALL_PATH_SIZE", ll->size == MATLAB_TRAVEL_ALL_PATH_SIZE);
    linked_list_destroy(ll);
    ll = pathfinder(graph, &graph_astar, start, end);
    mu_assert("test_graph_travel_all: error, ll->size != MATLAB_ASTAR_PATH_SIZE", ll->size == MATLAB_ASTAR_PATH_SIZE);
    linked_list_destroy(ll);

    graph_destroy(graph);
    return 0;
}


static char * all_tests() {
    mu_run_test(test_graph_travel_all);

    return 0;
}

int main(int argc, char **argv) {
    char *result = all_tests();
    if (result != 0) {
        printf("%s\n", result);
    }
    else {
        printf("ALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", tests_run);

    return result != 0;
}
