#include <project.h>
#include "sensors.h"

static LineData line_invert = {
    .state = {true, true, false, false, false, false}
};
static volatile LineData line_data = {
    .state = {true, true, true, true, true, true}
};
static volatile uint8_t line_buffer, line_data = 0;
static volatile int8_t mux_selection = 0;
static volatile uint8_t line_fsm_state = 0;
static volatile bool line_init = false;
static volatile bool line_new_reading = false;

static void line_fsm_start() {
    REG_DRAIN_Write(0b11);
    SIGTIMER_RESET_Write(0b1);
    isr_SIGTIMER_ClearPending();
    isr_SIGTIMER_Enable();
    mux_selection = SIGMUX_GetChannel();
}

static void line_fsm(uint8_t trigger) {  // 0 for line_timer, 1 for line_rise, 2 for line_fall
    uint8_t reading;
    switch(line_fsm_state) {
        case 0:
        if (trigger == 0) {
            REG_DRAIN_Write(0b00);
            isr_SIGRISE_ClearPending();
            isr_SIGRISE_Enable();
            SIGTIMER_RESET_Write(0b1);
            isr_SIGTIMER_ClearPending();
            isr_SIGTIMER_Enable();
            line_fsm_state = 1;
        }
        case 1:
        if (trigger == 0 || trigger == 1) {
            isr_SIGTIMER_Disable();
            isr_SIGRISE_Disable();
            reading = REG_LINE_Read();
            line_data.state[mux_selection] = (bool) reading ^ line_invert.state[mux_selection];
            line_new_reading = true;
            if (!(REG_DIP_Read() & 1)){
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
            }
            if (mux_selection == SIGMUX_MAX) {
                line_init = true;
            }
            SIGMUX_Next();
            isr_SIGFALL_ClearPending();
            isr_SIGFALL_Enable();
            REG_DRAIN_Write(0b11);
            SIGTIMER_RESET_Write(0b1);
            isr_SIGTIMER_ClearPending();
            isr_SIGTIMER_Enable();
            line_fsm_state = 2;
        }
        break;
        case 2:
        if (trigger == 0 || trigger == 2) {
            isr_SIGTIMER_Disable();
            isr_SIGFALL_Disable();
            REG_DRAIN_Write(0b00);
            isr_SIGRISE_ClearPending();
            isr_SIGRISE_Enable();
            SIGTIMER_RESET_Write(0b1);
            isr_SIGTIMER_ClearPending();
            isr_SIGTIMER_Enable();
            mux_selection = SIGMUX_GetChannel();
            line_fsm_state = 1;
        }
        break;
        default:
        break;
    }
}

CY_ISR(line_timer) {
    line_fsm(0);
}

CY_ISR(line_rise) {
    line_fsm(1);
}

CY_ISR(line_fall) {
    line_fsm(2);
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
    isr_SIGTIMER_StartEx(line_timer);
    isr_SIGTIMER_Disable();
    isr_SIGRISE_StartEx(line_rise);
    isr_SIGRISE_Disable();
    isr_SIGFALL_StartEx(line_fall);
    isr_SIGFALL_Disable();
    SIGTIMER_Start();
    line_fsm_start();
    // Wait for first set of readings.
    //while (!line_init);
}

LineData sensors_line_get() {
    // Reports inverted/non-inverted sensors based on straight line model
    line_new_reading = false;
    return line_data;
}

bool sensors_line_check() {
    bool ret = line_new_reading;
    return ret;
}
