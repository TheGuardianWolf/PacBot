#ifndef SIGNALS_H
#define SIGNALS_H

#include <cytypes.h>
#include <stdbool.h>

#define SIGMUX_MAX 5
#define LINE_INVERT 0b00111100
#define LINE_LOW 0x2D
#define LINE_HIGH 0x35

CY_ISR_PROTO(line_rise);

CY_ISR_PROTO(line_fall);
    
void sensors_init();

uint8_t sensors_line_get();

#endif /* SIGNALS_H */
