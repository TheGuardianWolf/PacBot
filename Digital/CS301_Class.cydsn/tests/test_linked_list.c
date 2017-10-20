#include "minunit.h"
#include "linked_list.h"
#include <stddef.h>
#include <stdio.h>

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
    linked_list_destroy(ll);
    return 0;
}

static char * test_linked_list_push() {
    LinkedList* ll = linked_list_create();
    mu_assert("test_linked_list_push: error, linkedlist->first != NULL", ll->first == NULL);
	mu_assert("test_linked_list_push: error, linkedlist->last != NULL", ll->last == NULL);
    mu_assert("test_linked_list_push: error, linkedlist->size != 0", ll->size == 0);
	
    linked_list_push(ll, &foo);
    mu_assert("test_linked_list_push: error, linkedlist->size != 1", ll->size == 1);
    mu_assert("test_linked_list_push: error, linkedlist->first != 7", (*((int*)ll->first->item) == 7));
    mu_assert("test_linked_list_push: error, linkedlist->last != 7", (*(int* )ll->last->item == 7));
	
	linked_list_push(ll, &bar);
	mu_assert("test_linked_list_push: error, linkedlist->size != 2", ll->size == 2);
    mu_assert("test_linked_list_push: error, linkedlist->first != 4", (*(int* )ll->first->item == 4));
    mu_assert("test_linked_list_push: error, linkedlist->last != 7", (*(int* )ll->last->item == 7));
	
	linked_list_push(ll, &bof);
	mu_assert("test_linked_list_push: error, linkedlist->size != 3", ll->size == 3);
    mu_assert("test_linked_list_push: error, linkedlist->first != bof", (*(int** )ll->first->item == bof));
    mu_assert("test_linked_list_push: error, linkedlist->last != 7", (*(int* )ll->last->item == 7));
	
    linked_list_destroy(ll);
    return 0;
}

static char * test_linked_list_pop() {
    LinkedList* ll = linked_list_create();
	
    linked_list_push(ll, &foo);
	linked_list_push(ll, &bar);
	linked_list_push(ll, &baz);
	linked_list_push(ll, &bof);	
	mu_assert("test_linked_list_pop: error, linkedlist->size != 4", ll->size == 4);

    mu_assert("test_linked_list_pop: error, linked_list_pop(ll) != &bof", linked_list_pop(ll) == &bof);
    mu_assert("test_linked_list_pop: error, linkedlist->size != 3", ll->size == 3);

    mu_assert("test_linked_list_pop: error, linked_list_pop(ll) == &baz", linked_list_pop(ll) == &baz);
    mu_assert("test_linked_list_pop: error, linkedlist->size != 2", ll->size == 2);

    mu_assert("test_linked_list_pop: error, linked_list_pop(ll) == &bar", linked_list_pop(ll) == &bar);
    mu_assert("test_linked_list_pop: error, linkedlist->size != 1", ll->size == 1);		
    
    mu_assert("test_linked_list_pop: error, linked_list_pop(ll) == &foo", linked_list_pop(ll) == &foo);
    mu_assert("test_linked_list_pop: error, linkedlist->size != 0", ll->size == 0);
    
    mu_assert("test_linked_list_pop: error, linkedlist->first != NULL", (ll->first == NULL));
    mu_assert("test_linked_list_pop: error, linkedlist->last != NULL", (ll->last == NULL));
	
	
    linked_list_destroy(ll);
    return 0;
}

static char * test_linked_list_add() {
    LinkedList* ll = linked_list_create();
    mu_assert("test_linked_list_add: error, linkedlist->first != NULL", ll->first == NULL);
	mu_assert("test_linked_list_add: error, linkedlist->last != NULL", ll->last == NULL);
    mu_assert("test_linked_list_add: error, linkedlist->size != 0", ll->size == 0);
	
    linked_list_add(ll, &foo);
	mu_assert("test_linked_list_add: error, linkedlist->size != 1", ll->size == 1);
    mu_assert("test_linked_list_add: error, linkedlist->first != 7", (*(int* )ll->first->item == 7));
    mu_assert("test_linked_list_add: error, linkedlist->last != 7", (*(int* )ll->last->item == 7));
	
	linked_list_add(ll, &bar);
	mu_assert("test_linked_list_add: error, linkedlist->size != 2", ll->size == 2);
    mu_assert("test_linked_list_add: error, linkedlist->first != 7", (*(int* )ll->first->item == 7));
    mu_assert("test_linked_list_add: error, linkedlist->last != 4", (*(int* )ll->last->item == 4));
	
	linked_list_add(ll, &bof);
	mu_assert("test_linked_list_add: error, linkedlist->size != 3", ll->size == 3);
    mu_assert("test_linked_list_add: error, linkedlist->first != 7", (*(int* )ll->first->item == 7));
    mu_assert("test_linked_list_add: error, linkedlist->last != bof", (*(int** )ll->last->item == bof));
	
    linked_list_destroy(ll);
    return 0;
}

static char * test_linked_list_remove() {
    LinkedList* ll = linked_list_create();
	
    linked_list_add(ll, &foo);
	linked_list_add(ll, &bar);
	linked_list_add(ll, &baz);
	linked_list_add(ll, &bof);	
	mu_assert("test_linked_list_remove: error, linkedlist->size != 4", ll->size == 4);

    mu_assert("test_linked_list_remove: error, linked_list_remove(ll) != &foo", linked_list_remove(ll) == &foo);
    mu_assert("test_linked_list_remove: error, linkedlist->size != 3", ll->size == 3);

    mu_assert("test_linked_list_remove: error, linked_list_remove(ll) == &bar", linked_list_remove(ll) == &bar);
    mu_assert("test_linked_list_remove: error, linkedlist->size != 2", ll->size == 2);

    mu_assert("test_linked_list_remove: error, linked_list_remove(ll) == &baz", linked_list_remove(ll) == &baz);
    mu_assert("test_linked_list_remove: error, linkedlist->size != 1", ll->size == 1);		
    
    mu_assert("test_linked_list_remove: error, linked_list_remove(ll) == &bof", linked_list_remove(ll) == &bof);
    mu_assert("test_linked_list_remove: error, linkedlist->size != 0", ll->size == 0);
    
    mu_assert("test_linked_list_remove: error, linkedlist->first != NULL", (ll->first == NULL));
    mu_assert("test_linked_list_remove: error, linkedlist->last != NULL", (ll->last == NULL));
	
	
    linked_list_destroy(ll);
    return 0;
}

static char * test_linked_list_peek() {
    LinkedList* ll = linked_list_create();
    mu_assert("test_linked_list_add: error, linked_list_peek != NULL", linked_list_peek(ll) == NULL);
    
    linked_list_add(ll, &foo);
	mu_assert("test_linked_list_add: error, linked_list_peek != 7", *((int* )linked_list_peek(ll)) == 7);

	
	linked_list_add(ll, &bar);
	mu_assert("test_linked_list_add: error, linked_list_peek != 4", *((int* )linked_list_peek(ll)) == 7);

    linked_list_remove(ll);
    linked_list_remove(ll);

    linked_list_push(ll, &foo);
    mu_assert("test_linked_list_add: error, linked_list_peek !=7", *((int* )linked_list_peek(ll)) == 7);
    linked_list_push(ll, &bar);
    mu_assert("test_linked_list_add: error, linked_list_peek !=7", *((int* )linked_list_peek(ll)) == 4);
    
    linked_list_destroy(ll);
    return 0;
}

static char * all_tests() {
    mu_run_test(test_linked_list_create);
    mu_run_test(test_linked_list_push);
    mu_run_test(test_linked_list_pop);
    mu_run_test(test_linked_list_add);
    mu_run_test(test_linked_list_peek);
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
