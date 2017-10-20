#include <stdio.h>
#include "minunit.h"
#include "graph.h"
#include <stddef.h>
#include <voidtypes.h>
#include "graph_travel_all.h"
#include "graph_astar.h"
#include "pathfinder.h"
#include <sys/time.h>
#include <time.h>

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

static double getTime() {
    struct timeval t;
    double sec, msec;

    while (gettimeofday(&t, NULL) != 0);
    sec = t.tv_sec;
    msec = t.tv_usec;

    sec = sec + msec/1000000.0;

    return sec;
}

static char * test_pathfinder() {
    double t1, t2, t3;
    size_t i;
    uint8_t test_map PACMAN_MAP;
    
    Graph* graph = graph_create((uint8_t*) test_map, 15, 19);
    mu_assert("test_graph_travel_all: error, graph == NULL", graph != NULL);
    mu_assert("test_graph_travel_all: error, graph->nodes->size != 140", graph->nodes->size == 140);
    mu_assert("test_graph_travel_all: error, graph->unique_edges != 154", graph->unique_edges == 154);
    
    point_uint8_t start = MATLAB_START;
    point_uint8_t end = MATLAB_TARGET;

    t1 = getTime();
    LinkedList* ll = pathfinder(graph, &graph_travel_all, start, end);
    mu_assert("test_graph_travel_all: error, ll->size != MATLAB_TRAVEL_ALL_PATH_SIZE", ll->size == MATLAB_TRAVEL_ALL_PATH_SIZE);
    linked_list_destroy(ll);
    t2 = getTime();
    ll = pathfinder(graph, &graph_astar, start, end);
    mu_assert("test_graph_travel_all: error, ll->size != MATLAB_ASTAR_PATH_SIZE", ll->size == MATLAB_ASTAR_PATH_SIZE);
    linked_list_destroy(ll);
    t3 = getTime();

    printf("Travel all took: %3.9lf\n", t2 - t1);
    printf("Astar took: %3.9lf\n", t3 - t2);

    graph_destroy(graph);
    return 0;
}


static char * all_tests() {
    mu_run_test(test_pathfinder);

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
