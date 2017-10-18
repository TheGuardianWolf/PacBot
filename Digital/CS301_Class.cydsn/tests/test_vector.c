#include <stdio.h>
#include "minunit.h"
#include "vector.h"
#include <stddef.h>

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
    vector_append(vec, &baz);
    mu_assert("test_vector_append: error, vector->capacity != 4", vec->capacity == 8);
    mu_assert("test_vector_append: error, vector->size != 4", vec->size == 5);
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


static char * test_vector_insert() {
    Vector* vec = vector_create(3);
    vector_append(vec, &foo);
    vector_append(vec, &bar);
    vector_insert(vec, 1, &baz);
	mu_assert("test_vector_insert: error, *vector[0] != 7", (*((int*) vector_get(vec, 0)) == 7));
    mu_assert("test_vector_insert: error, *vector[1] != 1", (*((int*) vector_get(vec, 1)) == 1));
	mu_assert("test_vector_insert: error, *vector[2] != 4", (*((int*) vector_get(vec, 2)) == 4));
	
    vector_insert(vec, 2 ,&bof);
    mu_assert("test_vector_insert: error, *vector[0] != 7", (*((int*) vector_get(vec, 0)) == 7));
    mu_assert("test_vector_insert: error, *vector[1] != 1", (*((int*) vector_get(vec, 1)) == 1));
    mu_assert("test_vector_insert: error, *vector[2] != bof", (*((int**) vector_get(vec, 2)) == bof));
    mu_assert("test_vector_insert: error, *vector[3] != 4", (*((int*) vector_get(vec, 3)) == 4));
    vector_destroy(vec);
    return 0;
}

static char * test_vector_remove() {
    Vector* vec = vector_create(4);
    vector_append(vec, &foo);
    vector_append(vec, &bar);
    vector_append(vec, &baz);
    vector_append(vec, &bof);
	
	mu_assert("test_vector_remove: error, should return NULL", vector_remove(vec, 4) == NULL);
	mu_assert("test_vector_remove: error, *vector[0] != 7", (*((int*) vector_get(vec, 0))) == 7);
    mu_assert("test_vector_remove: error, *vector[1] != 4", (*((int*) vector_get(vec, 1))) == 4);
    mu_assert("test_vector_remove: error, *vector[2] != 1", (*((int*) vector_get(vec, 2))) == 1);
    mu_assert("test_vector_remove: error, *vector[3] != bof", (*((int**) vector_get(vec, 3))) == bof);
	mu_assert("test_vector_remove: error, vector->size != 4", vec->size == 4);
	vector_remove(vec, 1);
    mu_assert("test_vector_remove: error, *vector[0] != 7", (*((int*) vector_get(vec, 0))) == 7);
    mu_assert("test_vector_remove: error, *vector[1] != 1", (*((int*) vector_get(vec, 1))) == 1);
    mu_assert("test_vector_remove: error, *vector[2] != bof", (*((int**) vector_get(vec, 2))) == bof);
	
	vector_remove(vec, 1);
    mu_assert("test_vector_remove: error, *vector[0] != 7", (*((int*) vector_get(vec, 0))) == 7);
    mu_assert("test_vector_remove: error, *vector[1] != bof", (*((int**) vector_get(vec, 1)) == bof));
	
	vector_remove(vec, 0);
    mu_assert("test_vector_remove: error, *vector[0] != bof", (*((int**) vector_get(vec, 0)) == bof));
	
    mu_assert("test_vector_remove: error, *vector[0] != bof", (*((int**) vector_remove(vec, 0)) == bof));

    mu_assert("test_vector_remove: error, should return NULL", ((int*) vector_remove(vec, 0)) == NULL);
    mu_assert("test_vector_remove: error, vector->size != 0", vec->size == 0);
    
    vector_destroy(vec);
    return 0;
}

static char * test_vector_set() {
    Vector* vec = vector_create(4);
    vector_append(vec, &foo);
    vector_append(vec, &bar);
    vector_append(vec, &baz);
    vector_append(vec, &bof);
	
	mu_assert("test_vector_set: error, *vector[0] != 7", (*((int*) vector_get(vec, 0)) == 7));
    mu_assert("test_vector_set: error, *vector[1] != 4", (*((int*) vector_get(vec, 1)) == 4));
    mu_assert("test_vector_set: error, *vector[2] != 1", (*((int*) vector_get(vec, 2)) == 1));
    mu_assert("test_vector_set: error, *vector[3] != bof", (*((int**) vector_get(vec, 3)) == bof));
	
	vector_set(vec, 2,&bar);
	
	mu_assert("test_vector_set: error, *vector[0] != 7", (*((int*) vector_get(vec, 0)) == 7));
    mu_assert("test_vector_set: error, *vector[1] != 4", (*((int*) vector_get(vec, 1)) == 4));
    mu_assert("test_vector_set: error, *vector[2] != 4", (*((int*) vector_get(vec, 2)) == 4));
    mu_assert("test_vector_set: error, *vector[3] != bof", (*((int**) vector_get(vec, 3)) == bof));
	
	vector_set(vec, 0,&bar);
	
	mu_assert("test_vector_set: error, *vector[0] != 4", (*((int*) vector_get(vec, 0)) == 4));
    mu_assert("test_vector_set: error, *vector[1] != 4", (*((int*) vector_get(vec, 1)) == 4));
    mu_assert("test_vector_set: error, *vector[2] != 4", (*((int*) vector_get(vec, 2)) == 4));
    mu_assert("test_vector_set: error, *vector[3] != bof", (*((int**) vector_get(vec, 3)) == bof));
	
	vector_set(vec, 0,&bof);
	
	mu_assert("test_vector_set: error, *vector[0] != bof", (*((int**) vector_get(vec, 0)) == bof));
    mu_assert("test_vector_set: error, *vector[1] != 4", (*((int*) vector_get(vec, 1)) == 4));
    mu_assert("test_vector_set: error, *vector[2] != 4", (*((int*) vector_get(vec, 2)) == 4));
    mu_assert("test_vector_set: error, *vector[3] != bof", (*((int**) vector_get(vec, 3)) == bof));

    vector_destroy(vec);
    return 0;
}


static char * all_tests() {
    mu_run_test(test_vector_create);
    mu_run_test(test_vector_append);
    mu_run_test(test_vector_get);
	mu_run_test(test_vector_insert);
	mu_run_test(test_vector_remove);
	mu_run_test(test_vector_set);
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
