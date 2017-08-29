#include <project.h>
#include "adc.h"

static volatile uint8_t conversions_finished = 0;
static bool conversions_ready = false;

static uint16_t int162uint16(int16 i) {
    return (uint16_t)((int32_t) i + 32768) 
}

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
    // Need to wait until the finalarray is populated before you can access it
    // Remove this from initialisation if something goes wrong and debug.
    while (!conversions_ready);
}

ADCData adc_get() {
    p = ADC_SEQ_finalArray;
    // p is a pointer to the first element an array, try *(p + sizeof(*p)) if p[1] doesn't work.
    ADCData data = {
        .voltage = (float) int162uint16(p[0]) * VOLTAGE_SF,
        .current = (float) int162uint16(p[1]) * CURRENT_SF 
    };
    return data;
}
