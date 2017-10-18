#include <stddef.h>
#include "memory.h"
#include "interactive.h"

void* g_malloc(size_t segment_size) {
    void* ptr = malloc(segment_size);
    if (ptr == NULL) {
        led_set(0b001);
    }
    return ptr;
}

void* g_realloc(size_t segment_size) {
    void* ptr = realloc(segment_size);
    if (ptr == NULL) {
        led_set(0b001);
    }
    return ptr;
}
