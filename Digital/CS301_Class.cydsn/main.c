#include <stdbool.h>
#include <project.h>
// #include <unistd.h>
#include <stdio.h>
#include "motor_controller.h"
#include "sensors_controller.h"
#include "interactive.h"
#include "systime.h"
#include "usb.h"

void system_init() {
    systime_init();
    sensors_controller_init();
    motor_controller_init();
    usb_init();
    CYGlobalIntEnable;
}

int main() {
    system_init();
    SCData scd = sensors_controller_create(30, (REG_DIP_Read() >> 1) & 1, (REG_DIP_Read() >> 2) & 1);
    MCData mcd = motor_controller_create(30, &scd);
    uint32_t td = 0;
    led_set(0b111);
    while(true) {
        uint32_t time = systime_ms();
        if(btn_get()) {
            led_set(0b000);
            while(btn_get());
            if(!btn_get()) {
                if (REG_DIP_Read() & 1) {
                    td = time;
                    while(systime_ms() - td < 1){}
                    motor_controller_set(&mcd, 0.7f, 0,2000); //0.1 == 9 // 0.2 == 17 // 0.3 == 25 0.4 == 35 // 0.6 == 53 // 0.7 == 60
                    while (true) {
                        sensors_controller_worker(&scd);
                        motor_controller_worker(&mcd);
                    }
                }
                led_set(0b111);
            }
        }
        else {
            if (REG_DIP_Read() >> 3 & 1) {
                if (time - td >= 1000) {
                    td = time;
                    char buffer[64];
                    uint8_t line_data = sensors_line_get();
                    sprintf(buffer, "0:%d 1:%d 2:%d 3:%d 4:%d 5:%d\r\n",
                            (int) (line_data & 1),
                            (int) ((line_data >> 1) & 1),
                            (int) ((line_data >> 2) & 1),
                            (int) ((line_data >> 3) & 1),
                            (int) ((line_data >> 4) & 1),
                            (int) ((line_data >> 5) & 1)
                            );
                    usb_send_string(buffer);
                }
            }
        }
    }
    return 0;
}

/* [] END OF FILE */
