#include "priority_queue.h"
#include "vector.h"

static void perc_up(const PriorityQueue* queue, size_t index) {
    HeapNode* a, b;
    size_t fd = index / 2;
    while fd > 0 {
        a = vector_get(queue, index);
        b = vector_get(queue, fd);
        if (a != NULL && b != NULL) {
            if (a->priority < b->priority) {
                vector_set(queue, fd, a);
                vector_set(queue, index, b);
            }
        }
        fd /= 2;
    }    
}

static size_t min_child(const PriorityQueue* queue, size_t index) {
    size_t child_index;
    size_t index_x2 = index * 2;
    size_t index_x2p1 = index * 2 + 1;

    if (index_x2p1 > queue->size) {
        child_index = index_x2;
    }
    else {
        HeapNode* a, b;
        a = vector_get(queue, index_x2);
        b = vector_get(queue, index_x2p1);
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

static void perc_down(const PriorityQueue* queue, size_t index) {
    HeapNode* a, b;
    size_t mc;

    while (index * 2 <= queue->size) {
        mc = min_child(queue, index);
        a = vector_get(queue, index);
        b = vector_get(queue, mc);
        if (a != NULL && b != NULL) {
            if (a->priority > b->priority) {
                vector_set(queue, index, b);
                vector_set(queue, mc, a);
            }
        }
        index = mc;
    }  
}

PriorityQueue* priority_queue_create(size_t block_size, size_t initial_capacity) {
    PriorityQueue* queue = malloc(sizeof(PriorityQueue));
    if (queue != NULL) {
        // Spare block at front of vector for easier addressing
        queue = vector_create(block_size, initial_capacity + 1);
        vector_append(queue, NULL);
        return queue;
    }
    return NULL;
}

size_t priority_queue_index(PriorityQueue* queue, void* item) {
    size_t i;
    HeapNode* node;
    for (i = 0; i < queue->size; i++) {
        node = vector_get(queue, i);
        if (node != NULL && node->item == item) {
            return i;
        }
    }
    return queue->size;
}

void priority_queue_reprioritise(const PriorityQueue* queue, size_t index, size_t priority) {
    HeapNode* node = vector_get(queue, index);
    if (node != NULL) {
        size_t last_priority = node->priority;
        node->priority = priority;

        if (priority > last_priority) {
            perc_down(index);
        }
        else if (priority < last_priority) {
            perc_up(index);
        }
    }
}

void priority_queue_push(const PriorityQueue* queue, size_t priority, void* item, bool upsert) {
    if (upsert) {
        size_t index = priority_queue_index(item);
        if (index > 0) {
            priority_queue_reprioritise(queue, index, priority);
            return;
        }
    }
    HeapNode* node = malloc(sizeof(HeapNode));
    if (node != NULL) {
        node->priority = priority;
        node->item = item;
        vector_append(queue, node);
        self.perc_up(queue->size - 1);
    }
}

void* priority_queue_remove(const PriorityQueue* queue, size_t index) {
    HeapNode node = vector_get(queue, queue->size - 1);
    if (node != NULL) {
        node = vector_set(queue, 1, node);
        if (node != NULL) {
            void* item = node->item;
            free(node);
            vector_remove(queue, queue->size - 1);
            perc_down(queue, 1);
        }
    }

    return NULL;
}

void* priority_queue_peek(const ProrityQueue* queue, const size_t* r_priority) {
    HeapNode node = vector_get(queue, 1);
    if (node != NULL) {
        if (r_priority != NULL) {
            *r_priority = node->priority;
        }
        return node->item;
    }
    return null;
}