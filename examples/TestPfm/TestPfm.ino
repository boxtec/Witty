//TestMotPfm.ino witty
// On accélère et ralenti les moteurs
// la vitesse max est toujours +/- 80
#include "Witty.h"
#include "Pfm.h"
#include "Inter2.h"

void setup(){  
    SetupWitty();
    SetupInter2();
}

#define MaxVit 70
void loop() { 
  LedOff;
  for (pfmR=0; pfmR<MaxVit; pfmR++) {pfmL=pfmR; DelMs(20);}
  LedOn;
  for (pfmR=MaxVit; pfmR>0; pfmR--) {pfmL=pfmR; DelMs(20);}
  LedOff;
  for (pfmR=0; pfmR>-MaxVit; pfmR--) {pfmL=pfmR; DelMs(20);}
  LedOn;
  for (pfmR=-MaxVit; pfmR<0; pfmR++) {pfmL=pfmR; DelMs(20);}
  LedOff;
  pfmL=1; pfmR=1; DelMs(2000);
  pfmL=-1; pfmR=-1; DelMs(2000);
}

