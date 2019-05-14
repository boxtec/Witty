//Witty.h 190331  ref voir www.didel.com/WittySoft.pdf 
//Dans PP  #include "Witty.h"  SetupWitty(); 

#define bLed 1 // PORTC
#define bPush 0  // actif à zero
#define PushOn (!(PINC&(1<<bPush)))  // pull-up int 
#define LedOn bitSet (PORTC,bLed)
#define LedOff bitClear (PORTC,bLed)
#define LedToggle (PORTC^=(1<<bLed))  
//Module IR - voir Telec.h et www.didel.com/TelecommandeIrSimple.pdf 
#define bIrm 2 // on PORTD int
#define IrmOn !(PIND&(1<<bIrm))

//Moteurs;
#define bAvD  6 
#define bRecD 7   
#define AvD   bitSet    (PORTD,bAvD); bitClear (PORTD,bRecD)
#define RecD  bitClear  (PORTD,bAvD);  bitSet  (PORTD,bRecD)
#define FreeD  bitClear (PORTD,bAvD); bitClear (PORTD,bRecD)
#define BlockD   bitSet  (PORTD,bAvD);  bitSet  (PORTD,bRecD)
#define bAvG  5 
#define bRecG 4   
#define AvG  bitSet    (PORTD,bAvG); bitClear (PORTD,bRecG)
#define RecG bitClear  (PORTD,bAvG);  bitSet  (PORTD,bRecG)
#define FreeG bitClear (PORTD,bAvG); bitClear (PORTD,bRecG) // TEST
#define BlockG   bitSet (PORTD,bAvG);  bitSet  (PORTD,bRecG)
#define StopG BlockG 
#define StopD BlockD 

#define bIrm 2 // on PORTD int
#define IrmOn !(PIND&(1<<bIrm))

#define bLed 1 // PORTC
#define bPous 0  // actif à zero
#define PousOn (!(PINC&(1<<bPous)))  // pull-up
#define LedOn bitSet (PORTC,bLed)
#define LedOff bitClear (PORTC,bLed)
#define LedToggle (PORTC^=(1<<bLed))  

#define nop asm ("nop")
void DelMs (uint16_t dm) {   //172-152
   for (uint16_t i=0; i<dm; i++) {
      for (uint16_t j=0; j<3200; j++) {nop;}
   }
}
void Delus (uint16_t dm) {  // 182-152
     for (uint16_t i=0; i<dm; i++) {nop;nop;nop;nop;}
     for (uint16_t i=0; i<dm; i++) {nop;nop;nop;}
}

void Cligno(byte nn,int dd) {
  for (byte i=0; i<2*nn; i++) { LedToggle, DelMs(dd);}
}
void CliErr (byte ct,int dd, int ww) {
  while(1) {
     Cligno (ct,dd);
     DelMs (ww);
  }
}


void  SetupWitty() {
  DDRD  = 0b11111000;  // rien sur pd3?
  DDRC  = 0b000010; // Led
  PORTC = 0b000001; // pullup Push 
  DDRB= 0x03;  // PB0 1 for Apa102
  LedOn; DelMs(100); LedOff;
}
