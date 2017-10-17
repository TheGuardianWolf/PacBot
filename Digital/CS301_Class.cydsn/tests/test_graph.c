#include "minunit.h"
#include "graph.h"
#include <stdio.h>





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
