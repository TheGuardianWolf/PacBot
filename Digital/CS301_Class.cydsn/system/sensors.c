#include <project.h>
#include "sensors.h"

static volatile uint8_t line_buffer, line_data = 0;
static volatile int8_t mux_selection = 0;
static volatile uint8_t line_fsm_state = 0;
static volatile bool line_init = false;

CY_ISR(line_rise) {
    uint8_t reading;
    switch(line_fsm_state) {
        case 0:
        reading = REG_LINE_Read();
        line_buffer |= reading << mux_selection;
        if (mux_selection == 0) {
            if (reading == 1) {
                REG_LED_Write(REG_LED_Read() | 0b001);
            }
            else {
                REG_LED_Write(REG_LED_Read() & 0b110);
            }
        }
        else if (mux_selection == 1) {
            if (reading == 1) {
                REG_LED_Write(REG_LED_Read() | 0b010);
            }
            else {
                REG_LED_Write(REG_LED_Read() & 0b101);
            }
        } 
        else if (mux_selection == 2) {
            if (reading == 1) {
                REG_LED_Write(REG_LED_Read() | 0b100);
            }
            else {
                REG_LED_Write(REG_LED_Read() & 0b011);
            }
        }
        SIGMUX_Next();
        if (mux_selection < SIGMUX_MAX) {
            mux_selection++;
        }
        else {
            mux_selection = 0;
            line_data = line_buffer;
            uint8_t temp = line_data;
            line_buffer = 0;
            if (!line_init) {
                line_init = true;
            }
        }
        PK_DRAIN_Write(0);
        // SAMP_DRAIN_Write(0);
        SIGTIMER_RESET_Write(0b10);
        line_fsm_state = 1;
        break;
        default:
        break;
    }
}

CY_ISR(line_fall) {
    switch(line_fsm_state) {
        case 1:
        PK_DRAIN_Write(1);
        SAMP_DRAIN_Write(1);
        line_fsm_state = 0;
        SIGTIMER_RESET_Write(0b01);
        break;
        default:
        break;
    }
}

void sensors_init() {
    SIGMUX_Start();
    SIGMUX_Next();
    SMUX_BUF_Start();
    SAMP_Start();
    IAMP_Start();
    PKAMP_Start();
    PKCOMP_LOW_Start();
    PKCOMP_HIGH_Start();
    PKCOMP_REF_LOW_Start();
    PKCOMP_REF_HIGH_Start();
    PKCOMP_REF_LOW_SetValue(LINE_LOW);
    PKCOMP_REF_HIGH_SetValue(LINE_HIGH);
    isr_SIGRISE_StartEx(line_rise);
    isr_SIGFALL_StartEx(line_fall);
    SIGTIMER_RISE_Start();
    SIGTIMER_FALL_Start();

    // Wait for first set of readings.
    //while (!line_init);
}

uint8_t sensors_line_get() {
    // Reports inverted/non-inverted sensors based on straight line model
    return line_buffer; //^ LINE_INVERT;
}
