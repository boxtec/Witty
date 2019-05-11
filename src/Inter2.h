//Inter2.h
//Begin main progr #include "Inter2.h"  SetupInter2();
volatile byte period;
volatile  uint16_t tt;
void DoCligno() {
  if (tt++ > period) {
    tt=0;  LedToggle;
  }
}
volatile byte cpf;
ISR (TIMER2_OVF_vect) {
 TCNT2 = 141;  //  58 us period
  if (cpf++ > 18-1) { cpf=0; // toutes les 18x58= ~1ms 
    DoCligno();  // durée <2us
  }
} 

void SetupInter2() {  // initialisation
  TCCR2A = 0; //default
  TCCR2B = 0b00000010;  //  2MHz
  TIMSK2 = 0b00000001;  // TOIE2
  sei();  // Interrupt enabled
}

