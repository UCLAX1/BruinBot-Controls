#include <Face.h>
#include <LEDMatrix.h>
 
#define WS2812_pin 8 // only digital pin 8 works right now!
#define numberOfLEDs 256// total number of RGB LEDs

Face myFace(8); 

void setup() {
  // put your setup code here, to run once:
  //Set up hardware
  pinMode(WS2812_pin, OUTPUT);

  //create a new Face object on pin 8 
  Face myFace(8); 
}

void loop() {
  // put your main code here, to run repeatedly:
 
  /*
  for (int i =0; i<3; i++){
     myFace.bounce(150); 
  }
  */
  
  //myFace.loading(); 
  //myFace.loading(); 
  //myFace.loading(); 

 // myFace.startup(); 
  myFace.happy_standby(); 
  myFace.surprise(); 
  myFace.surprise_reverse(); 
    myFace.happy_standby(); 
  
  //myFace.happy_emphasis(1500);
 // myFace.happy_standby(); 
  //myFace.angry_transition(); 
  //myFace.angry_standby();
 // myFace.angry_standby();
  //myFace.angry_transition_reverse();
  //myFace.happy_standby(); 
  //myFace.bounce(150);
  //myFace.bounce(150);

}
