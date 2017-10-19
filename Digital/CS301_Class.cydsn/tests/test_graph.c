#include <stdio.h>
#include "minunit.h"
#include "graph.h"
#include <stddef.h>

#define PACMAN_MAP [15][19] = {\
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},\
{1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1},\
{1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,0,1,0,1},\
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1},\
{1,0,1,0,1,1,1,1,1,0,1,0,1,1,1,0,1,0,1},\
{1,0,1,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,1},\
{1,0,1,1,1,0,1,0,1,0,1,0,1,0,1,1,1,0,1},\
{1,0,0,0,0,0,1,0,1,0,1,0,1,0,1,0,0,0,1},\
{1,0,1,1,1,1,1,0,1,0,1,1,1,0,1,0,1,1,1},\
{1,0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,1},\
{1,1,1,1,1,1,1,0,1,1,1,0,1,0,1,1,1,0,1},\
{1,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,1},\
{1,0,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,0,1},\
{1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1},\
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},\
}

int tests_run = 0;

int foo = 7;
int bar = 4;
int baz = 1;
int* bof = &foo;

static char * test_graph_create() {
	uint8_t test_map PACMAN_MAP;
	
    Graph* graph = graph_create((uint8_t*) test_map, 15, 19);
    mu_assert("test_graph_create: error, graph == NULL", graph != NULL);
    mu_assert("test_graph_create: error, graph->nodes->size != 130", graph->nodes->size == 130);
    mu_assert("test_graph_create: error, graph->unique_edges != 134", graph->unique_edges == 134);
    graph_destroy(graph);
    return 0;
}

static char * test_graph_grid2nodeid() {
    uint8_t test_map PACMAN_MAP;
    point_uint8_t point;
	
    Graph* graph = graph_create((uint8_t*) test_map, 15, 19);
    mu_assert("test_graph_grid2nodeid: error, graph == NULL", graph != NULL);
    mu_assert("test_graph_grid2nodeid: error, graph->nodes->size != 130", graph->nodes->size == 130);
    mu_assert("test_graph_grid2nodeid: error, graph->unique_edges != 134", graph->unique_edges == 134);

    point.x = 1;
    point.y = 1;
    mu_assert("test_graph_grid2nodeid: error, nodeid != 1", graph_grid2nodeid(graph, point) == 0);

    point.x = 0;
    point.y = 0;
    mu_assert("test_graph_grid2nodeid: error, nodeid != NODE_INVALID1", graph_grid2nodeid(graph, point) == NODE_INVALID);

    point.x = 6;
    point.y = 5;
    mu_assert("test_graph_grid2nodeid: error, nodeid != NODE_INVALID2", graph_grid2nodeid(graph, point) == NODE_INVALID);

    point.x = 17;
    point.y = 13;
    mu_assert("test_graph_grid2nodeid: error, nodeid != 1", graph_grid2nodeid(graph, point) == 129);

    point.x = 15;
    point.y = 19;
    mu_assert("test_graph_grid2nodeid: error, nodeid != NODE_INVALID3", graph_grid2nodeid(graph, point) == NODE_INVALID);

    point.x = 11;
    point.y = 6;
    mu_assert("test_graph_grid2nodeid: error, nodeid != 1", graph_grid2nodeid(graph, point) == 57);
    

    graph_destroy(graph);
    return 0;
}


static char * test_graph_nodeid2grid() {
	
    uint8_t test_map PACMAN_MAP;
    point_uint8_t point;
	
    Graph* graph = graph_create((uint8_t*) test_map, 15, 19);
    mu_assert("test_graph_nodeid2grid: error, graph == NULL", graph != NULL);
    mu_assert("test_graph_nodeid2grid: error, graph->nodes->size != 130", graph->nodes->size == 130);
    mu_assert("test_graph_nodeid2grid: error, graph->unique_edges != 134", graph->unique_edges == 134);

    point = graph_nodeid2grid(graph, 0);
    mu_assert("test_graph_nodeid2grid: error, x!=1", point.x == 1);
    mu_assert("test_graph_nodeid2grid: error, y!=1", point.y == 1);

    point = graph_nodeid2grid(graph, 129);
    mu_assert("test_graph_nodeid2grid: error, x != 17", point.x == 17);
    mu_assert("test_graph_nodeid2grid: error, y != 13", point.y == 13);

    point = graph_nodeid2grid(graph, 130);
    mu_assert("test_graph_nodeid2grid: error, nodeid != 1", point.x == NODE_INVALID);
    mu_assert("test_graph_nodeid2grid: error, nodeid != 1", point.y == NODE_INVALID);

    point = graph_nodeid2grid(graph, 4);
    mu_assert("test_graph_nodeid2grid: error, nodeid != 5", point.x == 5);
    mu_assert("test_graph_nodeid2grid: error, nodeid != 1", point.y == 1);

    point = graph_nodeid2grid(graph, 5);
    mu_assert("test_graph_nodeid2grid: error, nodeid != 7", point.x == 7);
    mu_assert("test_graph_nodeid2grid: error, nodeid != 1", point.y == 1);

    graph_destroy(graph);
    return 0;
}

static char * test_graph_node_order() {
	
    uint8_t test_map PACMAN_MAP;
    int i = 0;
    int j = 0;
	
    Graph* graph = graph_create((uint8_t*) test_map, 15, 19);
    mu_assert("test_graph_node_order: error, graph == NULL", graph != NULL);
    mu_assert("test_graph_node_order: error, graph->nodes->size != 130", graph->nodes->size == 130);
    mu_assert("test_graph_node_order: error, graph->unique_edges != 134", graph->unique_edges == 134);

    mu_assert("test_graph_node_order: error, order != 1", graph_node_order(graph, 0) == 1);
    mu_assert("test_graph_node_order: error, order != 2", graph_node_order(graph, 1) == 2);
    mu_assert("test_graph_node_order: error, order != 2", graph_node_order(graph, 2) == 2);
    mu_assert("test_graph_node_order: error, order != 3", graph_node_order(graph, 13) == 3);
    mu_assert("test_graph_node_order: error, order != 3", graph_node_order(graph, 123) == 3);
    mu_assert("test_graph_node_order: error, order != 0", graph_node_order(graph, 150) == 0);
    mu_assert("test_graph_node_order: error, order != 0", graph_node_order(graph, 130) == 0);

    graph_destroy(graph);
    return 0;
}


static char * test_graph_edge_remove_attach() {
	
    uint8_t test_map PACMAN_MAP;
    GraphEdge* ed;
    GraphNode* nod;
    GraphArc* arc;
    int i;
    int j = 0;
    int head = 5;
	
    Graph* graph = graph_create((uint8_t*) test_map, 15, 19);
    mu_assert("test_graph_edge_remove: error, graph == NULL", graph != NULL);
    mu_assert("test_graph_edge_remove: error, graph->nodes->size != 130", graph->nodes->size == 130);
    mu_assert("test_graph_edge_remove: error, graph->unique_edges != 134", graph->unique_edges == 134);

    nod = vector_get(graph->nodes, 13);
    ed =  vector_get(nod->edges, 1);
    arc = graph_arc_from(ed, 13);

    graph_edge_remove(graph, ed);
    mu_assert("test_graph_edge_remove: error, node 13 order != 2", graph_node_order(graph, 13) == 2);
    for (i = 0; i < nod->edges->size; i++) {
        if (((GraphEdge*) vector_get(nod->edges, i)) == ed) {
            mu_assert("test_graph_edge_remove: error, edge still detected on origin", false);
        }
    } 
    mu_assert("test_graph_edge_remove: error, node 14 order != 1", graph_node_order(graph, 14) == 1);
    nod = vector_get(graph->nodes, 14);
    for (i = 0; i < nod->edges->size; i++) {
        if (((GraphEdge*) vector_get(nod->edges, i)) == ed) {
            mu_assert("test_graph_edge_remove: error, edge still detected on destination", false);
        }
    } 
    mu_assert("test_graph_edge_remove: error, detatched_edges size != 1", graph->detatched_edges->size == 1);

    if(vector_get(graph->detatched_edges, 0) != ed) {
        mu_assert("test_graph_edge_remove: error, removed edge not in detatched_edges", false);
    }

    graph_edge_attach(graph, vector_get(graph->detatched_edges, 0));

    bool edge_detected = false;
    mu_assert("test_graph_edge_remove: error, node 13 order != 3", graph_node_order(graph, 13) == 3);
    for (i = 0; i < nod->edges->size; i++) {
        if (((GraphEdge*) vector_get(nod->edges, i)) == ed) {
            edge_detected = true;
            break;
        }
    } 
    mu_assert("test_graph_edge_remove: error, edge not detected on origin", edge_detected);
    mu_assert("test_graph_edge_remove: error, node 14 order != 2", graph_node_order(graph, 14) == 2);
    nod = vector_get(graph->nodes, 14);
    for (i = 0; i < nod->edges->size; i++) {
        if (((GraphEdge*) vector_get(nod->edges, i)) == ed) {
            edge_detected = true;
            break;
        }
    } 
    mu_assert("test_graph_edge_remove: error, edge not detected on destination", edge_detected);

    mu_assert("test_graph_edge_remove: error, detatched_edges size != 0", graph->detatched_edges->size == 0);

    graph_destroy(graph);
    return 0;
}

static char * test_graph_arc_to() {
	
    uint8_t test_map PACMAN_MAP;
    GraphEdge* ed0, *ed1, *ed2;
    GraphNode* nod;
    GraphArc* arc0, *arc1, *arc2;
	
    Graph* graph = graph_create((uint8_t*) test_map, 15, 19);
    mu_assert("test_graph_create: error, graph == NULL", graph != NULL);
    mu_assert("test_graph_create: error, graph->nodes->size != 130", graph->nodes->size == 130);
    mu_assert("test_graph_create: error, graph->unique_edges != 134", graph->unique_edges == 134);
    
    nod = vector_get(graph->nodes, 13);
    ed0 =  vector_get(nod->edges, 0);
    ed1 =  vector_get(nod->edges, 1);
    ed2 =  vector_get(nod->edges, 2);
    arc0 = graph_arc_to(ed0, 13);
    arc1 = graph_arc_to(ed1, 13);
    arc2 = graph_arc_to(ed2, 13);

    mu_assert("test_graph_arc_to: arc0->destination == 13", arc0->destination == 13);
    mu_assert("test_graph_arc_to: arc1->destination == 13", arc1->destination == 13);
    mu_assert("test_graph_arc_to: arc2->destination == 13", arc2->destination == 13);
    
    graph_destroy(graph);
    return 0;
}

static char * test_graph_arc_from() {
    uint8_t test_map PACMAN_MAP;
    GraphEdge* ed0, *ed1, *ed2;
    GraphNode* nod;
    GraphArc* arc0, *arc1, *arc2;
	
    Graph* graph = graph_create((uint8_t*) test_map, 15, 19);
    mu_assert("test_graph_create: error, graph == NULL", graph != NULL);
    mu_assert("test_graph_create: error, graph->nodes->size != 130", graph->nodes->size == 130);
    mu_assert("test_graph_create: error, graph->unique_edges != 134", graph->unique_edges == 134);
    
    nod = vector_get(graph->nodes, 13);
    ed0 =  vector_get(nod->edges, 0);
    ed1 =  vector_get(nod->edges, 1);
    ed2 =  vector_get(nod->edges, 2);
    arc0 = graph_arc_from(ed0, 13);
    arc1 = graph_arc_from(ed1, 13);
    arc2 = graph_arc_from(ed2, 13);

    mu_assert("test_graph_arc_from: wrong arc destination for from == 13", (arc0->destination + arc1->destination + arc2->destination) == 44);
    
    graph_destroy(graph);
    return 0;
}

static char * all_tests() {
    mu_run_test(test_graph_create);
    mu_run_test(test_graph_grid2nodeid);
	mu_run_test(test_graph_nodeid2grid);
    mu_run_test(test_graph_node_order);
    mu_run_test(test_graph_edge_remove_attach);
    mu_run_test(test_graph_arc_to);
    mu_run_test(test_graph_arc_from);

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
