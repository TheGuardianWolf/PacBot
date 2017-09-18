#ifndef SIGNALS_H
#define SIGNALS_H

#include <cytypes.h>
#include <stdbool.h>

#define SIGMUX_MAX 5
#define LINE_INVERT 0b00011110

CY_ISR_PROTO(read_line);
    
void sensors_init();

uint8_t sensors_line_get();

#endif /* SIGNALS_H */
