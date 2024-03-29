#include <project.h>
#include "adc.h"

void adc_init() {
    ADC_SEQ_Start();
    ADC_SEQ_StartConvert();
}

void adc_wait_ready() {
    // Need to wait until the finalarray is populated before you can access it
    // Remove this from initialisation if something goes wrong and debug.
    ADC_SEQ_IsEndConversion(ADC_SEQ_WAIT_FOR_RESULT);
}

ADCData adc_get() {
    // p is a pointer to the first element an array, try *(p + sizeof(*p)) if p[1] doesn't work.
    ADCData data = {
        .voltage = (float) ADC_SEQ_CountsTo_mVolts(ADC_SEQ_GetResult16(0)) * VOLTAGE_SF,
        .current = (float) ADC_SEQ_CountsTo_mVolts(ADC_SEQ_GetResult16(1)) * CURRENT_SF 
    };
    return data;
}
