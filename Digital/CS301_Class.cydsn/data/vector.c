#include "vector.h"
#include <stdlib.h>

Vector* vector_create(size_t initial_capacity) {
    Vector* vector = malloc(sizeof(Vector));
    vector->container = malloc(sizeof(void*) * initial_capacity);
    vector->size = 0;
    vector->capacity = (initial_capacity < 1) ? 1 : initial_capacity;

    if (vector != NULL && vector->container != NULL) {
        return vector;
    }
    return NULL;
}

bool vector_resize(Vector* vector, size_t target) {
    void** new_container;
    size_t new_capacity = vector->capacity;
    if (target > vector->capacity) {
        do {
            if (new_capacity >= 1) {
                new_capacity *= 2;
            }
            else {
                new_capacity = 1;
            }
        } while(target > new_capacity);
        new_container = realloc(vector->container, sizeof(void*) * new_capacity);
        if (new_container == NULL) {
            return false;
        }
        vector->capacity = new_capacity;
        vector->container = new_container;
    }
    // else if (target < vector->capacity) {
    //     do {
    //         new_capacity /= 2;
    //     } while(target < new_capacity);
    //     new_container = realloc(vector, sizeof(void*) * new_capacity);
    //     if (new_container == NULL) {
    //         return false;
    //     }
    //     vector->capacity = new_capacity;
    //     vector->container = new_container;
    // }
    return true;
}

void* vector_set(Vector* vector, size_t index, void* item) {
    void* old_item = vector->container[index];
    vector->container[index] = item;
    return old_item;
}

void* vector_get(Vector* vector, size_t index) {
    if (index < vector->size) {
        return vector->container[index];
    }
    return NULL;
}

void vector_append(Vector* vector, void* item) {
    if (vector_resize(vector, vector->size + 1)) {
        vector->container[vector->size] = item;
        vector->size++;
    }
}

void vector_insert(Vector* vector, size_t index, void* item) {
    if (index <= vector->size && vector_resize(vector, vector->size + 1)) {
        size_t i;
        for (i = vector->size; i > index; i--) {
            vector->container[i] = vector->container[i - 1];
        }
        vector->container[index] = item;
        vector->size++;
    }
}

void* vector_remove(Vector* vector, size_t index) {
    if (index < vector->size) {
        void* item = vector->container[index];
        size_t i;
        for (i = index; i < vector->size - 1; i++) {
             vector->container[i] = vector->container[i + 1];
        }
        vector->size--;
        return item;
    }
    return NULL;
}

void vector_destroy(Vector* vector) {
    free(vector->container);
    free(vector);
}
