#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <stddef.h>

typedef struct {
    size_t priority;
    void* item;
} HeapNode;

typedef Vector PriorityQueue;

PriorityQueue* priority_queue_create(size_t initial_capacity)

size_t priority_queue_index(PriorityQueue* queue, void* item);

size_t priority_queue_reprioritise(const PriorityQueue* queue, size_t index, size_t priority);

void priority_queue_add(const PriorityQueue* queue, size_t priority, void* item, bool upsert);
    
void* priority_queue_remove(const PriorityQueue* queue);
        
void* priority_queue_peek(const ProrityQueue* queue, const size_t* r_priority);

void priority_queue_destroy(const PriorityQueue* queue);
    
#endif /* PRIORITY_QUEUE_H */
