#ifndef ANALOG_H
#define ANALOG_H

#include <cytypes.h>
#include <stdbool.h>

#define CHANNEL_MAX 5
#define SOLO_SAMPLES 3
#define DMA_BYTES_PER_BURST 2
#define DMA_REQUEST_PER_BURST 1
#define DMA_SRC_BASE (CYDEV_PERIPH_BASE)
#define DMA_DST_BASE (CYDEV_SRAM_BASE)

CY_ISR_PROTO(adc_seq);

CY_ISR_PROTO(dma_solo);

void analog_init();

void mux_select(uint8_t channel);

void mux_next();

void mux_prev();

void peak_drain_set(bool state);

#endif /* ANALOG_H */
