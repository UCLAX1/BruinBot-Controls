#include "Face.h"
#include "LEDMatrix.h"

#define WS2812_pin 8 // only digital pin 8 works right now! PLug LED panel into 3V, Gnd, and pin 8. 
#define numberOfLEDs 256// total number of RGB LEDs

Face myFace(8); 

void setup() {
 
  //Set up hardware
  pinMode(WS2812_pin, OUTPUT);

  //create a new Face object on pin 8 
  Face myFace(WS2812_pin); 
}

void loop() {
 // Animation Demo
 
 //Loading screen 
  myFace.loading(); 
  myFace.loading(); 
  myFace.loading();
 //Startup and go to happy standby 
  myFace.startup(); 
  myFace.happy_standby(); 
 //Surprise
  myFace.surprise(); 
  myFace.surprise_reverse(); 
 //Happy emphasis
  myFace.happy_standby();
  myFace.happy_emphasis(1500); // have to put a duration for this one
  myFace.happy_standby(); 
 //Angry
  myFace.angry_transition(); 
  myFace.angry_standby();
  myFace.angry_transition_reverse();
  myFace.happy_standby(); 
 // Sad 
  myFace.sad_transition(); 
  myFace.sad_standby();  
  myFace.sad_transition_reverse(); 
  myFace.happy_standby(); 
// neutral and tired/bored
  myFace.neutral_standby(); 
  myFace.bored_standby(); 
  myFace.happy_standby(); 
 //bounce 
  myFace.bounce(150);
  myFace.bounce(150);

}
