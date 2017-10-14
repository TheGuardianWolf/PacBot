#include "vector.h"


Vector* vector_create(size_t initial_capacity) {
    Vector* vector = malloc(block_size * initial_capacity);
    if (vector != NULL) {
        vector->capacity = initial_capacity;
        return vector;
    }
    return NULL;
}

bool vector_resize(const Vector* vector, size_t target) {
    size_t new_capacity = vector->capacity;
    if (target > vector->capacity) {
        do {
            new_capacity *= 2;
        } while(target > new_capacity);
        new_container = realloc(vector, sizeof(void*) * new_capacity);
        if (new_container == NULL) {
            return false;
        }
        vector->capacity = new_capacity;
        vector->container = new_container;
    }
    else if (target < vector->capacity) {
        do {
            new_capacity /= 2;
        } while(target < new_capacity);
        new_container = realloc(vector, sizeof(void*) * new_capacity);
        if (new_container == NULL) {
            return false;
        }
        vector->capacity = new_capacity;
        vector->container = new_container;
    }
    return true;
}

void* vector_set(const Vector* vector, size_t index, void* item) {
    void* old_item = *(vector->container[index]);
    *(vector->container[index]) = item;
    return old_item;
}

void* vector_get(const Vector* vector, size_t index) {
    if (index < vector->size) {
        return *(vector->container[index]);
    }
    return NULL;
}

void vector_append(const Vector* vector, void* item) {
    if (vector_resize(vector, vector->size + 1)) {
        *(vector->container[vector->size]) = item;
        vector->size++;
    }
}

void vector_insert(const Vector* vector, size_t index, void* item) {
    if (index <= vector->size && vector_resize(vector, vector->size + 1)) {
        size_t i;
        for (i = vector->size; i > index; i--) {
            *(vector->container[i]) = *(vector->container[i - 1]);
        }
        *(vector->container[index]) = item;
        vector->size++;
    }
}

void* vector_remove(const Vector* vector, size_t index) {
    if (index < vector->size) {
        void* item = *(vector->container[index]);
        size_t i;
        for (i = vector->size; i > index; i--) {
             *(vector->container[i - 1]) = *(vector->container[i]);
        }
        vector->size--;
        vector_resize(vector, vector->size);
        return item;
    }
    return NULL;
}

void vector_destroy(const Vector* vector) {
    free(vector);
}
