#include <cytypes.h>
#include <string.h>
#include "usb.h"

void log_usb(char[] message) {
    usb_send_string(message);
}
