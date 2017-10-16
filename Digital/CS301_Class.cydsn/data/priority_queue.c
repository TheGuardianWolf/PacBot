#include "priority_queue.h"
#include <stdlib.h>
#include <stddef.h>

static void perc_up(PriorityQueue* queue, size_t index) {
    HeapNode* a;
    HeapNode* b;
    size_t fd = index / 2;
    while (fd > 0) {
        a = vector_get((Vector*) queue, index);
        b = vector_get((Vector*) queue, fd);
        if (a != NULL && b != NULL) {
            if (a->priority < b->priority) {
                vector_set((Vector*) queue, fd, a);
                vector_set((Vector*) queue, index, b);
            }
        }
        fd /= 2;
    }    
}

static size_t min_child(PriorityQueue* queue, size_t index) {
    size_t child_index;
    size_t index_x2 = index * 2;
    size_t index_x2p1 = index * 2 + 1;

    if (index_x2p1 > queue->size) {
        child_index = index_x2;
    }
    else {
        HeapNode* a;
        HeapNode* b;
        a = vector_get((Vector*) queue, index_x2);
        b = vector_get((Vector*) queue, index_x2p1);
        if (a != NULL && b != NULL) {
            if (a->priority < b->priority) {
                child_index = index_x2;
            }
            else {
                child_index = index_x2p1;
            }
        }
    }

    return child_index;
}

static void perc_down(PriorityQueue* queue, size_t index) {
    HeapNode* a;
    HeapNode* b;
    size_t mc;

    while (index * 2 <= queue->size) {
        mc = min_child(queue, index);
        a = vector_get((Vector*) queue, index);
        b = vector_get((Vector*) queue, mc);
        if (a != NULL && b != NULL) {
            if (a->priority > b->priority) {
                vector_set((Vector*) queue, index, b);
                vector_set((Vector*) queue, mc, a);
            }
        }
        index = mc;
    }  
}

PriorityQueue* priority_queue_create(size_t initial_capacity) {
    PriorityQueue* queue = malloc(sizeof(PriorityQueue));
    if (queue != NULL) {
        // Spare block at front of vector for easier addressing
        queue = vector_create(initial_capacity + 1);
        vector_append(queue, NULL);
        return queue;
    }
    return NULL;
}

size_t priority_queue_index(PriorityQueue* queue, void* item) {
    size_t i;
    HeapNode* node;
    for (i = 0; i < queue->size; i++) {
        node = vector_get((Vector*) queue, i);
        if (node != NULL && node->item == item) {
            return i;
        }
    }
    return queue->size;
}

void priority_queue_reprioritise(PriorityQueue* queue, size_t index, size_t priority) {
    HeapNode* node = vector_get((Vector*) queue, index);
    if (node != NULL) {
        size_t last_priority = node->priority;
        node->priority = priority;

        if (priority > last_priority) {
            perc_down(queue, index);
        }
        else if (priority < last_priority) {
            perc_up(queue, index);
        }
    }
}

void priority_queue_add(PriorityQueue* queue, size_t priority, void* item, bool upsert) {
    if (upsert) {
        size_t index = priority_queue_index(queue, item);
        if (index > 0) {
            priority_queue_reprioritise(queue, index, priority);
            return;
        }
    }
    HeapNode* node = malloc(sizeof(HeapNode));
    if (node != NULL) {
        node->priority = priority;
        node->item = item;
        vector_append((Vector*) queue, node);
        perc_up(queue, queue->size - 1);
    }
}

void* priority_queue_remove(PriorityQueue* queue) {
    HeapNode* node = vector_get((Vector*) queue, queue->size - 1);
    if (node != NULL) {
        node = vector_set((Vector*) queue, 1, node);
        if (node != NULL) {
            void* item = node->item;
            free(node);
            vector_remove((Vector*) queue, queue->size - 1);
            perc_down(queue, 1);
        }
    }

    return NULL;
}

void* priority_queue_peek(PriorityQueue* queue, size_t* r_priority) {
    HeapNode* node = vector_get(queue, 1);
    if (node != NULL) {
        if (r_priority != NULL) {
            *r_priority = node->priority;
        }
        return node->item;
    }
    return NULL;
}

bool priority_queue_empty(PriorityQueue* queue) {
    return queue->size <= 1;
}

void priority_queue_destroy(PriorityQueue* queue) {
    size_t i;
    HeapNode* node;
    for (i = 0; i < queue->size; i++) {
        node = vector_get((Vector*) queue, i);
        free(node);
    }
    vector_destroy((Vector*) queue);
}
