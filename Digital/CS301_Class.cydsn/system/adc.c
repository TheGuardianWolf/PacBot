#include <project.h>
#include "adc.h"

CY_ISR(adc_seq) {
    
}

static uint8_t current_channel = 0;

void adc_init() {
    isr_ADC_SEQ_StartEx(adc_seq);
    ADC_SEQ_Start();
}
