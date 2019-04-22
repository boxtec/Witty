//Pfm.h  avec table vit
//#include "Pfm.h"  SetupPfm();  dans le pp
volatile int8_t  pfmL, pfmR;  // -80 .. +80
#define MaxPfm 80 
volatile byte pfmCntL;
volatile byte pfmCntR;
void DoPfm () {
  char tempL,tempR;
  if (pfmR > MaxPfm) pfmR=MaxPfm; // saturer
  if (pfmR < -MaxPfm) pfmR= -MaxPfm;
  
  if (pfmR >=0) {
     if ((pfmCntR += pfmR) > MaxPfm) {
        pfmCntR -= MaxPfm;   AvD;    }
     else { FreeD; }
   }
  if (pfmR <0) {
     tempR= -pfmR;  // ou pfR= ABS(pfR)
     if ((pfmCntR += tempR) > MaxPfm) {
        pfmCntR -= MaxPfm;   RecD;    }
     else { FreeD; }
   }
  if (pfmL > MaxPfm) pfmL=MaxPfm; // saturer
  if (pfmL < -MaxPfm) pfmL= -MaxPfm;
  
  if (pfmL >=0) {
     if ((pfmCntL += pfmL) > MaxPfm) {
        pfmCntL -= MaxPfm;   AvG;    }
     else { FreeG; }
  }
  if (pfmL <0) {
     tempL= -pfmL;  // ou pfL= ABS(pfL)
     if ((pfmCntL += tempL) > MaxPfm) {
        pfmCntL -= MaxPfm;   RecG;    }
     else { FreeG; }
   }
}


