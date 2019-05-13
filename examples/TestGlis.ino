//TestGlis.ino  Display values and average
//use TerSer
#define aadd 0x68*2  // adresse 8bits du 6050 .. a faire mieux

// variables AcX ... defined in Gy521.h
volatile int16_t iniAcZ,mAcZ;
volatile int16_t iniGyX,mGyX;
volatile int16_t  acumAcZ;

#include "Witty.h"
#include "TerSer.h"
#include "I2Ctwi.h"
#include "Gy521.h" 
#include "Glis.h" 

void setup() {
  SetupWitty();
  SetupI2Ctwi();
  SetupGy521();
  SetupTerSer();
  DelMs(1000); // Put in place/stabilization time
   AcZ=ReadWordAt(0x3F);
   IniGlis1(AcZ); iniAcZ=AcZ;
   GyX=ReadWordAt(0x43);
   IniGlis2(GyX); iniGyX=GyX;
}

void loop() { 
    Text("Gy521 initial values"); CR();
    Text("iniAcZ "); Dec16s(iniAcZ); Text("       iniGyX "); Dec16s(iniGyX); CR();
       while(1){
          AcZ=ReadWordAt(0x3F);    GyX=ReadWordAt(0x43);
          Text("   AcZ "); Dec16s(AcZ); Dec8s((AcZ-iniAcZ)/16); Text("%%"); 
          Text("   GyX "); Dec16s(GyX); Dec8s((GyX-iniGyX)/16); Text("%%"); CR();    
          DelMs (500);
       }  // end while
}  // end loop
