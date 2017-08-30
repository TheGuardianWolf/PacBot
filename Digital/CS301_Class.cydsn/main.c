#include <stdbool.h>
#include <project.h>
#include <unistd.h>
#include <stdio.h>

#include "adc.h"
#include "motor_controller.h"
#include "interactive.h"
#include "motor.h"

#include "systime.h"
#include "usb.h"
#include "wireless.h"

void system_init() {
    // Starting the IAMP opamp only. If issues are encountered, set the input
    // of IAMP to pull-down resistive.
    systime_init();
    IAMP_Start();
    usb_init();
    isrRF_RX_Start();
    motor_controller_init();
    adc_init();
    wireless_init();
    CYGlobalIntEnable;

    // If system does not initialise (LED does not light up), this is likely the issue.
    adc_wait_ready();
}

int main() {
    system_init();
    MCData mcd = motor_controller_create();
    uint32_t now = 0;
    uint32_t start = 0;
    char adc_packet[64];
    char rf_packet[64];
    while(true) {
        // Without button press, default to RF and ADC output mode
        // With button press, go to straight line test
        if(btn_get()) {
            led_set(0b000);
            isrRF_RX_Stop();
            while(btn_get());
            if(!btn_get()) {
                uint32_t now = systime_ms();
                uint32_t td = 0;
                while (td < 2000){
                    td = systime_ms() - now;
                }
                // PID disable if needed
                //mcd.PID_L.active = false;
                //mcd.PID_R.active = false;
                //
                // Signature is MCData, left wheel distance (mm), right wheel distance (mm)
                motor_controller_run_forward(&mcd, 1300, 1300);
                // This function blocks intentionally, reset the robot power after reached.
            }
            
        }
        else {            
            now = systime_s();
            // RF polling function can run here
            if (now - start >= 2) {
                start = now;
                
                ADCData adc_data = adc_get();
                sprintf(adc_packet, "V:%u\n\r", (int) adc_data.voltage);
                usb_send_string(adc_packet);
//                RFData rf_data = wireless_get();
//                sprintf(rf_packet, "RSSI:%d Index:%u\n\r", rf_data.rssi, rf_data.index);
//                usb_send_string(rf_packet);
            }
        }  
    }
    return 0;
}
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
//    return 0;
//}


/* [] END OF FILE */
