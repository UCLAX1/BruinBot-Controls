#include "Face.h"
#include "LEDMatrix.h"

#define WS2812_pin 8 // only digital pin 8 works right now! PLug LED panel into 3.3V, Gnd, and pin 8. 
#define numberOfLEDs 256// total number of RGB LEDs

Face myFace(8); 

void setup() {
 
  //Set up hardware
  pinMode(WS2812_pin, OUTPUT);

  //create a new Face object on pin 8 
  Face myFace(WS2812_pin); 
}

void loop() {
  myFace.readFile("test.csv")
 }
