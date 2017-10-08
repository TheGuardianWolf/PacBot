#ifndef POINT_H
#define POINT_H

#include <stdint.h>

typedef struct {
    uint8_t x;
    uint8_t y;
} pointu8_t;

typedef struct {
    uint16_t x;
    uint16_t y;
} pointu16_t;

typedef struct {
    float x;
    float y;
} pointf;

#endif /* POINT_H */
