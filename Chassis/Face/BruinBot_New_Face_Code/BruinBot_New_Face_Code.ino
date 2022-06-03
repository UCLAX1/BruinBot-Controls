#include "Face.h"
#include "LEDMatrix.h"
#include "Arduino.h"
#define WS2812_pin 8 // only digital pin 8 works right now! PLug LED panel into 3.3V, Gnd, and pin 8. 
#define numberOfLEDs 256// total number of RGB LEDs
#define LED_PIN 13
#define BAUD_RATE 115200


// Create a face object on pin 8. 
Face myFace(WS2812_pin);

void setup() {
    //Set up hardware
    pinMode(WS2812_pin, OUTPUT);
    pinMode(LED_PIN, OUTPUT);

    myFace.clear();
    Serial.begin(BAUD_RATE);
    while (!Serial) {}; // hangs until serial port is properly opened
    // turn off LED when connected (?)
    digitalWrite(LED_PIN, HIGH);
    // try to connect to the pi
    while(!establish_connection()) {}
    digitalWrite(LED_PIN, LOW);
    //Serial.write("established connection \n");
    
}

void loop() {
    // when a target is set, and it is not the current emotion, it will trigger the face to transition into that emotion. Each emotion will loop itself (leave target unchanged) if it does not detect any interrupts, but if it does detect an interrupt it is responsible for getting itself back to happy_standby and updating the target variable. 
    // NOTE: The face code is capable of receiving several emotions in sequence, but it will transition into them and out of them again (so avoid sending 3 calls to "angry" in sequence when the face should be displaying angry_standby)
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
        - Receiving any other command will set the emotion back to happy_standby
    */
    
    if (myFace.target == "loading") {
        //Serial.println("loading");
        myFace.currentEmotion = "loading";
        myFace.loading();
       
    }
    else if (myFace.target == "happy") {
        //Serial.println("happy");
        myFace.currentEmotion = "happy";
        myFace.happy_standby();
    }
    else if (myFace.target == "angry") {
        //Serial.println("angry");
        if (myFace.currentEmotion != myFace.target) {
            myFace.angry_transition();
        }
        myFace.currentEmotion = "angry";
        myFace.angry_standby(); 
    }
    else if (myFace.target == "surprise"){
        //Serial.println("surprise");
        if (myFace.currentEmotion != myFace.target) {
            myFace.surprise_transition();
        }
        myFace.currentEmotion = "surprise";
        myFace.surprise(myFace.global_duration);
    }
    else if (myFace.target == "happy_emphasis") {
        //Serial.println("happy_emphasis");
        if (myFace.currentEmotion != myFace.target) {
            myFace.happy_emphasis_transition();
        }
        myFace.currentEmotion = "happy_emphasis";
        myFace.happy_emphasis(myFace.global_duration);
    }
    else if (myFace.target == "sad") {
        //Serial.println("sad");
        if (myFace.currentEmotion != myFace.target) {
            myFace.sad_transition();
        }
        myFace.currentEmotion = "sad";
        myFace.sad_standby();
    }
    else if (myFace.target == "neutral") {
        //Serial.println("neutral");
        myFace.currentEmotion = "neutral";
        myFace.neutral_standby();
    }
    else if (myFace.target == "bounce") {
        //Serial.println("bounce");
        myFace.currentEmotion = "bounce";
        myFace.bounce();
    }
    else {
        // some invalid target was found 
        myFace.target = "happy";
    }
}
// Since the emotion functions inside of the Face class need to interface directly with the serial monitor, a lot of the standard Serial Read functions like recvWithStartEndMarkers() has been moved into the Face.cpp file. 

//This function estbalishes a connection with the raspberry pi (currently it is never called because the raspberry pi is not being used)
boolean establish_connection() {
    while (!(Serial.available() > 0)) {} // hangs until a msg is received
    char rcv = Serial.read();
    if (rcv == 'i') {
        Serial.println("face");
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
