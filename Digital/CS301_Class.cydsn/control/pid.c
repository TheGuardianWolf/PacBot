/* ========================================
 *
 * http://brettbeauregard.com/blog/2011/04/improving-the-beginners-pid-introduction/ 
 *
 * this 7/08 code only implements 2/8 of the blog
 *
 * ========================================
*/

/* [] END OF FILE */

#include <project.h>
#include "pid.h"
#include "quad_dec.h"
#include "motor.h"
#include "systime.h"

static uint32_t lastRun = 0;
static float lastCounter_R, lastCounter_L = 0;
static float errorSum_L, errorSum_R, pidout_L, pidout_R;
static float kpL, kiL, kdL, kpR, kiR, kdR;


void pid_init() {
    systime_init();
    motor_init();
    quad_dec_init();
}

//this gives me the rotations predicted based on speed. Dunno how to calculate yet.
float getExpectedRotations() {
    return 0.0;
}

void pid_worker() {
    if (systime_ms() - lastRun >= 100) {
        pid_compute();
    }
}

void pid_compute() {
    QuadDecData qd = quad_dec_get(); 
    
    //for proportional part
    float error_L = getExpectedRotations() - currentCounter_L;
    float error_R = getExpectedRotations() - currentCounter_R;
    
    // for integral part
    errorSum_L += error_L;
    errorSum_R += error_R;
    
    //for differentiatial part
    float dCounter_L = currentCounter_L - lastCounter_L;
    float dCounter_R = currentCounter_R - lastCounter_R;
    
    pidout_L = kpL * error_L + kiL * errorSum_L - kdL * dCounter_L;
    pidout_R = kpR * error_R + kiR * errorSum_R - kdR * dCounter_R;
    
    lastCounter_L = currentCounter_L;
    lastCounter_R = currentCounter_R;
    
    //probably need getters and setters in motor.c to get the current speeds
    float currentspeedright, currentspeedleft = 0;
    motor_set(currentspeedleft * pidout_L, currentspeedright * pidout_R);
}

void setPIDL(float KpL, float KiL, float KdL) {
    kpL = KpL;
    kiL = KiL;
    kdL = KdL;
}

void setPIDR(float KpR, float KiR, float KdR) {
    kpR = KpR;
    kiR = KiR;
    kdR = KdR;
}
