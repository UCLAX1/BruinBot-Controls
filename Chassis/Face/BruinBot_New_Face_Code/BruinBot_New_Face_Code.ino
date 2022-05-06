#include "Face.h"
#include "LEDMatrix.h"
#include "Arduino.h"
#define WS2812_pin 8 // only digital pin 8 works right now! PLug LED panel into 3.3V, Gnd, and pin 8. 
#define numberOfLEDs 256// total number of RGB LEDs
#define LED_PIN 13
#define BAUD_RATE 115200
// default hold of 100 ms per frame and 20 frames per long hold


// Create a face object on pin 8. 
Face myFace(WS2812_pin);

void setup() {
    //Set up hardware
    pinMode(WS2812_pin, OUTPUT);
    pinMode(LED_PIN, OUTPUT);
    
    Serial.begin(BAUD_RATE);
    while (!Serial) {}; // hangs until serial port is properly opened
    Serial.write("connected port \n");
    // turn off LED when connected (?)
    digitalWrite(LED_PIN, HIGH);
    //while (!establish_connection()) {}
    digitalWrite(LED_PIN, LOW);
    Serial.write("established connection \n");
    
}

void loop() {
    // when a target is set, and it is not the current emotion, it will trigger the face to transition into that emotion. Each emotion will loop itself (leave target unchanged) if it does not detect any interrupts, but if it does detect an interrupt it is responsible for getting itself back to happy_standby and updating the target variable. 
    // NOTE: The face code is not capable of receiving many target codes quickly, as it only checks and parses them at specific times!
 /*
 //Loading screen 
  myFace.loading(); 
  myFace.loading(); 
  myFace.loading();
 //Startup and go to happy standby 
  myFace.startup(); 
  myFace.happy_standby(); 
 //Surprise
  myFace.surprise_transition(); 
  myFace.surprise();
 //Happy emphasis
  myFace.happy_standby();
  myFace.happy_emphasis_transition(); 
  myFace.happy_emphasis(); // have to put a duration for this one
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
  myFace.happy_standby(); 
 //bounce 
  myFace.bounce();
  myFace.bounce();
   */
  /*
    Possible target codes :
        "loading"
        "happy"
        "angry"
        "surprised"
        "happy_emphasis"
        "sad"
        "neutral"
        "bounce"
    */
    
    if (myFace.target == "loading") {
        Serial.println("loading");
        myFace.currentEmotion = "loading";
        myFace.loading();
       
    }
    else if (myFace.target == "happy") {
        Serial.println("happy");
        myFace.currentEmotion = "happy";
        myFace.happy_standby();
    }
    else if (myFace.target == "angry") {
        Serial.println("angry");
        if (myFace.currentEmotion != myFace.target) {
            myFace.angry_transition();
        }
        myFace.currentEmotion = "angry";
        myFace.angry_standby(); 
    }
    else if (myFace.target == "surprise"){
        Serial.println("surprise");
        if (myFace.currentEmotion != myFace.target) {
            myFace.surprise_transition();
        }
        myFace.currentEmotion = "surprise";
        myFace.surprise();
    }
    else if (myFace.target == "happy_emphasis") {
        Serial.println("happy_emphasis");
        if (myFace.currentEmotion != myFace.target) {
            myFace.happy_emphasis_transition();
        }
        myFace.currentEmotion = "happy_emphasis";
        myFace.happy_emphasis();
    }
    else if (myFace.target == "sad") {
        Serial.println("sad");
        if (myFace.currentEmotion != myFace.target) {
            myFace.sad_transition();
        }
        myFace.currentEmotion = "sad";
        myFace.sad_standby();
    }
    else if (myFace.target == "neutral") {
        Serial.println("neutral");
        myFace.currentEmotion = "neutral";
        myFace.neutral_standby();
    }
    else if (myFace.target == "bounce") {
        Serial.println("bounce");
        myFace.currentEmotion = "bounce";
        myFace.bounce();
    }
    else {
        // some invalid target was found 
        myFace.target = "happy";
    }
}
// Since the emotion functions inside of the Face class need to interface directly with the serial monitor, a lot of the standard Serial Read functions like recvWithStartEndMarkers() has been moved into the Face.cpp file. Hopefully this works. 

boolean establish_connection() {
    while (!(Serial.available() > 0)) {} // hangs until a msg is received
    char rcv = Serial.read();
    if (rcv == 'i') {
        Serial.println("motor");
    }

    int start = millis();
    while (!(Serial.available() > 0)) {
        if (millis() - start > 1000) {
            return false;
        }
    }
    rcv = Serial.read();
    if (rcv == 'd') {
        // Pi has acknowledged this arduino
        return true;
    }
    else {
        return false;
    }
}
