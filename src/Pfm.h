//Pfm.h  avec table vit
//#include "Pfm.h"  SetupPfm();  dans le pp
volatile char vitL, vitR;  // -15..+15 saturé dans inter
volatile char pfmL, pfmR;  // -80 .. +80
//#define MaxVit 15
#define MaxPfm 80 
char taVitR []= {-80,-70,-60,-51,-43,-35,-28,-22,-18,-14,-11,-8,-5,-2,0,\
                  0,0,2,5,8,11,14,18,22,28,35,43,51,60,70,80};
char taVitL []= {-80,-70,-60,-51,-43,-35,-28,-22,-18,-14,-11,-8,-5,-2,0,\
                  0,0,2,5,8,11,14,18,22,28,35,43,51,60,70,80};

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


