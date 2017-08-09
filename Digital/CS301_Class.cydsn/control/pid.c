/* ========================================
 *
 * http://brettbeauregard.com/blog/2011/04/improving-the-beginners-pid-introduction/ 
 *
 * this 7/08 code only implements 2/8 of the blog
 *
 * ========================================
*/

/* [] END OF FILE */

#include "pid.h"
#include <project.h>
#include "debug.h"
#include "quad_dec.h"
#include "motor.h"

//probably using too many static variables?
static unsigned long lastTime;
static unsigned long timeMilli;//time since start of program
static float lastCounter_R, lastCounter_L;
static float errorSum_L, errorSum_R, pidout_L, pidout_R;
static float kpL, kiL, kdL, kpR, kiR, kdR;


//this gives me the rotations predicted based on speed. Dunno how to calculate yet.
float getExpectedRotations() {
    return 0.0;
}


void Compute() {
    float currentCounter_L = M1_QuadDec_GetCounter();
    float currentCounter_R = M2_QuadDec_GetCounter();
    
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

CY_ISR(timer_0) {
    Compute();
}

//prob dont need this since its always 1000
unsigned long getTime() {
    return timeMilli;
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

void pid_timer_init() {
    timeMilli = 0;//prob not needed
    lastTime = 0;//prob not needed
    lastCounter_L = 0;
    lastCounter_R = 0;
    errorSum_R = 0;
    errorSum_L = 0;
    
    isr_PID_timer_StartEx(timer_0);
    isr_PID_timer_Start();
    Timer_1_Start();
}