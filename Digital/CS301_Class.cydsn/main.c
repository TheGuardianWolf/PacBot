#include <stdbool.h>
#include <project.h>
#include <unistd.h>

#include "adc.h"
#include "motor_controller.h"
#include "interactive.h"

#include "systime.h"
#include "usb.h"

void system_init() {
    // Starting the IAMP opamp only. If issues are encountered, set the input
    // of IAMP to pull-down resistive.
    IAMP_Start();

    motor_controller_init();
    adc_init();
    CYGlobalIntEnable;

    // If system does not initialise (LED does not light up), this is likely the issue.
    adc_wait_ready();
}

int main() {
    system_init();
    MCData mcd = motor_controller_create();
    while(true) {
        // Without button press, default to RF and ADC output mode
        // With button press, go to straight line test
        if(btn_get()) {
            led_set(0b000);
            while(btn_get());
            if(!btn_get()) {
                uint32_t now = systime_ms();
                uint32_t td = 0;
                while (td < 2000){
                    td = systime_ms() - now;
                }
                // Signature is MCData, left wheel distance (mm), right wheel distance (mm)
                motor_controller_run_forward(&mcd, 1100, 1100);
                // This function blocks intentionally, reset the robot power after reached.
            }
            
        }
        else {
            // RF per 1 second, ADC per 2 seconds.
            led_set(0b001);
            uint32_t now = systime_s();
            uint32_t start_adc = now;
            uint32_t start_rf = now;
            // RF polling function can run here


            if (now - start_adc >= 2) {
                start_adc = now;
                char packet[64];
                ADCData adc_data = adc_get();
                sprintf(packet, "V:%.3f A:%.3f", adc_data.voltage, adc_data.current);
                usb_send_string(packet);
            }
            if (now - start_rf >= 1) {
                start_rf = now;
                char packet[64];
                // Put RF formatting crap here and sprintf it into the packet
                //sprintf(packet, "V:%.3u A:%.3u");
                //usb_send_string(packet);
            }
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
