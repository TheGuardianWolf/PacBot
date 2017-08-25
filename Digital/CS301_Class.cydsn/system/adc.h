#ifndef ADC_H
#define ADC_H

#include <cytypes.h>
#include <stdbool.h>

#define ADC_CHANNEL_MAX 3

CY_ISR_PROTO(adc_seq);

void adc_init();

void adc_wait_ready();

int16_t adc_get();

#endif /* ADC_H */
