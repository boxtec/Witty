//Telec.h  Telec simple
// #include "Telec.h"  setup dans Witty.h

// Compte rend le nombre de pressions courtes
#define DelSil 100 
#define DelCnt 500
byte Compte () {
  byte cnt;
  while (!IrmOn) ;  // attend 1ere imp 
  int cDel=0; cnt=0; 
  while (cDel++<DelCnt) { 
    if (IrmOn) { LedOn; cDel=0;}  else {LedOff;}
   if (cDel==DelSil) { cnt++; } 
   DelMs (2); 
  }
  return (cnt);
}
