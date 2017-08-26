#include <project.h>

#include "sensors_controller.h"
#include "systime.h"
#include "signals.h"
#include "adc.h"

void sensors_controller_init() {
    adc_init();
    signals_init();
}
