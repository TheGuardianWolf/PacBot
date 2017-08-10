#include "analog.h"
#include <project.h>

static uint8_t current_channel = 0;

void analog_init() {
    SIGMUX_Start();
    SAMP_Start();
    IAMP_Start();
    PKAMP_Start();
    SIGMUX_FastSelect(current_channel);
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
