#include <stdio.h>
#include "minunit.h"
#include "vector.h"

int tests_run = 0;

int foo = 7;
int bar = 4;
int baz = 1;
int* bof = &foo;

static char * test_vector_create() {
    Vector* vec = vector_create(4);
    mu_assert("test_vector_create: error, vector->capacity != 4", vec->capacity == 4);
    mu_assert("test_vector_create: error, vector->size != 0", vec->size == 0);
    vector_destroy(vec);
    return 0;
}

static char * test_vector_append() {
    Vector* vec = vector_create(2);
    mu_assert("test_vector_append: error, vector->capacity != 2", vec->capacity == 2);
    mu_assert("test_vector_append: error, vector->size != 0", vec->size == 0);
    vector_append(vec, &foo);
    vector_append(vec, &bar);
    vector_append(vec, &baz);
    vector_append(vec, &bof);
    mu_assert("test_vector_append: error, vector->capacity != 4", vec->capacity == 4);
    mu_assert("test_vector_append: error, vector->size != 4", vec->size == 4);
    vector_destroy(vec);
    return 0;
}

static char * test_vector_get() {
    Vector* vec = vector_create(4);
    vector_append(vec, &foo);
    vector_append(vec, &bar);
    vector_append(vec, &baz);
    vector_append(vec, &bof);
    mu_assert("test_vector_get: error, *vector[0] != 7", (*((int*) vector_get(vec, 0)) == 7));
    mu_assert("test_vector_get: error, *vector[1] != 4", (*((int*) vector_get(vec, 1)) == 4));
    mu_assert("test_vector_get: error, *vector[2] != 1", (*((int*) vector_get(vec, 2)) == 1));
    mu_assert("test_vector_get: error, *vector[3] != bof", (*((int**) vector_get(vec, 3)) == bof));
    vector_destroy(vec);
    return 0;
}

static char * all_tests() {
    mu_run_test(test_vector_create);
    mu_run_test(test_vector_append);
    mu_run_test(test_vector_get);
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
