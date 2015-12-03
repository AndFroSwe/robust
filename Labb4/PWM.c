 #include "mbed.h"
#define H 1
#define L 0
 
int main() {
    /*
    PHASE on pin    PWM0    p26
    ENABLE on pin   PE0     p5
    MODE on pin     PE1     p6
    BRAKE on pin    PC0     p30
    VREF on pin     DAC0    p18
    
    */
    
    // Define pins
    DigitalOut ENABLE(p5);
    DigitalOut MODE(p6);
    DigitalOut BRAKE(p30);
    PwmOut PHASE(p26);
    AnalogOut VREF(p18);
    
    // Set Slow current decay mode
    BRAKE = H;
    ENABLE = L;
    MODE = L;
    
        
    // Change here
    float pulswi=30;                        //Pulswidth in %    
    float freq = 20000;                     // Frequency in Hz                 
    // calculate
    float pertid = 1/freq;

    // Start PWM
    //int duty_cycle = 30;
 
    VREF = 0.31f;   // 1 V   
    PHASE.period(pertid);
    PHASE.pulsewidth(pertid*pulswi/100);
}