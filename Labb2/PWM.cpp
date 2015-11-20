#include "mbed.h"

//------------------------------------
// Puts out a PWM with periodtime and dutycycle
//------------------------------------


PwmOut motor(p26);                        //Kolla portnr

float pertid=0.00005;                   // 20 kHz
float pulswi=70;                        //Pulswidth in %                     

 
int main() {
  
    motor.period(pertid);
    motor.pulsewidth(pertid*pulswi/100);
 
        
    }