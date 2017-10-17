#include "minunit.h"
#include "priority_queue.h"


int tests_run = 0;

int foo = 7;
int bar = 4;
int baz = 1;
int* bof = &foo;

static char * test_priority_create() {
    PriorityQueue* pq = priority_queue_create(4);
    mu_assert("priority_queue_create: error, priorityQ->capacity != 4", pq->capacity == 4);
    mu_assert("priority_queue_create: error, priorityQ->size != 0", pq->size == 0);
    priority_queue_destroy(pq);
    return 0;
}

static char * test_priority_add() {
    PriorityQueue* pq = priority_queue_create(3);
    mu_assert("priority_queue_add: error, priorityQ->capacity != 3", pq->capacity == 3);
    mu_assert("priority_queue_add: error, priorityQ->size != 0", pq->size == 0);
	
    priority_queue_add(pq, 1, &foo);
	
    mu_assert("priority_queue_add: error, priorityQ->capacity != 3", pq->capacity == 3);
    mu_assert("priority_queue_add: error, priorityQ->size != 1", pq->size == 1);
	
	priority_queue_add(pq, 2, &bar);
	
    mu_assert("priority_queue_add: error, priorityQ->capacity != 3", pq->capacity == 3);
    mu_assert("priority_queue_add: error, priorityQ->size != 1", pq->size == 2)
	
	priority_queue_add(pq, 1, &baz);
	
    mu_assert("priority_queue_add: error, priorityQ->capacity != 3", pq->capacity == 3);
    mu_assert("priority_queue_add: error, priorityQ->size != 1", pq->size == 3)
	
	priority_queue_add(pq, 3, &bof);
	
    mu_assert("priority_queue_add: error, priorityQ->capacity != 3", pq->capacity == 4);
    mu_assert("priority_queue_add: error, priorityQ->size != 1", pq->size == 4)
	
    priority_queue_destroy(pq);
    return 0;
}

static char * test_priority_peek() {
    PriorityQueue* pq = priority_queue_create(3);
    mu_assert("priority_queue_peek: error, priorityQ->capacity != 3", pq->capacity == 3);
    mu_assert("priority_queue_peek: error, priorityQ->size != 0", pq->size == 0);
    mu_assert("priority_queue_peek: error, priorityQ_peek() != 1", (priority_queue_peek(pq) == NULL);
	
    priority_queue_add(pq, 4, &foo);
    mu_assert("priority_queue_peek: error, priorityQ_peek() != 1", (*((int* )priority_queue_peek(pq)) == 7);

	priority_queue_add(pq, 3, &bar);
    mu_assert("priority_queue_peek: error, priorityQ_peek() != 1", (*((int* )priority_queue_peek(pq)) == 7);
	
	priority_queue_add(pq, 2, &baz);
    mu_assert("priority_queue_peek: error, priorityQ_peek() != 1", (*((int* )priority_queue_peek(pq)) == 1);
	
	priority_queue_add(pq, 5, &bof);
    mu_assert("priority_queue_peek: error, priorityQ_peek() != 1", (*((int** )priority_queue_peek(pq)) == 1);

    priority_queue_add(pq, 1, &bof);
    mu_assert("priority_queue_peek: error, priorityQ_peek() != bof", (*((int** )priority_queue_peek(pq)) == bof);
	
    priority_queue_destroy(pq);
    return 0;
}

static char * test_priority_remove() {
    PriorityQueue* pq = priority_queue_create(3);
    mu_assert("priority_queue_peek: error, priorityQ->capacity != 3", pq->capacity == 3);
    mu_assert("priority_queue_peek: error, priorityQ->size != 0", pq->size == 0);
	
    priority_queue_add(pq, 4, &foo);
	priority_queue_add(pq, 3, &bar);
	priority_queue_add(pq, 2, &baz);
	priority_queue_add(pq, 5, &bof);
    priority_queue_add(pq, 1, &bof);
    mu_assert("priority_queue_peek: error, after priority_remove(), priorityQ_peek() != bof", (*((int** )priority_queue_peek(pq)) == bof);

    priority_queue_remove();
    mu_assert("priority_queue_peek: error, after priority_remove(), priorityQ_peek() != 1", (*((int* )priority_queue_peek(pq)) == 1);

    priority_queue_remove();
    mu_assert("priority_queue_peek: error, after priority_remove(), priorityQ_peek() != 4", (*((int* )priority_queue_peek(pq)) == 4);

    priority_queue_remove();
    mu_assert("priority_queue_peek: error, after priority_remove(), priorityQ_peek() != 7", (*((int* )priority_queue_peek(pq)) == 7);

    priority_queue_remove();
    mu_assert("priority_queue_peek: error, after priority_remove(), priorityQ_peek() != bof", (*((int** )priority_queue_peek(pq)) == bof);

    priority_queue_remove();
    mu_assert("priority_queue_peek: error, after priority_remove(), priorityQ_peek() != NULL", (*((int** )priority_queue_peek(pq)) == NULL);

	
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