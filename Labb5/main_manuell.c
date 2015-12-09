 #include "mbed.h"
 #include "mik.h"
 
#define H 1
#define L 0

 float amp(float volt){
     // Linear calculation of ampere 
     float k = 0.3373;
     float m = -0.6133;
     
     return k*volt + m;
     }

int main() {
    /*
    PHASE on pin    PWM0    p26
    ENABLE on pin   PE0     p5
    MODE on pin     PE1     p6
    BRAKE on pin    PC0     p30
    VREF on pin     DAC0    p18
    MEAS_IN on pin  ADC0    p15
    CONTROL_POT_IN  ADC1    p16
    */
    
    // Define pins
    DigitalOut ENABLE(p5);
    DigitalOut MODE(p6);
    DigitalOut BRAKE(p30);
    PwmOut PHASE(p26);
    AnalogOut VREF(p18);
    AnalogIn MEAS_IN(p15);      // Returns values between 0-1.0
    AnalogIn CONTROL(p16);      // For connection to a pot
    
    // Set Slow current decay mode
    BRAKE = H;
    ENABLE = L;
    MODE = L;
    VREF = 0.31f;   // 1 V     
        
    // Initiate
    float measured = 0;
    float control = 0;
    float dty = 0.5;
    float dty_old = dty;
    
    // Change here
    float freq = 50000;                     // Frequency in Hz   
    float span = 90;                        // Evenly distributed span [%]
    // Calculate
    float pertid = 1/freq;
    float low_lim = (100 - span)/2/100;         

    clear_disp(); 

    while(1) {
        
        
        
        // Read ADC
        measured = MEAS_IN.read();
        control = CONTROL.read();
        // Calculate Duty Cycle with limited span
        float dty = low_lim + control*span/100;
        
        // For use with changable PWM
        PHASE.period(pertid);
        PHASE.pulsewidth(pertid*dty);
    
        // Write display
        move_cursor(1,1);
        dprintf("Frequency: %i Hz", int(freq));
        move_cursor(2,1);
        dprintf("PWM: %i %%  ", int(dty*100)); 
        move_cursor(3, 1);
        dprintf("ADC read: %f V", measured*3.3);
        move_cursor(4, 1);
        dprintf("I: %f  A", amp(measured*3.3));
        }
}