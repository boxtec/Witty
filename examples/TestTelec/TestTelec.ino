//TestTelec.ino Blink number of pulses 

#include "Witty.h"
#include "Telec.h"
void setup () { 
  SetupWitty();
}

byte nPulses;
void loop() {  
 nPulses= CompteIR();
 Cligno (nPulses,1000);
}

/*
 Modify the timings on Telec.h
 Control the motos as in Demo2 program.
 */
