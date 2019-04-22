// Gy521  190419
//dans le pp  #include "Gy521"   SetupGy521();
// Adresse 7 bits 0x68 a déclarer en 8 bits pour TWI
#define aadd 0x68*2  // adresse 8bits
void SetupGy521() { WriteByteAt(0x6B,0); } //wakes up the MPU-6050)

volatile int16_t AcX,AcY,AcZ;
volatile int16_t GyX,GyY,GyZ;
void ReadGy521 () {    // update global variables
   AcX=ReadWordAt(0x3B);  //Acceleration X
   AcY=ReadWordAt(0x3D);
   AcZ=ReadWordAt(0x3F);
   GyX=ReadWordAt(0x43);  //Gyro X
   GyY=ReadWordAt(0x45);
   GyZ=ReadWordAt(0x47);
}

