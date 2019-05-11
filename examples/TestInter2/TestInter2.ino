//TestInter.ino Witty
// Clignotement continu par interruption

 
#include "Witty.h"
#include "Inter2.h"

void setup(){  
    SetupInter2();
}

void loop() { 
  for (byte i=10; i<200; i+=2) {
   period=i; DelMs(100);
  }
}

