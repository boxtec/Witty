 // WittyDemo2.ino 190419  4434b 112v  !! 2 copies ajouté effet lum 
// welcome with Rainbow and fast blink push n times
// 1 push red several move on-off and back to menu
// 2 pushes green same with pfm speed changes
// 3 control blue with most TV IR 1pulse stop, 2,3,4,5 move
// 4 demonstrate log dim
// 5 compte
// 6 test accelero Gy521  reset to restart demo

 #include "Witty.h"
 #include "GetPush.h"
#define  Npix 8  // nombre de LEDs
#define  Brt 2 // max 31 brigtness 
#include "Apa102.h"   
 #include "Telec.h"
  #include "Pfm.h"
  #include "Inter2.h"
 #include "TerSer.h"    
 #define aadd 0x68*2 // a revoir
 #include "I2Ctwi.h"
 #include "Gy521.h"  
void setup() {                
  SetupWitty();
  SetupApa102();
//  ApaClear ();  // not required?
  SetupInter2(); 
   SetupTerSer();
   SetupI2Ctwi();
   SetupGy521();
  Cli(2); DelMs(500); 
}

// Une fonction démo à mettre dans un .h sépare avec par exemple un générateur de caractères
// On veut tester un compteur binaire 1 allumé 0 sombre
void ShowByte (uint8_t nn,uint8_t rr,uint8_t gg,uint8_t bb) {
  ApaHead();
  for (byte i=0; i<Npix; i++) {
    if (nn&0x80) {ApaLed(rr,gg,bb);}
    else {ApaLed(rr&3,gg&3,bb&3);}  // int modulo 1,2,3 
    nn<<=1;
  }
  ApaTail();  
}

byte cPush; 
byte cnt5=0;  // test 5
      #define D DelMs(1000)
      #define D2 DelMs(2000)
void loop() {
  cli();  // if Demo3 before
  ApaRainbow();  
  cPush= GetPush(); 
  switch (cPush) {
    case 1: // Demo1 Move
      byte cmo;
      cmo=2;
      while(cmo--){  // on répète 2 fois 
        ApaLogRed(1);          
        AvD; AvG; D;     // Avance
        StopG; StopD; D; 
         ApaLogGreen(4);     
        AvD;   D;         //TourneG
        StopG; StopD; D;
         ApaLogBlue(4); 
        AvG;  D;         // TourneD
        StopG; StopD; D; 
         ApaLogWhite(4); 
        RecD; RecG; D;  // Recule
        StopG; StopD; D; 
         ApaLogRed(15); 
        AvD; AvG; D; RecD; RecG; D; 
        BlockG; BlockD; D;  
      }
      break;
    case 2: // Demo2 Pfm Move
      ApaLogBlue(10);
        SetupInter2();
          ApaLogGreen(1);        
        pfmL=1; pfmR=1; D2;
          ApaLogGreen(2);
        pfmL=2; pfmR=2; D2;  
          ApaLogGreen(4);
        pfmL=4; pfmR=4; D2;
          ApaLogGreen(8);
        pfmL=8; pfmR=8; D;  
          ApaLogGreen(10);
        pfmL=20; pfmR=20; DelMs(500);
         ApaClear();
        pfmL=0; pfmR=0; D;
         ApaLogRed(10);
        pfmL=-20; pfmR=-20; DelMs(500); 
        pfmL=0; pfmR=0; D;
         ApaLogRed(15);
        pfmL=70; pfmR=70; DelMs(300);
        pfmL=0; pfmR=0; D;
         ApaLogYellow(15);        
        pfmL=-70; pfmR=-70; DelMs(300);
        pfmL=0; pfmR=0; D;   
      ApaLogBlue(2); 
  #define Dac DelMs(20) 
  #define MaxVit 75  // 15x5 a cause int logique leds         
  for (pfmR=0; pfmR<MaxVit; pfmR++) {pfmL=pfmR; ApaLogGreen(pfmR/5);Dac;}
  for (pfmR=MaxVit; pfmR>=0; pfmR--) {pfmL=pfmR; ApaLogBlue(pfmR/5);Dac;}
        pfmL=0; pfmR=0; ApaLogWhite(5); D;
  for (pfmR=0; pfmR>-MaxVit; pfmR--) {pfmL=pfmR; ApaLogBlue(-pfmR/5);Dac;}
  for (pfmR=-MaxVit; pfmR<0; pfmR++) {pfmL=pfmR; ApaLogGreen(-pfmR/5);Dac;}
        pfmL=0; pfmR=0; ApaLogWhite(5); D;
      DelMs(1000);
      cli();
      break;    
    case 3: // Demo IRM control
      ApaLogWhite(10);
 byte nPress;
      while(1) {
        nPress= Compte();
        switch (nPress) {
          case 1:   // stop
            ApaLogGreen(5);
            FreeG; FreeD; break;
          case 2:   // avance
            ApaLogRed(5);
            AvG; AvD; break;
          case 3:   // tourneG
            ApaLogBlue(5);
            FreeG; AvD; break;
          case 4:   // tourneG
            ApaLogPink(5);         
            AvG; FreeD; break;
          case 5:   // recule
            ApaLogYellow(10);
            RecG; RecD; break;
          default: // plus de 5
            ShowByte (0x55,5,5,5); 
            FreeG; FreeD; break;
         }  // end switch
      }  // end while
    case 5: // RGB   // new numbering
    sei();
       for (byte i=0;i<16;i++) {ApaLogRed(i); DelMs(100);}
       for (byte i=16;i>0;i--) {ApaLogRed(i-1); DelMs(100);}
       ApaLogGreen(1);DelMs(1000);ApaLogBlue(1);DelMs(1000);ApaLogRed(1);DelMs(1000);
       ApaLogGreen(15);DelMs(1000);ApaLogBlue(15);DelMs(1000);ApaLogRed(15);DelMs(1000);  // max intensity
       // add your own tricks
     cli();
       break;
    case 6:
        while(1) {
          ShowByte (cnt5++,41,41,0);
          DelMs(200);
        }
      // reset to quit
 
      case 4: // Demo6 Gy521 color effect click on terminal before to sse data
       ApaLogYellow(10);
       Text("EnvoiSer Gy521"); CR();     
       while(1){
          ReadGy521();
          Text("AcX"); Dec16s(AcX); Text("GyX"); Dec16s(GyX);
          Text("AcY"); Dec16s(AcY); Text("GyY"); Dec16s(GyY);
          Text("AcZ"); Dec16s(AcZ); Text("GyZ"); Dec16s(GyZ);
          CR(); 
          if (AcY> 10000) {ApaLogCyan(10);}
          else if (AcY<-10000) {ApaLogBlue(10);}
          if (AcZ>10000) {ApaLogRed(10);}
          else if (AcZ<-10000) {ApaLogGreen(10);}                   
          DelMs (200);
       }
      // reset to quit
    default: // blink wait return  ;
    break;
 } // end switch

} // end loop
  
