#include "analog.h"
#include <project.h>

static uint8_t current_channel = 0;
static uint16_t adc_solo_samples[SOLO_SAMPLES] = {0};
static uint8_t DMA_SOLO_TD[1];
static uint8_t DMA_SOLO_Chan;

CY_ISR(adc_seq) {

}

CY_ISR(dma_solo) {
    
}

static void dma_solo_init() {
    DMA_SOLO_Chan = DMA_SOLO_DmaInitialize(DMA_BYTES_PER_BURST, DMA_REQUEST_PER_BURST, 
                                 HI16(DMA_SRC_BASE), HI16(DMA_DST_BASE));
    DMA_SOLO_TD[0] = CyDmaTdAllocate();

    CyDmaTdSetConfiguration(DMA_SOLO_TD[0], (2 * SOLO_SAMPLES), DMA_DISABLE_TD, 
				                            DMA_SOLO__TD_TERMOUT_EN | TD_INC_DST_ADR);
    CyDmaTdSetAddress(DMA_SOLO_TD[0], LO16((uint32) ADC_SOLO_SAR_WRK_PTR), LO16((uint32) adc_solo_samples));
    CyDmaChSetInitialTd(DMA_SOLO_Chan, DMA_SOLO_TD[0]);
}

void analog_init() {
    isr_ADC_SEQ_StartEx(adc_seq);
    isr_DMA_SOLO_StartEx(dma_solo);
    dma_solo_init();
    SIGMUX_Start();
    SIGMUX_FastSelect(current_channel);
    SAMP_Start();
    IAMP_Start();
    PKAMP_Start();
    ADC_SEQ_Start();
    ADC_SOLO_Start();
}

void mux_select(uint8_t channel) {
    if (channel < CHANNEL_MAX) {
        current_channel = channel;
        SIGMUX_FastSelect(channel);
    }    
}

void mux_next() {
    if (current_channel < CHANNEL_MAX) {
        current_channel++;
    }
    else {
        current_channel = 0;
    }
    SIGMUX_FastSelect(current_channel);
}

void mux_prev() {
    if (current_channel > 0) {
        current_channel--;
    }
    else {
        current_channel = CHANNEL_MAX;
    }
    SIGMUX_FastSelect(current_channel);
}

void peak_drain_set(bool state) {
    PK_DRAIN_Write(~((uint8_t) state));
}
