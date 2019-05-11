//TestWitty.ino Test Button, Led Cligno and CliErr
#include "Witty.h" 
void setup() {
  SetupWitty();
  Cligno(3,200);
}

byte cDel=0;
void loop() { 
// Led stay on for 2 second, reactivate after <2 seconds with a push

  while (cDel++<200) { 
    LedOn;  DelMs(2000/200);  // 2 
    if (PushOn) {cDel=0;}   // check every 10ms
  }
  LedOff;
  DelMs(1000);
  
  Cligno(5,50);

  CliErr(2,200,1000);
// need a reset now
}

