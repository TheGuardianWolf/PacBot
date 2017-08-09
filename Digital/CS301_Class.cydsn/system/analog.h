#ifndef ANALOG_H
#define ANALOG_H

#include <cytypes.h>

#define CHANNEL_MAX 5

void mux_init();

void mux_select(uint8_t channel);

void mux_next();

void mux_prev();

#endif /* ANALOG_H */
