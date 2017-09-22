#include <project.h>
#include "sensors.h"

static volatile uint8_t line_buffer, line_data = 0;
static volatile int8_t mux_selection = 0;

CY_ISR(read_line) {
    uint8_t temp = line_buffer |= REG_LINE_Read() << mux_selection;
    line_buffer = temp;//|= REG_LINE_Read() << mux_selection;
    REG_DRAIN_Write(1);
    SIGMUX_Next();
    if (mux_selection == SIGMUX_MAX) {
        mux_selection = 0;
        line_data = line_buffer;
        line_buffer = 0;
    }
    else {
        mux_selection++;
    }
    REG_DRAIN_Write(0);
    SIGTIMER_RESET_Write(1);
}

void sensors_init() {
    SIGMUX_Start();
    SIGMUX_Next();
    SMUX_BUF_Start();
    SAMP_Start();
    IAMP_Start();
    PKAMP_Start();
    PKCOMP_Start();
    PKCOMP_REF_Start();
    SIGTIMER_Start();
    isr_SIGRISE_StartEx(read_line);
}

uint8_t sensors_line_get() {
    // Reports inverted/non-inverted sensors based on straight line model
    return line_data ^ LINE_INVERT;
}
