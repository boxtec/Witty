//DemoWittyPfm.h  #include "DemoWittyPfm.h"
// erreur d'avoir fusionner
#include "Pfm.h"
#include "Inter2.h"
    
SetupInter2();  // set interrupts
     #define D2 DelMs(1000)
     #define D5 DelMs(50)
       pfmL=2; pfmR=2; D2;
       pfmL=-2; pfmR=-2; D2;
       pfmL=0; pfmR=0; D2;  
       pfmL=80; pfmR=80; D5;
       pfmL=0; pfmR=0; D2;      
       pfmL=-80; pfmR=-80; D5;
       pfmL=0; pfmR=0; D2; 

  #define MaxVit 40
  #define MaxVit2 10
#define Dac DelMs(20)
  for (pfmR=0; pfmR<MaxVit; pfmR++) {pfmL=pfmR; Dac;}
  for (pfmR=MaxVit; pfmR>0; pfmR--) {pfmL=pfmR; Dac;}
  for (pfmR=0; pfmR>-MaxVit; pfmR--) {pfmL=pfmR; Dac;}
  for (pfmR=-MaxVit; pfmR<0; pfmR++) {pfmL=pfmR; Dac;}
  D2;
  #define Dacf DelMs(10)
  for (pfmR=0; pfmR<MaxVit2; pfmR++) {pfmL=pfmR; Dacf;}
  for (pfmR=MaxVit2; pfmR>0; pfmR--) {pfmL=pfmR; Dacf;}
  for (pfmR=0; pfmR>-MaxVit2; pfmR--) {pfmL=pfmR; Dacf;}
  for (pfmR=-MaxVit2; pfmR<0; pfmR++) {pfmL=pfmR; Dacf;}
  D2;
	
//break, fin while
