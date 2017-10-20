#include <project.h>
#include "quad_dec.h"

// mm -> pulse
int32_t dist2dec(int32_t dist) {
    return (int32_t) ((float) dist * PULSES_PER_REV / WHEEL_CIRCUMFERENCE);
}

// pulse -> mm
int32_t dec2dist(int32_t dec) {
    return (int32_t) ((float) dec * WHEEL_CIRCUMFERENCE / PULSES_PER_REV) ;
}

void quad_dec_init() {
    // isr_M1_QuadDec_StartEx(quad_dec_L);
    // isr_M2_QuadDec_StartEx(quad_dec_R);
    M1_QuadDec_Start();
    M2_QuadDec_Start();
}

QuadDecData quad_dec_get() {
    QuadDecData qd = {
        .L = M1_QuadDec_GetCounter(),
        .R = M2_QuadDec_GetCounter()
    };
    // qd.L = M1_QuadDec_GetCounter();// + QUADDEC_MAX * quad_dec_L_mult;
    // qd.R = M2_QuadDec_GetCounter();// + QUADDEC_MAX * quad_dec_R_mult;
    return qd;
}

void quad_dec_clear() {
    M1_QuadDec_Stop();
    M2_QuadDec_Stop();
    M1_QuadDec_SetCounter(0);
    M2_QuadDec_SetCounter(0);
    M1_QuadDec_Start();
    M2_QuadDec_Start();
}
