#ifndef QUADDEC_H
#define QUADDEC_H
    
#include <cytypes.h>

#define QUADDEC_MAX 0x7FFF
#define QUADDEC_UNDERFLOW M1_QuadDec_COUNTER_UNDERFLOW
#define QUADDEC_OVERFLOW M1_QuadDec_COUNTER_OVERFLOW
#define QDL_GET() M1_QuadDec_GetCounter()
#define QDR_GET() M2_QuadDec_GetCounter()
#define QDL_SET(x) M1_QuadDec_SetCounter(x)
#define QDR_SET(x) M2_QuadDec_SetCounter(x)

typedef struct {
    int32_t L;
    int32_t R;
}  QuadDecData;

CY_ISR_PROTO(quad_dec_L);

CY_ISR_PROTO(quad_dec_R);

void quad_dec_init();

QuadDecData quad_dec_get();

void quad_dec_clear();

#endif /* QUADDEC_H */
