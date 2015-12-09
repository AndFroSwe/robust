 #include "mbed.h"
 #include "mik.h"
 
#define H 1
#define L 0
 
 float amp(float volt){
     // Linear calculation of ampere 
     float k = 0.3373;
     float m = -0.72;
     
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
    float dty = 0;
    float dty_old = dty;
    
    // Change here
    float freq = 50000;                     // Frequency in Hz   
    // Calculate
    float pertid = 1/freq;
    //float low_lim = (100 - span)/2/100;    
    float i_max = 0.1;
    float i_min = -0.1;
    float P = 0.01;

    clear_disp(); 

    while(1) {
        // For use with changable PWM
        PHASE.period(pertid);
        PHASE.pulsewidth(pertid*dty);
        // Read ADC
        measured = MEAS_IN.read();
        control = CONTROL.read();
        // Calculate reference signal
        float ref = i_min + (i_max - i_min)*control;
        // Regulator
        float e = ref - amp(measured*3.3);
        // Regulation
        dty = dty_old - e*P;
        dty_old = dty;
        if (dty > 1){ dty = 1;}
        else if (dty < 0){ dty = 0;}
        // Write display
        /*
        move_cursor(1,1);
        dprintf("Ref: %f A", ref);
        move_cursor(2,1);
        dprintf("PWM: %i %%  ", int(dty*100)); 
        move_cursor(3, 1);
        dprintf("ADC read: %f V", measured*3.3);
        move_cursor(4, 1);
        dprintf("I: %f  A", amp(measured*3.3));
        */
        }
}