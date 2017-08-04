#include <project.h>
#include "quad_dec.h"
#include "debug.h"

static int8_t quad_dec_L_mult = 0;
static int8_t quad_dec_R_mult = 0;

static void events_check(uint8_t events, int8_t *mult) {
    if ((events & QUADDEC_OVERFLOW) == QUADDEC_OVERFLOW) {
        (*mult)++;
    }
    else if ((events & QUADDEC_UNDERFLOW) == QUADDEC_UNDERFLOW) {
        (*mult)--;
    }
}

CY_ISR(quad_dec_L) {
    led_set(LED_STATE_ON);
    events_check(M1_QuadDec_GetEvents(), &quad_dec_L_mult);
}

CY_ISR(quad_dec_R) {
    led_set(LED_STATE_ON);
    events_check(M2_QuadDec_GetEvents(), &quad_dec_R_mult);
}

void quad_dec_init() {
    isr_M1_QuadDec_StartEx(quad_dec_L);
    isr_M1_QuadDec_StartEx(quad_dec_R);
    M1_QuadDec_Start();
    M2_QuadDec_Start();
}

QuadDecData quad_dec_get() {
    QuadDecData qd;
    qd.L = QDL_GET() + QUADDEC_MAX * quad_dec_L_mult;
    qd.R = QDR_GET() + QUADDEC_MAX * quad_dec_R_mult;
    return qd;
}

void quad_dec_clear() {
    QDL_SET(0);
    QDR_SET(0);
}