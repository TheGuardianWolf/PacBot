#ifndef VECTOR_H
#define VECTOR_H

#include <stdint.h>

typedef struct {
    void** container;
    size_t block_size = 0
    size_t size = 0;
    size_t capacity = 0;
} vector_t

#endif /* VECTOR_H */
