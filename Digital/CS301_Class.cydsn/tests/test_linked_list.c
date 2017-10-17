#include "minunit.h"
#include "linked_list.h"

int tests_run = 0;

int foo = 7;
int bar = 4;
int baz = 1;
int* bof = &foo;

static char * test_linked_list_create() {
    LinkedList* ll = linked_list_create();
    mu_assert("test_vector_create: error, linkedlist->first != NULL", ll->first == NULL);
	mu_assert("test_vector_create: error, linkedlist->last != NULL", ll->last == NULL);
    mu_assert("test_vector_create: error, linkedlist->size != 0", ll->size == 0);
    vector_destroy(vec);
    return 0;
}

static char * test_linked_list_push() {
    LinkedList* ll = linked_list_create();
    mu_assert("test_vector_create: error, linkedlist->first != NULL", ll->first == NULL);
	mu_assert("test_vector_create: error, linkedlist->last != NULL", ll->last == NULL);
    mu_assert("test_vector_create: error, linkedlist->size != 0", ll->size == 0);
	
    linked_list_push(vec, &foo);
	mu_assert("test_vector_create: error, linkedlist->size != 1", ll->size == 1);
    mu_assert("test_vector_append: error, linkedlist->first != 7", (*(int* )ll->first == 7);
    mu_assert("test_vector_append: error, linkedlist->last != 7", (*(int* )ll->last == 7);
	
	linked_list_push(vec, &bar);
	mu_assert("test_vector_create: error, linkedlist->size != 2", ll->size == 2);
    mu_assert("test_vector_append: error, linkedlist->first != 4", (*(int* )ll->first == 4);
    mu_assert("test_vector_append: error, linkedlist->last != 7", (*(int* )ll->last == 7);
	
	linked_list_push(vec, &bof);
	mu_assert("test_vector_create: error, linkedlist->size != 3", ll->size == 3);
    mu_assert("test_vector_append: error, linkedlist->first != bof", (*(int** )ll->first == bof);
    mu_assert("test_vector_append: error, linkedlist->last != 7", (*(int* )ll->last == 7);
	
    vector_destroy(vec);
    return 0;
}

static char * test_linked_list_pop() {
    LinkedList* ll = linked_list_create();
    mu_assert("test_vector_create: error, linkedlist->first != NULL", ll->first == NULL);
	mu_assert("test_vector_create: error, linkedlist->last != NULL", ll->last == NULL);
    mu_assert("test_vector_create: error, linkedlist->size != 0", ll->size == 0);
	
    linked_list_push(vec, &foo);
	linked_list_push(vec, &bar);
	linked_list_push(vec, &baz);
	linked_list_push(vec, &bof);	
	mu_assert("test_vector_create: error, linkedlist->size != 4", ll->size == 4);
    mu_assert("test_vector_append: error, linkedlist->first != 7", (*(int* )ll->first == 7);
    mu_assert("test_vector_append: error, linkedlist->last != bof", (*(int** )ll->last == bof);
	
	linked_list_pop(vec);
	mu_assert("test_vector_create: error, linkedlist->size != 3", ll->size == 3);
    mu_assert("test_vector_append: error, linkedlist->first != 4", (*(int* )ll->first == 4);
    mu_assert("test_vector_append: error, linkedlist->last != bof", (*(int** )ll->last == bof);
		
	linked_list_pop(vec);
	mu_assert("test_vector_create: error, linkedlist->size != 2", ll->size == 2);
    mu_assert("test_vector_append: error, linkedlist->first != 1", (*(int* )ll->first == 1);
    mu_assert("test_vector_append: error, linkedlist->last != bof", (*(int** )ll->last == bof);
			
	linked_list_pop(vec);
	mu_assert("test_vector_create: error, linkedlist->size != 1", ll->size == 1);
    mu_assert("test_vector_append: error, linkedlist->last != bof", (*(int** )ll->last == bof);
    mu_assert("test_vector_append: error, linkedlist->last != bof", (*(int** )ll->last == bof);
				
	linked_list_pop(vec);
	mu_assert("test_vector_create: error, linkedlist->size != 0", ll->size == 0);
    mu_assert("test_vector_append: error, linkedlist->first != NULL", (*(int* )ll->first == NULL);
    mu_assert("test_vector_append: error, linkedlist->last != NULL", (*(int* )ll->last == NULL);
	
	
    vector_destroy(vec);
    return 0;
}

static char * test_linked_list_add() {
    LinkedList* ll = linked_list_create();
    mu_assert("test_vector_create: error, linkedlist->first != NULL", ll->first == NULL);
	mu_assert("test_vector_create: error, linkedlist->last != NULL", ll->last == NULL);
    mu_assert("test_vector_create: error, linkedlist->size != 0", ll->size == 0);
	
    linked_list_add(vec, &foo);
	mu_assert("test_vector_create: error, linkedlist->size != 1", ll->size == 1);
    mu_assert("test_vector_append: error, linkedlist->first != 7", (*(int* )ll->first == 7);
    mu_assert("test_vector_append: error, linkedlist->last != 7", (*(int* )ll->last == 7);
	
	linked_list_add(vec, &bar);
	mu_assert("test_vector_create: error, linkedlist->size != 2", ll->size == 2);
    mu_assert("test_vector_append: error, linkedlist->first != 7", (*(int* )ll->first == 7);
    mu_assert("test_vector_append: error, linkedlist->last != 4", (*(int* )ll->last == 4);
	
	linked_list_add(vec, &bof);
	mu_assert("test_vector_create: error, linkedlist->size != 3", ll->size == 3);
    mu_assert("test_vector_append: error, linkedlist->first != 7", (*(int* )ll->first == 7);
    mu_assert("test_vector_append: error, linkedlist->last != bof", (*(int** )ll->last == bof);
	
    vector_destroy(vec);
    return 0;
}

static char * all_tests() {
    mu_run_test(test_linked_list_create);
    mu_run_test(test_linked_list_push);
    mu_run_test(test_linked_list_pop);
	mu_run_test(test_linked_list_add);
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