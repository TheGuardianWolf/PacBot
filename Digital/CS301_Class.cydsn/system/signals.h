#ifndef SIGNALS_H
#define SIGNALS_H

#include <cytypes.h>
#include <stdbool.h>

#define SIGMUX_CHANNEL_MAX 5

void signals_init();

void mux_select(uint8_t channel);

void mux_next();

void mux_prev();

void drain_set(bool state);

#endif /* SIGNALS_H */
