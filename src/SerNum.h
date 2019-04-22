// SerNum.h  Formats avec zéros  enlever
//Dans PP  #include "SerNum.h"  SetupSerNum(); 
void SetupSerNum() {
  UBRR0=F_CPU/(9600*16L)-1; // set speed 
  UCSR0B=0x18; // -- -- -- rxe txe -- -- --
  UCSR0C=0x06; // set mode: 8 data bits, no parity, 1 stop bit
}
//Rx S1 est toujours en sortie
//Tx S2 aussi, sous controle du 328 si TerOn (S2 ignoré);
#define TerOn UBRR0=F_CPU/(9600*16L)-1; UCSR0C=0x06; UCSR0B=0x08 
#define TerOff DelMs(3); UCSR0C=0x06; UCSR0B=0
 
void TerCar( byte dd) {  // write bloquant  Ok initial
  while(!(UCSR0A&0x20));// wait 
  UDR0=dd;  // OK, send it now!
}
void TerCR() {
  TerCar(13);TerCar(10);
}
void TerText(const char str[]) {
   for (byte i=0; i< strlen(str); i++) {
      TerCar(str[i]);
   }
}


void TerBin8 (byte bb) {
  for (byte i=0;i<8;i++) {
     if (bb&0x80) TerCar('1');
     else TerCar('0');
     bb <<= 1;
  }
  TerCar(' ');
}
char TerConvNibble (byte nn) { // converti 4 bit hexa en Ascii
  byte cc;
  if (nn<10) {cc = nn + '0';}
  else {cc = nn-10 + 'A';}
  return cc;
}
void TerHex8 (byte hh) {
  byte cc;
  cc = TerConvNibble (hh >> 4) ; // ne modifie pas hh
  TerCar(cc);
  cc = TerConvNibble (hh & 0x0F) ; 
  TerCar(cc);
  TerCar(' '); // space
}
void TerHex16 (uint16_t hh) {
  byte cc;
  cc = TerConvNibble (hh >> 12) ;   TerCar(cc);
  cc = TerConvNibble ((hh >> 8)&0x0F) ; TerCar(cc);
  cc = TerConvNibble ((hh >> 4)&0x0F) ; TerCar(cc);
  cc = TerConvNibble ((hh     )&0x0F) ;   TerCar(cc);
  TerCar(' ');
}
void TerHex12 (uint16_t hh) {
  byte cc=0;
  cc = TerConvNibble ((hh >> 8)&0x0F) ; TerCar(cc);
  cc = TerConvNibble ((hh >> 4)&0x0F) ; TerCar(cc);
  cc = TerConvNibble (hh &      0x0F) ;   TerCar(cc);
  TerCar(' ');
}
void TerHex20 (uint32_t hh) {
  byte cc;
  cc = TerConvNibble ((hh >> 16)&0x0F) ; TerCar(cc);
  cc = TerConvNibble ((hh >> 12)&0x0F) ; TerCar(cc);
  cc = TerConvNibble ((hh >> 8 )&0x0F) ;  TerCar(cc);
  cc = TerConvNibble ((hh >> 4 )&0x0F) ;  TerCar(cc);
  cc = TerConvNibble (hh &       0x0F) ;   TerCar(cc);
  TerCar(' ');
}
void TerHex32 (uint32_t hh) {
  byte cc;
  
  cc = TerConvNibble ((hh >> 28)&0x0F) ; TerCar(cc);
  cc = TerConvNibble ((hh >> 24)&0x0F) ; TerCar(cc);
  cc = TerConvNibble ((hh >> 20)&0x0F) ; TerCar(cc);
  cc = TerConvNibble ((hh >> 16)&0x0F) ; TerCar(cc);
  cc = TerConvNibble ((hh >> 12)&0x0F) ;   TerCar(cc);
  cc = TerConvNibble ((hh >> 8)&0x0F) ; TerCar(cc);
  cc = TerConvNibble ((hh >> 4)&0x0F) ; TerCar(cc);
  cc = TerConvNibble ((hh     )&0x0F) ;   TerCar(cc);
  TerCar(' ');
}  
uint16_t TerBinDec8 (uint8_t bb) {
  uint16_t dd=0;
   for (byte i=0; i<8; i++) {
    if ((dd & 0x0F)>0x04) {dd += 0x03;}
    if ((dd & 0xF0)>0x40) {dd += 0x30;}
    dd=dd<<1;
    if ((bb & 0x80)) {dd += 1;}  //inject bit
    bb=bb<<1; // prepare next bit
  }
  return dd;
}
/*#define TerDec8(v) \
    do { const int8_t vv = (v); \
      if(vv<0) { TerDec8S(vv); } \
      else { TerDec8U(vv); } \
    } while(0)
*/
void TerDec8 (uint8_t hh) {
  TerHex12(TerBinDec8(hh));
}

void TerDec8S (int8_t hh) {
  if (hh<0) {  // négatif
    TerCar('-'); hh = -hh;
  } else {
    TerCar('+');
  }
  TerHex12(TerBinDec8(hh));
}
uint16_t BinDec9999 (uint16_t bb) { //0x270F max
  uint32_t dd=0;
  for (byte i=0; i<16; i++) {
    if ((dd & 0x000F)>0x0004) {dd += 0x0003;}
    if ((dd & 0x00F0)>0x0040) {dd += 0x0030;}
    if ((dd & 0x0F00)>0x0400) {dd += 0x0300;}
    if ((dd & 0xF000)>0x4000) {dd += 0x3000;}
      dd=dd<<1;
      if ((bb & 0x8000)) {dd += 1; }  //inject bit
      bb<<=1; // prepare next bit
  }
  return dd;
}

uint32_t BinDec16 (uint16_t bb) { 
  uint32_t dd=0;
  for (byte i=0; i<16 ; i++) {
    if ((dd & 0x000F)>0x0004) {dd += 0x0003;}
    if ((dd & 0x00F0)>0x0040) {dd += 0x0030;}
    if ((dd & 0x0F00)>0x0400) {dd += 0x0300;}
    if ((dd & 0xF000)>0x4000) {dd += 0x3000;}
      dd=dd<<1;
      if ((bb & 0x8000)) {dd += 1; }  //inject bit
      bb<<=1; // prepare next bit
  }
  return dd;
}
void TerDec9999 (uint16_t hh) {  // limité à 0x2703
  if (hh>9999) { TerText("over "); } 
  else TerHex16(BinDec9999(hh));
}
void TerDec16 (uint32_t hh) {  // 
  TerHex20(BinDec16(hh));
}

void TerDec16S (int16_t hh) {  // signé
  if (hh<0) {  // negatif
    TerCar('-'); hh = -hh;
  } else {
    TerCar('+');
  }
  TerHex20(BinDec16(hh));
}

