//Glis.h  sliding average on 16-bit variables
//#include "Glis.h"  IniGlis1,IniGlis2 (valeur); 
 #define Lglis 8  // Buffer length
 int32_t  tot1,tot2; 
 int16_t taGlis1[Lglis], taGlis2[Lglis]; 
 uint8_t ptTaGlis1, ptTaGlis2; 
 
void IniGlis1 (int16_t dd) {  //prepare table
  for (byte i=0; i<Lglis; i++)  { 
    taGlis1[i]= dd; 
    tot1 += dd; 
  }
}
void IniGlis2 (int16_t dd) {
  for (byte i=0; i<Lglis; i++)  { 
    taGlis2[i]= dd; 
    tot2 += dd; 
  }
}

  int16_t Glis1 (int16_t dd) {  // new measure in, average out
  tot1 -= taGlis1[ptTaGlis1]; 
  tot1 += dd;                 
  taGlis1[ptTaGlis1] = dd;     
  if (++ptTaGlis1==Lglis) {ptTaGlis1=0;}  
  return (tot1/Lglis);
}
  int16_t Glis2 (int16_t dd) {  // new measure in, average out
  tot2 -= taGlis2[ptTaGlis2];  
  tot2 += dd;                
  taGlis2[ptTaGlis2] = dd;      
  if (++ptTaGlis2==Lglis) {ptTaGlis2=0;}  
  return (tot2/Lglis);
}
