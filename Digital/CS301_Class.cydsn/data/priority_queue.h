#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <stddef.h>

typedef struct {
    size_t priority;
    void* item;
} HeapNode;

typedef Vector PriorityQueue;

PriorityQueue* priority_queue_create(size_t block_size, size_t initial_capacity);

size_t priority_queue_index(const PriorityQueue* queue, void* item);

size_t priority_queue_reprioritise(const PriorityQueue* queue, size_t index, size_t priority);

void priority_queue_push(size_t block_size, size_t initial_capacity);

void* priority_queue_remove(size_t block_size, size_t initial_capacity);

void* priority_queue_peek(const ProrityQueue* queue, size_t index, const size_t* r_priority);

#endif /* PRIORITY_QUEUE_H */
