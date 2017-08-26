#include <stdbool.h>
#include <project.h>
#include <unistd.h>

#include "adc.h"
#include "motor_controller.h"
#include "interactive.h"

void system_init() {
    motor_controller_init();
    // adc_init();
    CYGlobalIntEnable;
    // adc_wait_ready();
}

int main() {
    system_init();
    MCData mcd = motor_controller_create();
    motor_controller_run_forward(mcd, )

    // // ARM Cortex M3 supports max 3 pipelines
    // pid_t process_id = 0;
    // process_id = fork();

    // // Multiprocess task selection
    // if (process_id == 0) {
    //     // Child process
    //     // Non-critical workers (etc. voltage measurements, rf)
    //     while(true) {
            
    //     }
    // } 
    // else if (process_id > 0) {
    //     // Parent process
    //     // Mission critical workers only
    //     while(true) {
    //         motor_controller_worker();
    //     }
    // }
    // else {
    //     // Error in forking, display warning LED.
    //     led_set(0b001);
    // }
    return 0;
}


/* [] END OF FILE */
