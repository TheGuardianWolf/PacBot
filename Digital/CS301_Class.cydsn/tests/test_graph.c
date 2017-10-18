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
	
    Graph* gra = graph_create(test_map , 15, 19);
    mu_assert("test_graph_create: error, graph == NULL", gra != NULL);
    mu_assert("test_graph_create: error, graph->nodes->size != 130", graph->nodes->size == 130);
    mu_assert("test_graph_create: error, graph->unique_edges != 104", graph->unique_edges == 104);
    graph_destroy(gra);
    return 0;
}

static char * test_graph_grid2nodeid() {
	
    uint8_t test_map PACMAN_MAP;
    point_uint8_t point;
	
    Graph* gra = graph_create(&test_map , 15, 19);
    mu_assert("test_graph_grid2nodeid: error, graph == NULL", gra != NULL);
    mu_assert("test_graph_grid2nodeid: error, graph->nodes->size != 130", graph->nodes->size == 130);
    mu_assert("test_graph_grid2nodeid: error, graph->unique_edges != 104", graph->unique_edges == 104);

    point->x = 1;
    point->y = 1;
    mu_assert("test_graph_grid2nodeid: error, nodeid != 1", graph_grid2nodeid(gra, point) == 0);

    point->x = 0;
    point->y = 0;
    mu_assert("test_graph_grid2nodeid: error, nodeid != NODE_INVALID1", graph_grid2nodeid(gra, point) == NODE_INVALID);

    point->x = 6;
    point->y = 5;
    mu_assert("test_graph_grid2nodeid: error, nodeid != NODE_INVALID2", graph_grid2nodeid(gra, point) == NODE_INVALID);

    point->x = 17;
    point->y = 13;
    mu_assert("test_graph_grid2nodeid: error, nodeid != 1", graph_grid2nodeid(gra, point) == 129);

    point->x = 15;
    point->y = 19;
    mu_assert("test_graph_grid2nodeid: error, nodeid != NODE_INVALID3", graph_grid2nodeid(gra, point) == NODE_INVALID);

    point->x = 11;
    point->y = 6;
    mu_assert("test_graph_grid2nodeid: error, nodeid != 1", graph_grid2nodeid(gra, point) == 57);
    

    graph_destroy(gra);
    return 0;
}


static char * test_graph_nodeid2grid() {
	
    uint8_t test_map PACMAN_MAP;
    point_uint8_t point;
	
    Graph* gra = graph_create(&test_map , 15, 19);
    mu_assert("test_graph_nodeid2grid: error, graph == NULL", gra != NULL);
    mu_assert("test_graph_nodeid2grid: error, graph->nodes->size != 130", graph->nodes->size == 130);
    mu_assert("test_graph_nodeid2grid: error, graph->unique_edges != 104", graph->unique_edges == 104);

    point_uint8_t point;
    point = graph_nodeid2grid(gra, 0);
    mu_assert("test_graph_nodeid2grid: error, x!=1", point->x == 1);
    mu_assert("test_graph_nodeid2grid: error, y!=1", point->y == 1);

    point_uint8_t point;
    point = graph_nodeid2grid(gra, 129);
    mu_assert("test_graph_nodeid2grid: error, x != 17", point->x == 17);
    mu_assert("test_graph_nodeid2grid: error, y != 13", point->y == 13);

    point_uint8_t point;
    point = graph_nodeid2grid(gra, 130);
    mu_assert("test_graph_nodeid2grid: error, nodeid != 1", point->x == NODE_INVALID);
    mu_assert("test_graph_nodeid2grid: error, nodeid != 1", point->y == NODE_INVALID);

    point_uint8_t point;
    point = graph_nodeid2grid(gra, 4);
    mu_assert("test_graph_nodeid2grid: error, nodeid != 5", point->x == 5);
    mu_assert("test_graph_nodeid2grid: error, nodeid != 1", point->y == 1);

    point_uint8_t point;
    point = graph_nodeid2grid(gra, 5);
    mu_assert("test_graph_nodeid2grid: error, nodeid != 7", point->x == 7);
    mu_assert("test_graph_nodeid2grid: error, nodeid != 1", point->y == 1);

    graph_destroy(gra);
    return 0;
}

static char * test_graph_node_order() {
	
    uint8_t test_map PACMAN_MAP;
    int i = 0;
    int j = 0;
	
    Graph* gra = graph_create(&test_map , 15, 19);
    mu_assert("test_graph_node_order: error, graph == NULL", gra != NULL);
    mu_assert("test_graph_node_order: error, graph->nodes->size != 130", graph->nodes->size == 130);
    mu_assert("test_graph_node_order: error, graph->unique_edges != 104", graph->unique_edges == 104);

    mu_assert("test_graph_node_order: error, order != 1", graph_node_order(gra, 0) == 1);
    mu_assert("test_graph_node_order: error, order != 2", graph_node_order(gra, 0) == 2);
    mu_assert("test_graph_node_order: error, order != 2", graph_node_order(gra, 0) == 2);
    mu_assert("test_graph_node_order: error, order != 3", graph_node_order(gra, 13) == 3);
    mu_assert("test_graph_node_order: error, order != 3", graph_node_order(gra, 123) == 3);
    mu_assert("test_graph_node_order: error, order != 0", graph_node_order(gra, 150) == 0);
    mu_assert("test_graph_node_order: error, order != 0", graph_node_order(gra, 130) == 0);

    graph_destroy(gra);
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
	
    Graph* gra = graph_create(&test_map , 15, 19);
    mu_assert("test_graph_edge_remove: error, graph == NULL", gra != NULL);
    mu_assert("test_graph_edge_remove: error, graph->nodes->size != 130", graph->nodes->size == 130);
    mu_assert("test_graph_edge_remove: error, graph->unique_edges != 104", graph->unique_edges == 104);

    nod = vector_get(gra->nodes, 13);
    ed =  vector_get(nod->edges, 1);
    arc = graph_arc_from(ed, 13);

    graph_edge_remove(Graph* graph, GraphEdge* edge);
    if (arc->heading == 2) {
        head = 2;
        mu_assert("test_graph_edge_remove: error, order != 2", graph_node_order(gra, 13) == 2);
        mu_assert("test_graph_edge_remove: error, order != 1", graph_node_order(gra, 18) == 1);
    }
    else if (arc->heading == 1) {
        head = 1;
        mu_assert("test_graph_edge_remove: error, order != 2", graph_node_order(gra, 13) == 2);
        mu_assert("test_graph_edge_remove: error, order != 1", graph_node_order(gra, 14) == 1);
    }
    else if (arc->heading == 3) {
        head = 3;
        mu_assert("test_graph_edge_remove: error, order != 2", graph_node_order(gra, 13) == 2);
        mu_assert("test_graph_edge_remove: error, order != 1", graph_node_order(gra, 12) == 1);
    }

    mu_assert("test_graph_edge_remove: error, detatched_edges size != 1", gra->detatched_edges->size == 1);

    for (i = 0; i < gra->detatched_edges->size; i++) {
        if(vector_get(gra->detatched_edges, i) == ed) {
            j = 1;
        }
    }

    mu_assert("test_graph_edge_remove: error, removed edge not in detatched_edges", j == 1);

    mu_assert("test_graph_edge_remove: error, edge not removed", nod->edges->size == 2);

    for (i = 0; i < nod->edges->size; i++) {
        mu_assert("test_graph_edge_remove: error, edges array still contains removed edge", vector_get(nod->edges, i) != ed;
    }

    //how do you know which edge you want to attach --------------------below is attach
    graph_edge_attach(gra, vector_get(gra->detatched_edges, 0);

    if (arc->head == 2) {
        mu_assert("test_graph_edge_attach: error, order != 3", graph_node_order(gra, 13) == 3);
        mu_assert("test_graph_edge_attach: error, order != 2", graph_node_order(gra, 18) == 2);
    }
    else if (arc->head == 1) {
        mu_assert("test_graph_edge_attach: error, order != 3", graph_node_order(gra, 13) == 3);
        mu_assert("test_graph_edge_attach: error, order != 2", graph_node_order(gra, 14) == 2);
    }
    else if (arc->head == 3) {
        mu_assert("test_graph_edge_attach: error, order != 3", graph_node_order(gra, 13) == 3);
        mu_assert("test_graph_edge_attach: error, order != 2", graph_node_order(gra, 12) == 2);
    }

    mu_assert("test_graph_edge_attach: error, detatched_edges size != 0", gra->detatched_edges->size == 0);

    mu_assert("test_graph_edge_attach: error, edge not attached", nod->edges->size == 3);

    graph_destroy(gra);
    return 0;
}

static char * test_graph_arc_to() {
	
    uint8_t test_map PACMAN_MAP;
    GraphEdge* ed0,ed1,ed2;
    GraphNode* nod;
    GraphArc* arc0,arc1,arc2;
	
    Graph* gra = graph_create(&test_map , 15, 19);
    mu_assert("test_graph_create: error, graph == NULL", gra != NULL);
    mu_assert("test_graph_create: error, graph->nodes->size != 130", graph->nodes->size == 130);
    mu_assert("test_graph_create: error, graph->unique_edges != 104", graph->unique_edges == 104);
    
    nod = vector_get(gra->nodes, 13);
    ed0 =  vector_get(nod->edges, 0);
    ed1 =  vector_get(nod->edges, 1);
    ed2 =  vector_get(nod->edges, 2);
    arc0 = graph_arc_to(ed0, 13);
    arc1 = graph_arc_to(ed1, 13);
    arc2 = graph_arc_to(ed2, 13);

    mu_assert("test_graph_arc_to: arc0->destination == 13", arc0->destination == 13);
    mu_assert("test_graph_arc_to: arc1->destination == 13", arc1->destination == 13);
    mu_assert("test_graph_arc_to: arc2->destination == 13", arc2->destination == 13);
    
    graph_destroy(gra);
    return 0;
}

static char * test_graph_arc_from() {
    uint8_t test_map PACMAN_MAP;
    GraphEdge* ed0,ed1,ed2;
    GraphNode* nod;
    GraphArc* arc0,arc1,arc2;
	
    Graph* gra = graph_create(&test_map , 15, 19);
    mu_assert("test_graph_create: error, graph == NULL", gra != NULL);
    mu_assert("test_graph_create: error, graph->nodes->size != 130", graph->nodes->size == 130);
    mu_assert("test_graph_create: error, graph->unique_edges != 104", graph->unique_edges == 104);
    
    nod = vector_get(gra->nodes, 13);
    ed0 =  vector_get(nod->edges, 0);
    ed1 =  vector_get(nod->edges, 1);
    ed2 =  vector_get(nod->edges, 2);
    arc0 = graph_arc_from(ed0, 13);
    arc1 = graph_arc_from(ed1, 13);
    arc2 = graph_arc_from(ed2, 13);

    mu_assert("test_graph_arc_from: wrong arc destination for from == 13", (arc0->destination + arc1->destination + arc2->destination) == 44);
    
    graph_destroy(gra);
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
