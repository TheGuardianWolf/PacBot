#ifndef DEBUG_H
#define DEBUG_H

#include <stdint.h>
    
#define LED_STATE_ON 1
#define LED_STATE_OFF 0
#define LED_STATE_TOGGLE 2
    
void led_set(uint8_t state);
    
void log_usb(char* message);

#endif /* DEBUG_H */
