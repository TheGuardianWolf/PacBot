#ifndef SIGNALS_H
#define SIGNALS_H

#include <cytypes.h>
#include <stdbool.h>

#define LINE_SENSORS 6
#define LINE_INVERT 0b00011110
#define LINE_SAMPLE_RATE 20 // ms

void sensors_init();

uint8_t sensors_line_get();

#endif /* SIGNALS_H */
