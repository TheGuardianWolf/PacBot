#include <project.h>
#include "signals.h"

static uint8_t mux_channel = 0;

void signals_init() {
    SIGMUX_Start();
    SIGMUX_FastSelect(current_channel);
    SMUX_BUF_Start();
    SAMP_Start();
    IAMP_Start();
    PKAMP_Start();
    PKCOMP_Start();
    PKCOMP_REF_Start();
}

void mux_select(uint8_t channel) {
    if (channel < SIGMUX_CHANNEL_MAX) {
        mux_channel = channel;
        SIGMUX_FastSelect(channel);
    }    
}

void mux_next() {
    if (mux_channel < SIGMUX_CHANNEL_MAX) {
        mux_channel++;
    }
    else {
        mux_channel = 0;
    }
    SIGMUX_FastSelect(mux_channel);
}

void mux_prev() {
    if (mux_channel > 0) {
        mux_channel--;
    }
    else {
        mux_channel = SIGMUX_CHANNEL_MAX;
    }
    SIGMUX_FastSelect(mux_channel);
}

void drain_set(bool state) {
    REG_DRAIN_Write(((uint8_t) state));
}
