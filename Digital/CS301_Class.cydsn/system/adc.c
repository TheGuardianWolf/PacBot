#include <project.h>
#include "adc.h"

static volatile uint8_t conversions_finished = 0;
static bool conversions_ready = false;

CY_ISR(adc_seq) {
    conversions_finished++;
    if (conversions_finished >= ADC_CHANNEL_MAX) {
        ADC_SEQ_IRQ_Disable();
        isr_ADC_SEQ_Disable();
    }
}

void adc_init() {
    isr_ADC_SEQ_StartEx(adc_seq);
    ADC_SEQ_Start();
}

void adc_wait_ready() {
    while (!conversions_ready);
}

int16_t adc_get() {
    return ADC_SEQ_finalArray;
}
