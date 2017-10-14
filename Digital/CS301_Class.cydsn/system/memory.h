#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>

void* g_malloc(size_t block_size);

void* g_realloc(size_t segment_size);

#endif /* MEMORY_H */
