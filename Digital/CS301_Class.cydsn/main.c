#include <stdbool.h>
#include <project.h>
#include <unistd.h>

#include "adc.h"
#include "motor_controller.h"
#include "interactive.h"

// #include "systime.h"
#include "quad_dec.h"
#include "motor.h"

void system_init() {
    motor_controller_init();
    // adc_init();
    CYGlobalIntEnable;
    // adc_wait_ready();
}

int main() {
    system_init();
    MCData mcd = motor_controller_create();
    while(true) {
        if(btn_get()) {
            // Forward by 1000mm
            led_set(0b000);
            
            while(btn_get());
            if(!btn_get()) {
                // quad_dec_clear();
                // QuadDecData qd = quad_dec_get();
                // QuadDecData target = {
                //     .L = 300,
                //     .R = 300
                // };
                // motor_set_L(20);
                // motor_set_R(20);
                // bool disable_L = false;
                // bool disable_R = false;
                // while (!disable_L || !disable_R) {
                //     if (qd.L >= target.L) {
                //         motor_set_L(0);
                //         disable_L = true;
                //     }
            
                //     if (qd.R >= target.R) {
                //         motor_set_R(0);
                //         disable_R = true;
                //     }
                //     qd = quad_dec_get();
                // }
                motor_controller_run_forward(&mcd, 300, 300);
            }
            
        }
        else {
            motor_set_L(M_DRIFT);
            motor_set_R(M_DRIFT);
            led_set(0b001);
        }  
    }
    return 0;

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
