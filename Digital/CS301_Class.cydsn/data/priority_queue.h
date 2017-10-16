#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include "vector.h"
#include <stddef.h>

typedef struct {
    size_t priority;
    void* item;
} HeapNode;

typedef Vector PriorityQueue;

PriorityQueue* priority_queue_create(size_t initial_capacity);

size_t priority_queue_index(PriorityQueue* queue, void* item);

void priority_queue_reprioritise(PriorityQueue* queue, size_t index, size_t priority);

void priority_queue_add(PriorityQueue* queue, size_t priority, void* item, bool upsert);
    
void* priority_queue_remove(PriorityQueue* queue);
        
void* priority_queue_peek(PriorityQueue* queue, size_t* r_priority);

bool priority_queue_empty(PriorityQueue* queue);

void priority_queue_destroy(PriorityQueue* queue);
    
#endif /* PRIORITY_QUEUE_H */
