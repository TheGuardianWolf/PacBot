#ifndef SIGNALS_H
#define SIGNALS_H

#include <cytypes.h>
#include <stdbool.h>

#define SIGMUX_MAX 5
#define LINE_HIGH 0xBB
#define LINE_LOW 0x2F

typedef struct {
    bool state[SIGMUX_MAX + 1];
} LineData;

CY_ISR_PROTO(line_timer);
    
CY_ISR_PROTO(line_rise);

CY_ISR_PROTO(line_fall);
    
void sensors_init();

bool sensors_ready();

LineData sensors_line_get();

bool sensors_line_check();

void sensors_line_disable(uint8_t index);

void sensors_line_enable(uint8_t index);

#endif /* SIGNALS_H */
