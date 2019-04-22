//Inter2.h
//Dans PP  #include "Inter2.h"  SetupInter2();
volatile byte cpf,cup,cdo;
ISR (TIMER2_OVF_vect) {
// TCNT2 = 256-20;  // 2M  20 11us 100 35us  200 85us
 TCNT2 = 141;  //  58 us calibre pour Tell
  cup++;cdo++;
  if (cpf++ > 35) { cpf=0; // toutes les 35x58= 2ms 
    DoPfm ();  //durée 2us
  }
} 

void SetupInter2() {  // initialisation
  TCCR2A = 0; //default
  TCCR2B = 0b00000010;  //  2MHz
  TIMSK2 = 0b00000001;  // TOIE2
  //TIFR2  flag  TOV2
  sei();
}

