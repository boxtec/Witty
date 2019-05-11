// TestGetPush.ino  
#include "Witty.h"
#include "GetPush.h"
void setup() {
  SetupWitty();
  Cligno(2,200);  DelMs(500); // 2 blinks and delay
}
byte choix;
void loop() { 
    choix= GetPush();  // blink fast
    DelMs(500);  //little wait to show the transistion
    // Direct use of parameter
//    DelMs(500);  //delay after the confirmation blinks
//    Cligno(choix,200);  // do the selected task
//    DelMs (1000);

     //Test with a switch-case
     switch(choix) {
       case 1:
         Cligno(5,200);
         break;
       case 2:
         AvD;  DelMs(500); StopD; DelMs(200);
         AvG;  DelMs(500); StopG; DelMs(200);
         RecD; DelMs(500); StopD; DelMs(200);
         RecG; DelMs(500); StopG; DelMs(200);      
         break;
       case 3:
         while (!PushOn) {   // push to quit
           if (IrmOn) {LedOn;} // Test your TV remote
           else {LedOff;}
         }
         while (PushOn) {}
         break;
       default:
        CliErr(3,50,500);  // Reset to quit
     } // end switch 
} // end loop


//  Write an ErrSOS() function that repeat SOS the marine way (no parameter)
