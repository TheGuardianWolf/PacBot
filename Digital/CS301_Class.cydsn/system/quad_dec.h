#ifndef QUADDEC_H
#define QUADDEC_H
    
#include <cytypes.h>

#define QUADDEC_MAX_16 0x7FFF  // About 30 seconds at max speed for overflow
#define WHEEL_CIRCUMFERENCE 204 // millimeters
#define PULSES_PER_REV 228 // Decoder x4

typedef struct {
    int32_t L;
    int32_t R;
}  QuadDecData;

int32_t dist2dec(int32_t dist);

int32_t dec2dist(int32_t dec);

void quad_dec_init();

QuadDecData quad_dec_get();

void quad_dec_clear();

#endif /* QUADDEC_H */
