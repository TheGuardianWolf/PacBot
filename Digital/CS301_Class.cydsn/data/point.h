#ifndef POINT_H
#define POINT_H

#include <stdint.h>

typedef struct {
    uint8_t x;
    uint8_t y;
} point_uint8_t;

typedef struct {
    uint16_t x;
    uint16_t y;
} point_uint16_t;

typedef struct {
    float x;
    float y;
} point_f;

#endif /* POINT_H */
