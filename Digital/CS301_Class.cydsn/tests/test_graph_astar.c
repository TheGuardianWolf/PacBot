#include <stdio.h>
#include "minunit.h"
#include <stddef.h>
#include "voidtypes.h"

int tests_run = 0;

int foo = 7;
int bar = 4;
int baz = 1;
int* bof = &foo;

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

#define MATLAB_PATH_SIZE 33
// Format as [row, col]
#define MATLAB_PATH [MATLAB_PATH_SIZE][2] = {\
    {2,2},{3,2},{4,2},{5,2},{6,2},{6,3},{6,4},{7,4},{8,4},{8,5},{8,6},{8,7},\
    {8,8},{9,8},{10,8},{10,9},{10,10},{10,11},{10,12},{10,13},{10,14},{10,15},\
    {10,16},{10,17},{10,18},{11,18},{12,18},{12,17},{12,16},{13,16},{14,16},\
    {14,17},{14,18}\
}
#define MATLAB_START {.x = 2, .y = 2 }
#define MATLAB_TARGET {.x = 18, .y = 14}

static char * test_graph_astar() {
    size_t i;
    size_t j;
    uint8_t test_map PACMAN_MAP;
    uint8_t answer = MATLAB_PATH;
    
    Graph* graph = graph_create((uint8_t*) test_map, 15, 19);
    mu_assert("test_graph_create: error, graph == NULL", graph != NULL);
    mu_assert("test_graph_create: error, graph->nodes->size != 130", graph->nodes->size == 130);
    mu_assert("test_graph_create: error, graph->unique_edges != 134", graph->unique_edges == 134);
    
    point_uint8_t start;
    start.x = 1;
    start.y = 1;

    point_uint8_t end;
    end.x = 17;
    end.y = 13;

    LinkedList* ll;

    ll = graph_astar(graph, graph_grid2nodeid(start), graph_grid2nodeid(end));
    j = ll->size;
    int array[j];

    for (i = 0; i < j; i++) {
        array[i] = ((uvoid_t) linked_list_pop(ll));
    }

    graph_destroy(graph);
    return 0;
}


static char * all_tests() {
    mu_run_test(test_graph_astar);

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
