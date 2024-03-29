#ifndef VECTOR_H
#define VECTOR_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct {
    void** container;
    size_t size;
    size_t capacity;
} Vector;

Vector* vector_create(size_t initial_capacity);

bool vector_resize(Vector* vector, size_t target);

void* vector_set(Vector* vector, size_t index, void* item);

void* vector_get(Vector* vector, size_t index);

void vector_append(Vector* vector, void* item);

void vector_insert(Vector* vector, size_t index, void* item);

void* vector_remove(Vector* vector, size_t index);

void vector_destroy(Vector* vector);

#endif /* VECTOR_H */
