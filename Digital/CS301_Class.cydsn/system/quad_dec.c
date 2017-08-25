#include <project.h>
#include "quad_dec.h"

// static volatile int8_t quad_dec_L_mult = 0;
// static volatile int8_t quad_dec_R_mult = 0;

// static void events_check(uint8_t events, volatile int8_t *mult) {
//     if ((events & QUADDEC_OVERFLOW) == QUADDEC_OVERFLOW) {
//         (*mult)++;
//     }
//     else if ((events & QUADDEC_UNDERFLOW) == QUADDEC_UNDERFLOW) {
//         (*mult)--;
//     }
//     led_set(LED_STATE_ON); // Debug line
// }

// CY_ISR(quad_dec_L) {
//     events_check(M1_QuadDec_GetEvents(), &quad_dec_L_mult);
// }

// CY_ISR(quad_dec_R) {
//     events_check(M2_QuadDec_GetEvents(), &quad_dec_R_mult);
// }

void quad_dec_init() {
    // isr_M1_QuadDec_StartEx(quad_dec_L);
    // isr_M2_QuadDec_StartEx(quad_dec_R);
    M1_QuadDec_Start();
    M2_QuadDec_Start();
}

QuadDecData quad_dec_get() {
    QuadDecData qd;
    qd.L = M1_QuadDec_GetCounter() + QUADDEC_MAX * quad_dec_L_mult;
    qd.R = M2_QuadDec_GetCounter() + QUADDEC_MAX * quad_dec_R_mult;
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
