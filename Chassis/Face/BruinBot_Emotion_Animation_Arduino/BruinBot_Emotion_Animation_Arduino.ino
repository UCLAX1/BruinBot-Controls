#include "Face.h"
#include "LEDMatrix.h"
#include"Arduino.h"
#define WS2812_pin 8 // only digital pin 8 works right now! PLug LED panel into 3.3V, Gnd, and pin 8. 
#define numberOfLEDs 256 // total number of RGB LEDs

 //push all frames of the first emotion to the queue. 
myFace.addFrames(myFace.currentEmotion);

//push all frames of the first emotion to the queue. 
myFace.currentFrame = myFace.frameQueue.peek();
unsigned long startTime = millis(); 
unsigned long prevMillis = startTime;
unsigned long delay = myFace.currentFrame->delay;
int total_frames = 0;
    
void setup() {
    //Set up hardware
    pinMode(WS2812_pin, OUTPUT);
    
    //create a new Face object on pin 8 
    Face myFace(WS2812_pin); 
    // set up queue with its starting emotion
    Serial.begin(9600);
    Serial.write("Made face!");
}
void loop() {
    // This loop represents the "void loop" of the arduino
    // put a time limit on it so its not infinite
       
    // CHECK FOR INTERRUPTS HERE
    bool interrupted = false; 
    if (total_frames == 2 || total_frames == 20){
        interrupted = true; 
    }
    if (interrupted) {
        //   ---> Set target based on message received! 
        //    The target emotion should always be a TRANSITION emotion (not a standby emotion) since it may be interrupted as soon as it gets to the transition. 
        Emotion* target = &myFace.angry_transition;
        if (!myFace.currentEmotion->completesSelf) {
            // clear queue before adding new frames
            myFace.clearQueue();
        }
        //add new frames to end of queue
        // Add the emotions until right before reaching happy_standby (the neutral position) 

        Emotion* transitionEmotion = myFace.currentEmotion->interruptEmotion;

        while (transitionEmotion != &myFace.happy_standby) {
            myFace.addFrames(transitionEmotion);
            transitionEmotion = transitionEmotion->interruptEmotion;
        }
        //now skip happy_standby and add the destination emotion to the queue.
        // Now everything is set up to play out correctly. 
        myFace.addFrames(target);
        // not technically accurate as the current emotion will not be the target until the backlog of frames is finished displaying. But this will help the emotion web know how to proceed when it reaches the end of the queue. 
        myFace.currentEmotion = target; 
    }
    
    // FRAME DISPLAY CODE
    //Now display the current frame in the queue, or change frames if enough time has passed. 
    unsigned long currentMillis = millis();

    // if enough time has elapsed to switch to the next frame
    //if ((currentMillis - prevMillis) > myFace.currentFrame->delay) {
    if ((currentMillis - prevMillis) > 20) {
        
        Serial.write("delay passed: " + myFace.currentFrame->delay + " ms ");
        // check if frame needs to be refilled (only the current frame is left in it)
        if (size(myFace.frameQueue) == 1) {
            //the current frame is the last frame in the list 
            // frame queue is empty; find the new emotion and add it to the queue. 
            myFace.currentEmotion = myFace.currentEmotion->nextEmotion;

            //push all frames of the new emotion to the queue. 
            myFace.addFrames(myFace.currentEmotion);
            Serial.write("pushed new emotions ");
        }

        // pop the current frame (which has now completed displaying) 
        myFace.frameQueue.pop();
        myFace.currentFrame = myFace.frameQueue.front();
        delay = myFace.currentFrame->delay; 
        prevMillis = currentMillis;
        total_frames++;

        // display new frame here (only displays once, when first initialized)
        myFace.displayFrame(myFace.currentFrame);
        Serial.write("current frame # of pixels: " + myFace.currentFrame->pixelList.size());
     
    }
}
