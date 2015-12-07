 #include "mbed.h"
 #include "mik.h"
 
#define H 1
#define L 0
 

 
int main() {
    /*
    PHASE on pin    PWM0    p26
    ENABLE on pin   PE0     p5
    MODE on pin     PE1     p6
    BRAKE on pin    PC0     p30
    VREF on pin     DAC0    p18
    MEAS_IN on pin  ADC0    p15
    */
    
    // Define pins
    DigitalOut ENABLE(p5);
    DigitalOut MODE(p6);
    DigitalOut BRAKE(p30);
    PwmOut PHASE(p26);
    AnalogOut VREF(p18);
    AnalogIn MEAS_IN(p15);      // Returns values between 0-1.0
    
    // Set Slow current decay mode
    BRAKE = H;
    ENABLE = L;
    MODE = L;
    VREF = 0.31f;   // 1 V     
        
    // Change here
    float pulswi=20;                        //Pulswidth in %    
    float freq = 20000;                     // Frequency in Hz                 
    // calculate
    float pertid = 1/freq;
    float measured = 0;

    clear_disp(); 

    while(1) {
        // Read ADC
        measured = MEAS_IN.read();
        // For use with changable PWM
        //pulswi = measured*100;
        PHASE.period(pertid);
        PHASE.pulsewidth(pertid*pulswi/100);
        // Write display
        move_cursor(1,1);
        dprintf("Frequency: %i Hz", int(freq));
        move_cursor(2,1);
        dprintf("PWM: %i %%  ", int(pulswi)); 
        move_cursor(3, 1);
        dprintf("ADC read: %f V", measured*3.3);
        }
}