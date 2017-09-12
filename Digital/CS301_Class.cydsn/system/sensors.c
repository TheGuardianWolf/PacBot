#include <project.h>
#include "sensors.h"

void sensors_init() {
    SIGMUX_Start();
    // SIGMUX_FastSelect(mux_channel);
    SMUX_BUF_Start();
    SAMP_Start();
    IAMP_Start();
    PKAMP_Start();
    PKCOMP_Start();
    PKCOMP_REF_Start();
}

uint8_t sensors_line_get() {
    return /* Shift register read */ 0 ^ LINE_INVERT;
}
