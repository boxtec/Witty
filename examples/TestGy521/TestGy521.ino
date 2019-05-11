//TestG521.ino  affichage TerSer 
// 1g is given as 16384  
#define aadd 0x68*2  // adresse 8bits du 6050 .. a faire mieux

// déf et variables globales
int16_t GyX,GyY,GyZ;   
int16_t AcX,AcY,AcZ;  

#include "Witty.h"
#include "TerSer.h"
#include "I2Ctwi.h"
#include "Gy521.h" 

void setup() {
  SetupWitty();
  SetupI2Ctwi();
  SetupGy521();
  SetupTerSer();
}

int var;
void loop() { 
    DelMs(1000);  //Wait 1s for stabilisation 
       Text("Gy521 values"); CR();
       int16_t AcX,AcY,AcZ;
       while(1){
           AcX=ReadWordAt(0x3B);  //Accelerometer
           AcY=ReadWordAt(0x3D);
           AcZ=ReadWordAt(0x3F);

          Text("  AcX= "); Dec16s(AcX); 
          Text("  AcY= "); Dec16s(AcY);
          Text("  AcZ= "); Dec16s(AcZ); 
          CR(); 
          DelMs (500);
       }  // end while
}  // end loop

