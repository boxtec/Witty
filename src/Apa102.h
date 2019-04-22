// Apa102.h  ref 190303ino  Doc  Apa.pdf
// #include "Apa102.h"   SetupApa102();  dans PP
#define nop asm ("nop")
#define  bApaCk 1
#define  bApaDa 0
#define ApaCkOn  bitSet (PORTB,bApaCk)
#define ApaCkOff bitClear (PORTB,bApaCk)
#define ApaDaOn bitSet (PORTB,bApaDa)
#define ApaDaOff bitClear (PORTB,bApaDa)
#define ApaCkPulse bitSet(PORTB,bApaCk); bitClear(PORTB,bApaCk)

void SetupApa102() {
  DDRB |= (1<<bApaCk)+(1<<bApaDa) ;
}

// ne pas utiliser delay() 
#define Calib1ms 900  // 1ms  16 MHz
void delMs (int dm) { 
   for (volatile int i=0; i<dm; i++) {
      for (volatile int j=0; j<Calib1ms; j++) {}
   }
}

void ApaS8 (uint8_t dd) {
  for (uint8_t i=0;i<8;i++) {
    if (dd&0x80) ApaDaOn;  else  ApaDaOff;
    ApaCkOn; dd<<=1; ApaCkOff;  // durée 0.25 us
  }
  ApaDaOff;
}

void ApaHead () {
  ApaS8(0); ApaS8(0); ApaS8(0); ApaS8(0);
}
void ApaLed (uint8_t rr,uint8_t gg,uint8_t bb) {
  ApaS8(224+Brt); ApaS8(bb); ApaS8(gg); ApaS8(rr);
}
void ApaTail () {
   for (byte i=0; i<Npix/2; i++) {ApaCkOn; asm("nop"); ApaCkOff; }
}

void ApaClear () {
   ApaHead ();
   for (byte i=0; i<Npix; i++) { ApaLed (0,0,0); }
   for (byte i=0; i<Npix/2; i++) {ApaCkOn; asm("nop"); ApaCkOff; }
}
void ApaRGB (uint8_t rr,uint8_t gg,uint8_t bb) {
   ApaHead ();
   for (byte i=0; i<Npix; i++) { ApaLed (rr,gg,bb); }
   for (byte i=0; i<Npix/2; i++) {ApaCkOn; asm("nop"); ApaCkOff; }
}

byte taApaConv [16]={0,1,2,4, 6,9,12,17, 27,37,53,78, 100,144,198,255}; 

byte ApaConv (byte vv) {
  return taApaConv [vv/8];
}

void ApaLogRGB( byte bb, byte gg, byte rr ) {
  ApaS8(224+Brt); ApaS8(taApaConv [bb/8]); 
  ApaS8(taApaConv [gg/8]); ApaS8(taApaConv [rr/8]);
}
volatile byte lu; 
void ApaLogRed (uint8_t log) {
  lu= taApaConv[log]; ApaRGB (lu,0,0);
}
void ApaLogGreen (uint8_t log) {
  lu= taApaConv[log]; ApaRGB (0,lu,0);
}
void ApaLogBlue (uint8_t log) {
  lu=taApaConv[log]; ApaRGB (0,0,lu);
}
void ApaLogYellow (uint8_t log) {
  lu=taApaConv[log]; ApaRGB (lu,lu,0);
}
void ApaLogPink (uint8_t log) {
  lu=taApaConv[log]; ApaRGB (lu,0,lu);
}
void ApaLogCyan (uint8_t log) {
  lu=taApaConv[log]; ApaRGB (0,lu,lu);
}
void ApaLogWhite (uint8_t log) {
  lu=taApaConv[log]; ApaRGB (lu,lu,lu);
}

// HtoRGB.h ancien définitivement ici  voir doc

uint16_t zr,zg,zb; // globales
uint16_t xr,xg,xb; 
byte e;
void ApaHtoRGB (uint8_t h) {
  e = h/32 ;  // 8 zones
  switch (e) {
    case 0: xr=(96-h)*8   ; xg=h*8  ;       xb=0  ; break;
    case 1: xr=160*3      ; xg=h*8  ;       xb=0  ; break;
    case 2: xr=(96-h)*16  ; xg=h*8  ;       xb=0  ; break;
    case 3: xr=0          ; xg=(192-h)*8  ; xb=(h-96)*8   ; break;
    case 4: xr=0          ; xg=(160-h)*16 ; xb=(h-112)*16 ; break;
    case 5: xr=(h-160)*8  ; xg= 0 ;         xb=(256-h)*8  ; break;
    case 6: xr=(h-160)*8  ; xg= 0 ;         xb=(256-h)*8  ; break;
    case 7: xr=(h-160)*8  ; xg= 0 ;         xb=(256-h)*8  ; break;
  }
  zr=(xr*85)/256; zg=(xg*85)/256; zb=(xb*85)/256;
}

void ApaHue (byte hh) {
  e = hh/32 ;  // 8 zones
  switch (e) {
    case 0: xr=(96-hh)*8  ; xg=hh*8  ;       xb=0  ; break;
    case 1: xr=160*3      ; xg=hh*8  ;       xb=0  ; break;
    case 2: xr=(96-hh)*16 ; xg=hh*8  ;       xb=0  ; break;
    case 3: xr=0          ; xg=(192-hh)*8  ; xb=(hh-96)*8   ; break;
    case 4: xr=0          ; xg=(160-hh)*16 ; xb=(hh-112)*16 ; break;
    case 5: xr=(hh-160)*8 ; xg= 0 ;         xb=(256-hh)*8  ; break;
    case 6: xr=(hh-160)*8 ; xg= 0 ;         xb=(256-hh)*8  ;break;
    case 7: xr=(hh-160)*8 ; xg= 0 ;         xb=(256-hh)*8  ; break;
  }
  ApaRGB (xr,xg,xb);
}
void ApaRainbow() {  // exemple joli, manque d'orange
    ApaHead(); 
   for (byte i=1; i<Npix+1; i++) {
     ApaHtoRGB(i*32);   // 10x25 
     ApaLed (zr,zg,zb);
   } 
   ApaTail();
}
// fin de Apa102.h

  
