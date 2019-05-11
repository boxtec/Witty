//#include "I2Ctwi.h"  TWI et fonctions composées
// inside PP #include "I2Ctwi.h"  SetupI2Ctwi();
#include <Arduino.h>  // pas nécessaire?
// aadd defini dans pp avec #define aadd valeur adresse 8 bits sélect
// ? changera avec paramètre explicite? ou nom G521Twi
byte status;
void SetupI2Ctwi() {
  TWSR = 1;  //0 400kHz 1-160k 2 40k
  TWBR = 0x0C; // bitrate
  TWCR = (1 << TWEN); // autres bits à 0
}

void  TwStart () {
  TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN); // start
  while (!(TWCR & (1 << TWINT))) {}
}
void  TwReStart () {
  TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN); // start
  while (!(TWCR & (1 << TWINT))) {}
}
void  TwStop () {
  TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN); // stop
  for (volatile int i = 0; i < 20; i++) ;
}
void  TwWrite (byte ab) {  // addr8 ou data8
  TWDR = ab;
  TWCR = (1 << TWINT) | (1 << TWEN);
  while (!(TWCR & (1 << TWINT))) {}
  ;  status = TWSR & 0xF8;
}
byte TwReadAck () {  
  TWCR = (1 << TWINT) | (1 << TWEN) |(1<<TWEA);
  while (!(TWCR & (1 << TWINT))) {}
  return (TWDR);
}
byte TwReadNack () {  
  TWCR = (1 << TWINT) | (1 << TWEN); 
  while (!(TWCR & (1 << TWINT))) {}
  return (TWDR);
}
// Fonctions composées pour aadd -------------------------------
void WriteByte(byte dd) {
  TwStart(); TwWrite(aadd); TwWrite(dd); TwStop();
}
void WriteByteAt(byte rr,byte dd) {
  TwStart(); TwWrite(aadd); TwWrite(rr); TwWrite(dd); TwStop();
}
void WriteWordAt(byte rr,uint16_t dd) {  //Little Endian
  TwStart(); TwWrite(aadd); TwWrite(rr); TwWrite(dd>>8); TwWrite(dd&0xFF); TwStop();
}
byte ReadByte() {
  byte dd;
  TwStart(); TwWrite(aadd+1); dd=TwReadNack();  TwStop();
  return (dd);
}
byte ReadWord() {
  byte dd;
  TwStart(); TwWrite(aadd+1); dd=(TwReadAck()<<8); dd|= TwReadNack(); TwStop();
  return (dd);
}
uint8_t ReadByteAt(byte rr) {
  byte dd;
  TwStart(); TwWrite(aadd); TwWrite(rr); 
  TwReStart(); TwWrite(aadd+1); dd=TwReadNack(); TwStop();
  return(dd);
}
uint16_t ReadWordAt(byte rr) {
  uint16_t dd; 
  TwStart(); TwWrite(aadd); TwWrite(rr); 
  TwReStart(); TwWrite(aadd+1); dd=(TwReadAck()<<8); dd |=(TwReadNack()); TwStop();
  return(dd);
}



