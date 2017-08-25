#include <stdbool.h>
#include <project.h>
#include <unistd.h>
#include "adc.h"
#include "pid.h"
#include "interactive.h"

#define PID_ENABLE true

void system_init() {
    pid_init();
    adc_init();
    CYGlobalIntEnable;
    adc_wait_ready();
}

int main() {
    system_init();

    // ARM Cortex M3 supports max 3 pipelines
    pid_t process_id = 0;
    process_id = fork();

    // Multiprocess task selection
    if (process_id == 0) {
        // Child process
        // Non-critical workers (etc. voltage measurements, rf)
        while(true) {
            
        }
    } 
    else if (process_id > 0) {
        // Parent process
        // Mission critical workers only
        while(true) {
            #ifdef PID_ENABLE
            pid_worker();
            #else
            break;
            #endif
        }
    }
    else {
        // Error in forking, display warning LED.
        led_set(0b001);
    }
}


/* [] END OF FILE */
