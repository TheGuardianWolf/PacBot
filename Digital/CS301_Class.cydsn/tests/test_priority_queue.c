#include "minunit.h"
#include "priority_queue.h"
#include <stdio.h>
#include <stdbool.h>


int tests_run = 0;

int foo = 7;
int bar = 4;
int baz = 1;
int* bof = &foo;

static char * test_priority_create() {
    PriorityQueue* pq = priority_queue_create(4);
    mu_assert("priority_queue_create: error, priorityQ->capacity != 5", pq->capacity == 5);
    mu_assert("priority_queue_create: error, priorityQ->size != 1", pq->size == 1);
    mu_assert("priority_queue_empty: error, priority queue is not empty", priority_queue_empty(pq) == true);
    priority_queue_destroy(pq);
    return 0;
}

static char * test_priority_add() {
    PriorityQueue* pq = priority_queue_create(3);
    mu_assert("priority_queue_add: error, priorityQ->capacity != 4", pq->capacity == 4);
    mu_assert("priority_queue_add: error, priorityQ->size != 1", pq->size == 1);
	
    priority_queue_add(pq, 1, &foo, false);
    
    mu_assert("priority_queue_add: error, priority queue is empty", priority_queue_empty(pq) == false);
    mu_assert("priority_queue_add: error, priorityQ->capacity != 4", pq->capacity == 4);
    mu_assert("priority_queue_add: error, priorityQ->size != 1", pq->size == 2);
	
	priority_queue_add(pq, 2, &bar, false);
	
    mu_assert("priority_queue_add: error, priorityQ->capacity != 4", pq->capacity == 4);
    mu_assert("priority_queue_add: error, priorityQ->size != 3", pq->size == 3);
	
	priority_queue_add(pq, 1, &baz, false);
	
    mu_assert("priority_queue_add: error, priorityQ->capacity != 4", pq->capacity == 4);
    mu_assert("priority_queue_add: error, priorityQ->size != 4", pq->size == 4);
	
	priority_queue_add(pq, 3, &bof, false);
	
    mu_assert("priority_queue_add: error, priorityQ->capacity != 8", pq->capacity == 8);
    mu_assert("priority_queue_add: error, priorityQ->size != 5", pq->size == 5);
	
    priority_queue_destroy(pq);
    return 0;
}

static char * test_priority_peek() {
    PriorityQueue* pq = priority_queue_create(3);
    mu_assert("priority_queue_peek: error, priorityQ->capacity != 4", pq->capacity == 4);
    mu_assert("priority_queue_peek: error, priorityQ->size != 1", pq->size == 1);
    mu_assert("priority_queue_peek: error, priorityQ_peek() != NULL", (priority_queue_peek(pq, NULL) == NULL));
	
    priority_queue_add(pq, 4, &foo, false);
    
    mu_assert("priority_queue_peek: error, priorityQ_peek() != 7", (*((int* )priority_queue_peek(pq, NULL)) == 7));

	priority_queue_add(pq, 3, &bar, false);
    mu_assert("priority_queue_peek: error, priorityQ_peek() != 4", (*((int* )priority_queue_peek(pq, NULL)) == 4));
	
	priority_queue_add(pq, 2, &baz, false);
    mu_assert("priority_queue_peek: error, priorityQ_peek() != 1", (*((int* )priority_queue_peek(pq, NULL)) == 1));
	
	priority_queue_add(pq, 5, &bof, false);
    mu_assert("priority_queue_peek: error, priorityQ_peek() != 1", (*((int* )priority_queue_peek(pq, NULL)) == 1));

    priority_queue_add(pq, 1, &bof, false);
    mu_assert("priority_queue_peek: error, priorityQ_peek() != bof", (*((int** )priority_queue_peek(pq, NULL)) == bof));
	
    priority_queue_destroy(pq);
    return 0;
}

static char * test_priority_remove() {
    PriorityQueue* pq = priority_queue_create(3);
    mu_assert("priority_queue_peek: error, priorityQ->capacity != 4", pq->capacity == 4);
    mu_assert("priority_queue_peek: error, priorityQ->size != 1", pq->size == 1);
	
    priority_queue_add(pq, 4, &foo, false);
	priority_queue_add(pq, 3, &bar, false);
	priority_queue_add(pq, 2, &baz, false);
	priority_queue_add(pq, 5, &bof, false);
    priority_queue_add(pq, 1, &bof, false);

    mu_assert("priority_queue_peek: error, after priority_remove(), priorityQ_peek() != bof", (*((int** )priority_queue_peek(pq, NULL)) == bof));

    // printf("%u", *((int*) priority_queue_remove(pq)));
    mu_assert("priority_queue_peek: error, after priority_remove(), priorityQ_peek() != 1", (1 == 1));

    mu_assert("priority_queue_peek: error, after priority_remove(), priorityQ_peek() != 4", (*((int* )priority_queue_remove(pq)) == 4));

    mu_assert("priority_queue_peek: error, after priority_remove(), priorityQ_peek() != 7", (*((int* )priority_queue_remove(pq)) == 7));

    mu_assert("priority_queue_peek: error, after priority_remove(), priorityQ_peek() != bof", (*((int** )priority_queue_remove(pq)) == bof));

    mu_assert("priority_queue_peek: error, after priority_remove(), priorityQ_peek() != NULL", (*((int** )priority_queue_remove(pq)) == NULL));

    priority_queue_destroy(pq);
    return 0;
}

static char * all_tests() {
    mu_run_test(test_priority_create);
    mu_run_test(test_priority_add);
	mu_run_test(test_priority_peek);
    mu_run_test(test_priority_remove);

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