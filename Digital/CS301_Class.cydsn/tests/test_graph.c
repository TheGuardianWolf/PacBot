#include <stdio.h>
#include "minunit.h"
#include "vector.h"
#include <stddef.h>

int tests_run = 0;

int foo = 7;
int bar = 4;
int baz = 1;
int* bof = &foo;

static char * test_graph_create() {
	uint8_t grid[5][5];
    Graph* gra = graph_create(grid , 10, 10);
    mu_assert("test_vector_create: error, vector->capacity != 4", vec->capacity == 4);
    mu_assert("test_vector_create: error, vector->size != 0", vec->size == 0);
    vector_destroy(vec);
    return 0;
}


static char * all_tests() {
    mu_run_test(test_graph_create);
    mu_run_test(test_graph_add);
	mu_run_test(test_graph_peek);
    mu_run_test(test_graph_remove);

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
