 // WittyDemo2.ino 190506  4580b 113v  distributed on w01-w10 
// welcome with Rainbow and fast blink push n times
// 1 push  several move on-off and back to menu
// 2 pushes  same with pfm speed changes
// 3 control  with most TV IR 1pulse stop, 2,3,4,5 move
// 4 demonstrate log dim
// 5 count in binary
// 6 test accelerometer Gy521 value in terminal 4 orientations on leds

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
  Cligno(2,200); 
}

// Example of a not simple function binary counter 1 bright 0 dark but visible
// That function has no reason to be included in the Apa102.h file
// Show a byte in any color.
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
 #define D02 DelMs(200)
 #define D05 DelMs(500)
 #define D1 DelMs(1000)
 #define D2 DelMs(2000)
void loop() {
  cli();  // if Demo3 before
  ApaRainbow();  
  cPush= GetPush(); 
  switch (cPush) {
  D2; 
    case 1: // Demo1 Move
      ApaLogRed(4);
        AvD; AvG; D05;     // Avance
        StopG; StopD; D05;    
        AvD;   D05;         //TourneG
        StopG; StopD; D05;
        AvG;  D05;         // TourneD
        StopG; StopD; D05; 
        RecD; RecG; D05;  // Recule
        StopG; StopD; D05; 
        AvD; AvG; D05; RecD; RecG; D05; 
        BlockG; BlockD; D05;  
        ApaLogGreen(4); AvG; RecD; D1;
        ApaLogBlue(4); AvD; RecG; D1;
         BlockG; BlockD; D05;
        ApaLogWhite(4); AvD; AvG; D05; RecD; RecG; D05;
        StopG; StopD; ApaLogRed(4);D02;ApaLogGreen(4);D02;
      break;
    case 2: // Demo2 Pfm Move
      ApaLogBlue(10);
        SetupInter2();
        pfmL=2; pfmR=2;  D2;D2;
        pfmL=-2; pfmR=-2; D2;D2;
        pfmL=4; pfmR=4; D2;
        pfmL=-4; pfmR=-4; D2;  
        pfmL=16; pfmR=16; D1;
        pfmL=0; pfmR=0; D2;
        pfmL=-16; pfmR=-16; D1; 
        pfmL=0; pfmR=0; D2;
        pfmL=70; pfmR=70; D05;
        pfmL=0; pfmR=0; D2;
        pfmL=-70; pfmR=-70; D05;
        pfmL=0; pfmR=0; D1;   
      ApaLogBlue(2); 
  #define Dac DelMs(10) 
  for(volatile byte i; i<3; i++) {
    D2; 
    #define MaxVit 80 
    for (pfmR=0; pfmR<MaxVit; pfmR++) {pfmL=pfmR; ApaLogGreen(pfmL/5); Dac;} 
      ApaLogRed(2); 
    for (pfmR=MaxVit; pfmR>0; pfmR--) {pfmL=pfmR; ApaLogGreen(pfmL/5); Dac;}
    for (pfmR=0; pfmR>-MaxVit; pfmR--) {pfmL=pfmR; ApaLogRed(-pfmL/5); Dac;}
      ApaLogGreen(2); 
    for (pfmR=-MaxVit; pfmR<0; pfmR++) {pfmL=pfmR; ApaLogRed(-pfmL/5); Dac;}
        pfmL=0; pfmR=0; D1;
  }
     cli();
     break; 
            
    case 3: // Demo IRM control
      ApaLogWhite(10);
 byte nPress;
      while(1) {
        nPress= Compte();
        switch (nPress) {
          case 1:   // stop
            ApaLogRed(5);
            FreeG; FreeD; break;
          case 2:   // avance
            ApaLogGreen(5);
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
      // reset to quit
      
    case 5: // RGB   // ApaLog requires interrupt 
    sei();
       for (volatile byte i=0;i<16;i++) {ApaLogRed(i); DelMs(100);}
       for (volatile byte i=16;i>0;i--) {ApaLogRed(i-1); DelMs(100);}
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
  
