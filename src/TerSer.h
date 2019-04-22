// TerSer.h  4 modes  190323 
// ~280 lines ~7700 car
//add to main program  #include "TerSer.h"  SetupTerSer();
// or  from Arduino lib #include <TerSer.h>  SetupTerSer();
void SetupTerSer() { 
  UBRR0= 103;  // 9600
  UCSR0B=0x18; // -- -- -- rxe txe -- -- --
  UCSR0C=0x06; // set mode: 8 data bits, no parity, 1 stop bit
}

uint8_t moz=0;  // Default value
#define Normal moz=0
#define Spaces moz=1
#define Zeros  moz=2
#define Compact moz=3

uint8_t Get() {  // read blocking
  while (!(UCSR0A&0x80)); // wait
  return UDR0; 
}

void Car( uint8_t dd) {  // write blocking
  while(!(UCSR0A&0x20));// wait 
  UDR0=dd;  // OK, send it now!
}

void CR() {
  Car(13);Car(10);  // CR LF
}

void Text(const char str[]) {
  for (uint8_t i=0; i< strlen(str); i++) { Car(str[i]); }
}

void Textln(const char str[]) {
   for (uint8_t i=0; i< strlen(str); i++) { Car(str[i]); }
   Car(13);Car(10);
}

void TtBin8 (uint8_t bb) {  // auxiliary function
  for (uint8_t i=0;i<8;i++) {
     if (bb&0x80) Car('1');
     else Car('0');
     bb <<= 1;
  }
}
void Bin8 (uint8_t bb) {
  TtBin8 (bb); Car(' ');
}
void Bin16 (uint16_t bb) {
 TtBin8 (bb>>8); Car('.');
 TtBin8 (bb&0xFF);
 Car(' ');
}

char ConvNibble (uint8_t nn) { //  4 bit binary to Ascii
  uint8_t cc;
  if (nn<10) {cc = nn + '0';}
  else {cc = nn-10 + 'A';}
  return cc;
}

void Hex8 (uint8_t hh) {
  uint8_t cc;
  cc = ConvNibble (hh >> 4);  Car(cc);
  cc = ConvNibble (hh & 0x0F) ; Car(cc);
  Car(' '); // space
}
void Hex16 (uint16_t hh) {
  uint8_t cc;
  cc = ConvNibble (hh >> 12) ;  Car(cc);
  cc = ConvNibble ((hh >> 8)&0x0F) ; Car(cc);
  cc=hh&0xFF; Hex8 (cc);
  Car(' ');
}
void Hex32 (uint32_t hh) {
  uint8_t cc;
  cc = ConvNibble (hh >> 28) ;  Car(cc);
  cc = ConvNibble ((hh >> 24)&0x0F) ; Car(cc);
  cc = ConvNibble ((hh >> 20)&0x0F) ; Car(cc);
  cc = ConvNibble ((hh >> 16)&0x0F) ; Car(cc);
  cc=hh&0xFFFF; Hex16 (cc);
  Car(' ');
}

// -- new Decimal functions with 4 modes (see above)
   
uint8_t uni,diz,cen,mil,dim;
uint8_t tyn;  // type number
uint8_t sig;
 
void BinDec8 (uint8_t bb) {
  uni=0;diz=0;cen=0;
  tyn=0; if (bb==0) return;
  uni=bb%10; bb=bb/10; if (bb==0){tyn=1;return;}
  diz=bb%10; bb=bb/10; if (bb==0){tyn=2;return;}
  cen=bb; tyn=3;
}
 
void Af3Dig() {
 switch (moz) {
  case 0: // Normal  signe devant le nombre
  switch (tyn) {
    case 0:
     Car(' '); Car(' ');Car(' ');Car('0');  Car(' ');  break;
    case 1:
     Car(' ');Car(' '); Car(sig); Car(uni+'0'); Car(' ');  break;
    case 2:
     Car(' '); Car(sig); Car(diz+'0'); Car(uni+'0'); Car(' ');   break;
    case 3:
     Car(sig); Car(cen+'0'); Car(diz+'0'); Car(uni+'0'); Car(' ');  break;
   } // end switch tyn
   break; // end case moz 0
    case 1: // Spaces
    switch (tyn) { 
     case 0:
      Car(' '); Car(' ');Car(' ');Car('0');  Car(' '); break;
     case 1:
      Car(sig);Car(' '); Car(' '); Car(uni+'0'); Car(' '); break;
     case 2:
      Car(sig); Car(' '); Car(diz+'0'); Car(uni+'0'); Car(' ');  break;
     case 3:
      Car(sig); Car(cen+'0'); Car(diz+'0'); Car(uni+'0'); Car(' ');  break;
    } // end switch tyn
  break;  // end case moz 1
    case 2: // Zeros
    switch (tyn) {
     case 0:
      Car(' '); Car('0');Car('0');Car('0'); Car(' '); break;
     case 1:
      Car(sig);Car('0'); Car('0'); Car(uni+'0'); Car(' '); break;
     case 2:
      Car(sig); Car('0'); Car(diz+'0'); Car(uni+'0'); Car(' ');  break;
     case 3:
      Car(sig); Car(cen+'0'); Car(diz+'0'); Car(uni+'0'); Car(' ');  break;
   } // end switch tyn
   break; // end case moz 2
    case 3: // Compact
    switch (tyn) {
     case 0:
      Car('0');  Car(' ');  break;
     case 1:
      Car(sig); Car(uni+'0'); Car(' ');  break;
     case 2:
      Car(sig); Car(diz+'0'); Car(uni+'0'); Car(' ');   break;
     case 3:
      Car(sig); Car(cen+'0'); Car(diz+'0'); Car(uni+'0'); Car(' ');  break;
    } // end switch tyn
    break; // endcase moz 3
 }  // end switch moz
}
// this macro generate a warning depending on sign
#define Dec8(v)                 \
    do { typeof(v) _v = -1;     \
      if (_v < 0) { Dec8s(v); } \
       else { Dec8u(v); }       \
    } while(0);

void Dec8u (uint8_t hh) {
   sig=' ';
   BinDec8 (hh); Af3Dig();
}

void Dec8s (int8_t hh) {
  if (hh<0) {  // négatif
    sig='-'; hh = -hh;
  } else {
    sig='+';
  }
  BinDec8 (hh); Af3Dig();
}

//-----------------
void BinDec16 (uint32_t bb) { 
  uni=0;diz=0;cen=0;mil=0;dim=0;
  tyn=0; if (bb==0) return;
  uni=bb%10; bb=bb/10; if (bb==0){tyn=1;return;}
  diz=bb%10; bb=bb/10; if (bb==0){tyn=2;return;}
  cen=bb%10; bb=bb/10; if (bb==0){tyn=3;return;}
  mil=bb%10; bb=bb/10; if (bb==0){tyn=4;return;}
  dim=bb; tyn=5;
}

void Af5Dig() {
 switch (moz) {
  case 0: // Normal mode
  switch (tyn) {
    case 0:
     Car(' '); Car(' '); Car(' ');Car(' ');Car(' ');Car('0');  Car(' ');  break;
    case 1:
     Car(' '); Car(' '); Car(' ');Car(' ');Car(sig);Car(uni+'0'); Car(' ');  break;
    case 2:
     Car(' '); Car(' '); Car(' ');Car(sig);Car(diz+'0');Car(uni+'0'); Car(' ');   break;
    case 3:
     Car(' '); Car(' '); Car(sig);Car(cen+'0');Car(diz+'0');Car(uni+'0'); Car(' ');   break;
    case 4:
     Car(' '); Car(sig); Car(mil+'0');Car(cen+'0');Car(diz+'0');Car(uni+'0'); Car(' ');   break;    
    case 5:
     Car(sig); Car(dim+'0'); Car(mil+'0');Car(cen+'0');Car(diz+'0');Car(uni+'0'); Car(' ');  break;
  } // end switch tyn
  break;
  case 1: // Spaces
   switch (tyn) { 
    case 0: 
     Car(' '); Car(' ');Car(' ');Car(' ');Car(' ');Car('0');  Car(' '); break;
    case 1:
     Car(sig);Car(' '); Car(' ');Car(' ');Car(' '); Car(uni+'0'); Car(' '); break;
    case 2:
     Car(sig); Car(' '); Car(' ');Car(' ');Car(diz+'0'); Car(uni+'0'); Car(' ');  break;
    case 3:
     Car(sig); Car(' '); Car(cen+'0'); Car(diz+'0'); Car(uni+'0'); Car(' ');  break;
    case 4:
     Car(sig); Car(' ');Car(mil+'0');Car(cen+'0'); Car(diz+'0'); Car(uni+'0'); Car(' ');  break;
    case 5:
     Car(sig); Car(dim+'0');Car(mil+'0');Car(cen+'0'); Car(diz+'0'); Car(uni+'0'); Car(' ');  break;
   } // end switch tyn
  break;  // case 1
    case 2: // Zeros
    switch (tyn) {
    case 0:
     Car(' '); Car('0');Car('0');Car('0');Car('0');Car('0'); Car(' '); break;
    case 1:
     Car(sig); Car('0');Car('0');Car('0'); Car('0'); Car(uni+'0'); Car(' '); break;
    case 2:
     Car(sig); Car('0');Car('0');Car('0'); Car(diz+'0'); Car(uni+'0'); Car(' ');  break;
    case 3:
     Car(sig); Car('0');Car('0');Car(cen+'0'); Car(diz+'0'); Car(uni+'0'); Car(' ');  break;
    case 4:
     Car(sig); Car('0');Car(mil+'0');Car(cen+'0'); Car(diz+'0'); Car(uni+'0'); Car(' ');  break;
    case 5:
     Car(sig); Car(dim+'0');Car(mil+'0');Car(cen+'0'); Car(diz+'0'); Car(uni+'0'); Car(' ');  break;
   } // end switch tyn
   break;
    case 3: // Compact
     switch (tyn) {
      case 0:
       Car('0');  Car(' ');  break;
      case 1:
       Car(sig); Car(uni+'0'); Car(' ');  break;
      case 2:
       Car(sig); Car(diz+'0'); Car(uni+'0'); Car(' ');   break;
      case 3:
       Car(sig); Car(cen+'0'); Car(diz+'0'); Car(uni+'0'); Car(' ');  break;
      case 4:
       Car(sig); Car(mil+'0');Car(cen+'0'); Car(diz+'0'); Car(uni+'0'); Car(' ');  break;
      case 5:
       Car(sig); Car(dim+'0');Car(mil+'0');Car(cen+'0'); Car(diz+'0'); Car(uni+'0'); Car(' ');  break;    
     } // end switch tyn
     break;
   }  // end switch moz
}

// this macro generate a warning depending on sign
#define Dec16(v)                 \
    do { typeof(v) _v = -1;      \
      if (_v < 0) { Dec16s(v); } \
       else { Dec16u(v); }       \
    } while(0);

void Dec16u (uint16_t hh) {
   sig=' ';
   BinDec16(hh); Af5Dig();
}

void Dec16s (int16_t hh) {
  if (hh<0) {  // négatif
    sig='-'; hh = -hh;
  } else {
    sig='+';
  }
  BinDec16(hh); Af5Dig();
}
// end code


