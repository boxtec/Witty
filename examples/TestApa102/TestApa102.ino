//TestApa.ino ok 190330 1224b  38v
// arc en ciel de 0 à 192  manque de l'orangé
#include "Witty.h"
#define  Npix 8  // nombre de LEDs
#define  Brt 2 // max 31 brigtness 
#include "Apa102.h"

void setup () {
  SetupWitty();
  SetupApa102();  

}

void loop() { 
   ApaRainbow();
   DelMs (2000);
   
   for (byte i=0; i<16; i++) { ApaLogRed (i); DelMs(100);}
   for (byte i=16; i>0; i--) { ApaLogRed (i-1); DelMs(100);} 
   for(;;);
}

/*
 Test ApaRed from 0 to 255 (reduce delay)
 Modify Brt value
 Modify in the lib parameters of ApaRainbow. Hue etc is explained in doc.
*/

