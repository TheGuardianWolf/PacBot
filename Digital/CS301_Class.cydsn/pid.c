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
static double lastCounter_R, lastCounter_L;
static double errorSum_L, errorSum_R, pidout_L, pidout_R;
static double kpL, kiL, kdL, kpR, kiR, kdR;


//this gives me the rotations predicted based on speed. Dunno how to calculate yet.
double getExpectedRotations() {
    return 0;
}


void Compute() {
    double currentCounter_L = M1_QuadDec_GetCounter();
    double currentCounter_R = M2_QuadDec_GetCounter();
    
    //for proportional part
    double error_L = getExpectedRotations() - currentCounter_L;
    double error_R = getExpectedRotations() - currentCounter_R;
    
    // for integral part
    errorSum_L += error_L;
    errorSum_R += error_R;
    
    //for differentiatial part
    double dCounter_L = currentCounter_L - lastCounter_L;
    double dCounter_R = currentCounter_R - lastCounter_R;
    
    pidout_L = kpL * error_L + kiL * errorSum_L - kdL * dCounter_L;
    pidout_R = kpR * error_R + kiR * errorSum_R - kdR * dCounter_R;
    
    lastCounter_L = currentCounter_L;
    lastCounter_R = currentCounter_R;
    
    //probably need getters and setters in motor.c to get the current speeds
    double currentspeedright, currentspeedleft = 0;
    motor_set(currentspeedleft * pidout_L, currentspeedright * pidout_R);
}

CY_ISR(isr_PID_timer_Interrupt) {
    Compute();
}

//prob dont need this since its always 1000
unsigned long getTime() {
    return timeMilli;
}

void setPIDL(double KpL, double KiL, double KdL) {
    kpL = KpL;
    kiL = KiL;
    kdL = KdL;
}

void setPIDR(double KpR, double KiR, double KdR) {
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
    
    isr_PID_timer_StartEx(isr_PID_timer_Interrupt);
    isr_PID_timer_Start();
    Timer_1_Start();
}