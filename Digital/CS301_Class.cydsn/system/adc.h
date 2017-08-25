#ifndef ADC_H
#define ADC_H

#include <cytypes.h>
#include <stdbool.h>

#define CHANNEL_MAX 4

CY_ISR_PROTO(adc_seq);

void adc_init();

#endif /* ADC_H */
