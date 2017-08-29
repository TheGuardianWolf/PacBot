#ifndef ADC_H
#define ADC_H

#include <cytypes.h>
#include <stdbool.h>

#define ADC_CHANNEL_MAX 3
#define VOLTAGE_SF 1.8393  // 1V/V from Vbat with V=0.543Vbat with Vbat = 1.8393 Vdiv
#define CURRENT_SF 0.25  // 2V/A from IC, with x2 IAMP = 4V/A = 0.25A/V

typedef struct {
    float voltage;
    float current;
}  ADCData;

CY_ISR_PROTO(adc_seq);

void adc_init();

void adc_wait_ready();

ADCData adc_get();

#endif /* ADC_H */
