#include <stdbool.h>
#include "vector.h"

static bool vector_resize(vector_t* vector) {
    if (vector->size >= vector->capacity) {
        size_t new_capacity = vector->capacity * 2;
        new_container = realloc(vector, vector->block_size * new_capacity);
        if (new_container == NULL) {
            return false;
        }
        vector->capacity = new_capacity;
        vector->container = new_container;
    }
    else if (vector->size <= vector->capacity / 2) {
        size_t new_capacity = vector->capacity / 2;
        new_container = realloc(vector, vector->block_size * new_capacity);
        if (new_container == NULL) {
            return false;
        }
        vector->capacity = new_capacity;
        vector->container = new_container;
    }
    return true;
}

vector_t* vector_create(size_t block_size, size_t initial_capacity) {
    vector_t* vector = malloc(block_size * initial_capacity);
    vector->block_size = block_size;
    vector->capacity = initial_capacity;
    return vector;
}

void vector_append(vector_t* vector, void* item) {
    if (vector_resize(vector)) {
        *((vector->container) + (vector->size * vector->block_size)) = item;
        vector->size++;
    }
}

void vector_insert(vector_t* vector, size_t index, void* item) {
    if (index <= vector->size && vector_resize(vector)) {
        size_t i;
        for (i = vector->size; i > index; i--) {
            *((vector->container) + (i * vector->block_size)) = *((vector->container) + ((i - 1) * vector->block_size));
        }
        *((vector->container) + (index * vector->block_size)) = item;
        vector->size++;
    }
}

void vector_remove(vector_t* vector, size_t index) {
    if (index < vector->size) {
        size_t i;
        for (i = vector->size; i > index; i--) {
             *((vector->container) + ((i - 1) * vector->block_size)) = *((vector->container) + (i * vector->block_size));
        }
        vector->size--;
        vector_resize(vector);
    }
}

void vector_destroy(vector_t* vector) {
    free(vector);
}
