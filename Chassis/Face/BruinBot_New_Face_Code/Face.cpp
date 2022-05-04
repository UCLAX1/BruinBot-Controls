#include "Face.h"
#include "LEDMatrix.h"
#define HOLD 100
#define HOLD_FRAMES 20 
/*
  Face.cpp: code to operate a Face object with different emotion codes.
  Public functions contain emotion sequences such as happy_standby, angry_transition, angry_standby, bounce, etc.
  Private functions contain individual frames with their names/numbers to make it easier to concatenate animation.
  The functions which directly access the LED matrix are contained in the LEDMatrix class.

  Rebecca Celsi 2020
  Created to run a 16x16 NeoPixel WS2812B matrix. 
*/

#include "Arduino.h"
#include "Face.h"
#include "LEDMatrix.h" 


Face::Face(int pin)
    :m_Matrix(pin)
{

    target = "loading";
    currentEmotion = target;
}

// Code that handles serial communication. 
bool Face::newTarget() {
    recvWithStartEndMarkers();
    if (newData == true) {
        strcpy(tempChars, receivedChars);
        // this temporary copy is necessary to protect the original data
        //   because strtok() used in parseData() replaces the commas with \0
        parseData();
        // update global variable 'target'
        target = String(receivedMsg);
        newData = false;
        return true;
    }
    // If no command was received, return false
    return false;
}

void Face::recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;

    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }
        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

//need to edit this 
void Face::parseData() {      // split the data into its parts

    char* strtokIndx; // this is used by strtok() as an index

    strtokIndx = strtok(tempChars, ",");      // get the first part - the string
    strcpy(receivedMsg, strtokIndx); // copy it to messageFromPC

    // this part handles int data if I need it 
    //strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
    //motor_speed = atoi(strtokIndx);     // convert this part to an integer
}

void Face::happy_standby() {
    // Happy_standby is the default screen of BruinBot
    // This screen displays a neutral smiling face and blinks occasionally.
    // when interrupted, the display immediately returns to the main loop code. 

    for (int i = 0; i < HOLD_FRAMES; i++) {
        happy_frame1(0); //display frame, including delay 
        if (newTarget()) {
            return;
        }
    }
    blink();
}

void Face::neutral_standby() {
    //neutral is a single frame that doesn't require any transition, it only has a different mouth than the happy emotion. 
    for (int i = 0; i < HOLD_FRAMES; i++) {
        neutral_frame1(); //display frame, including delay 
        if (newTarget()) {
            return;
        }
    }
    neutral_blink(); 
}

void Face::happy_emphasis_transition() {
    blink();
    happy_em_frame1();
    happy_em_frame2();
    happy_em_frame3();
}
void Face::happy_emphasis() {
    for (int i = 0; i < HOLD_FRAMES; i++) {
        happy_em_frame3();
        if (newTarget()) {
            happy_emphasis_reverse();
            return;
        }
    }
    target = "happy_standby";
    happy_emphasis_reverse();
}
void Face:: happy_emphasis_reverse(){
    happy_em_frame2();
    happy_em_frame1();
    //blink to transition to happy_standby
    blink();
}

void Face::surprise_transition() {
    set_eyebrowColor(0, 0, 5);
    surprise_frame1();
    surprise_frame2();
    surprise_frame3();
    surprise_frame4();
}

void Face::surprise() {
    for (int i = 0; i < HOLD_FRAMES; i++) {
        surprise_frame5();
        if (newTarget()) {
            surprise_transition_reverse();
            return;
        }
    }
    // Unlike angry, the surprised emotion does not loop itself, it just holds and then returns to default happy expression. 
    target = "happy_standby";
    surprise_transition_reverse();
}

void Face::surprise_transition_reverse() {
    set_eyebrowColor(0, 0, 5);
    surprise_reverse_frame1();
    surprise_reverse_frame2();
    surprise_reverse_frame3();
    surprise_reverse_frame4();
}

void Face::angry_transition() {
    set_eyebrowColor(10, 0, 0);
    //transition through blink
    blink_frame1();
    basic_smile(0); 
    angry_frame1(); //eyes half open, mouth changes position
    set_eyeColor(30, 0, 75);
    angry_frame2();
    set_eyeColor(60, 0, 50);
    angry_frame3(); //start to add eyebrows
    set_eyeColor(90, 0, 25);
    angry_frame4();
    set_eyeColor(120, 0, 0);
    angry_frame5(); //finish eyebrows
}

void Face::angry_standby() {
    set_eyebrowColor(10, 0, 0);
    
    // hold, end by blinking
    for (int i = 0; i < 20; i++) {
        angry_frame5(); //display frame, including delay 
        if (newTarget()) {
            angry_transition_reverse();
            return;
        }
    }
    // end by blinking
    // 'Angry' loops by default, so code will check for targets again 
    // after blink is finished and 'angry' loop has restarted)
    angry_blink();
}

void Face::angry_transition_reverse() {
    set_eyebrowColor(10, 0, 0);
    angry_blink();
    set_eyeColor(90, 0, 25);
    angry_frame4();
    set_eyeColor(60, 0, 50);
    angry_frame3();
    set_eyeColor(30, 0, 75);
    angry_frame2();
    set_eyeColor(0, 0, 100);
}

void Face::sad_transition() {
    set_eyebrowColor(0, 0, 5);
    //transition through blink
    sad_frame1();
    sad_frame2();
    sad_frame3(); 
    sad_frame4();
    sad_frame5();
    sad_frame6();
}

void Face::sad_standby() {
     // hold, end by blinking
    for (int i = 0; i < 20; i++) {
        sad_frame6(); //display frame, including delay 
        if (newTarget()) {
            sad_transition_reverse();
            return;
        }
    }
    // end by blinking
    sad_blink();
}

void Face::sad_transition_reverse() {
    set_eyebrowColor(0, 0, 5);
    sad_frame6();
    sad_frame5();
    sad_frame4();
    sad_frame3();
    sad_frame2();
    sad_frame1();
}

void Face::loading() {
    loading_frame1();
    loading_frame2();
    loading_frame3();
    loading_frame4();
    loading_frame5();
    loading_frame6();
    loading_frame7();
    loading_frame8();
    // loops until receiving another command, then plays startup animation
    if (newTarget()) {
        startup();
        return;
    }
}

void Face::startup() {
    startup_frame1();
    startup_frame2();
    startup_frame3();
    startup_frame4();
    set_eyeColor(0, 0, 3);
    startup_frame5();
    startup_frame6();
    startup_frame7();
    startup_frame8();
    set_eyeColor(0, 0, 5);
    startup_frame9();
    set_eyeColor(0, 0, 10);
    startup_frame10();
    set_eyeColor(0, 0, 30);
    startup_frame11();
    set_eyeColor(0, 0, 60);
    startup_frame12();
    set_eyeColor(0, 0, 100);
    happy_frame1(0);
    blink(); 
    // check to see if a new target has come in 
    newTarget();
}

void Face::bounce() {
    //the "bounce" code is used as a loading screen or maybe some other functions.
    // moves the entire face up and down to bounce off the top or bottom of the frame. 
    // the counter "h" indicates the height of the face below the neutral 0 position. Min is -9 (top) max is 2(bottom)
    // speed is the delay in milliseconds between frames. 

    // "bounce" the face up towards the top 
    for (int h = 0; h > -9; h--) {
        happy_frame1(h);
    }

    //display top "squished" frame where mouth travels one space upwards relative to the eyes
    basic_eyes(-9);
    basic_smile(-10);
    m_Matrix.displayFrame(HOLD);

    //bounce the face back down 
    for (int h = -9; h < 2; h++) {
        happy_frame1(h);
        // check for interrupts once in the middle of the bounce 
        if (h == 0) {
            if (newTarget()) {
                return;
            }
        }
    }
    //display bottom "squished" frame 
    basic_eyes(3);
    basic_smile(2);
    m_Matrix.displayFrame(HOLD);

    //bounce back to center position 
    for (int h = 2; h > 0; h--) {
        happy_frame1(h);
    }
}

void Face::set_eyeColor(byte RED, byte GREEN, byte BLUE) {
    eyeColor[0] = RED;
    eyeColor[1] = GREEN;
    eyeColor[2] = BLUE;
}

void Face::set_mouthColor(byte RED, byte GREEN, byte BLUE) {
    mouthColor[0] = RED;
    mouthColor[1] = GREEN;
    mouthColor[2] = BLUE;
}

void Face::set_eyebrowColor(byte RED, byte GREEN, byte BLUE) {
    eyebrowColor[0] = RED;
    eyebrowColor[1] = GREEN;
    eyebrowColor[2] = BLUE;
}


///////// FRAME CODES AND PRIVATE FUNCTIONS ////////////////////////////////////////////////////////////////////////
/*
    The following functions represent:
        - snippets of facial animation
        - individual frames
        - individual frame elements

    They cannot be accessed publicly, as they are called by the high-level emotion animations. 
*/

//***********  Animation Snippets ************* //

void Face::blink() {
    blink_frame1();
    basic_smile(0);
    blink_frame1();
}

void Face::neutral_blink() {
    //frame 1
    m_Matrix.mapLEDXY(2, 10, eyeColor); m_Matrix.mapLEDXY(3, 10, eyeColor); m_Matrix.mapLEDXY(12, 10, eyeColor); m_Matrix.mapLEDXY(13, 10, eyeColor);
    neutral_mouth(); 
    m_Matrix.displayFrame(100);

    //frame 2
    neutral_mouth();
    m_Matrix.displayFrame(100);

    //frame 3
    m_Matrix.mapLEDXY(2, 10, eyeColor); m_Matrix.mapLEDXY(3, 10, eyeColor); m_Matrix.mapLEDXY(12, 10, eyeColor); m_Matrix.mapLEDXY(13, 10, eyeColor);
    neutral_mouth();
    m_Matrix.displayFrame(100);
}

void Face::angry_blink() {
    //Frame 1
    angry_blink_frame1();

    //Frame 2
    angry_mouth();
    angry_eyebrows();
    m_Matrix.displayFrame(100);

    //Frame 3
    angry_blink_frame1();
}

void Face::sad_blink() {
    // play 2 frames forward and reverse
    sad_blink_frame1();
    sad_blink_frame2();
    sad_blink_frame1();
}

//***********  Single Frames  ************* //

void Face::happy_frame1(int h) {
    //h represents the 'height' of the face on the screen (useful for bounce code)
    //h should be zero for the default position. 
    basic_eyes(h);
    basic_smile(h); 
    m_Matrix.displayFrame(HOLD);
}

void Face::neutral_frame1() { 
    basic_eyes(0);
    neutral_mouth();
    m_Matrix.displayFrame(HOLD);
}

void Face::bored_frame1() {
    m_Matrix.mapLEDXY(2, 10, eyeColor); m_Matrix.mapLEDXY(3, 10, eyeColor); m_Matrix.mapLEDXY(12, 10, eyeColor); m_Matrix.mapLEDXY(13, 10, eyeColor);
    neutral_mouth();
    m_Matrix.displayFrame(HOLD);
}

void Face::blink_frame1() {
    //eyes
    m_Matrix.mapLEDXY(2, 10, eyeColor); m_Matrix.mapLEDXY(3, 10, eyeColor); m_Matrix.mapLEDXY(12, 10, eyeColor); m_Matrix.mapLEDXY(13, 10, eyeColor);
    //mouth
    basic_smile(0);
    m_Matrix.displayFrame(HOLD);
}

void Face::happy_em_frame1() {
    //eyes
    m_Matrix.mapLEDXY(2, 9, eyeColor); m_Matrix.mapLEDXY(3, 9, eyeColor); m_Matrix.mapLEDXY(12, 9, eyeColor); m_Matrix.mapLEDXY(13, 9, eyeColor);
    //mouth
    basic_smile(0); 
    m_Matrix.displayFrame(HOLD);
}
void Face::happy_em_frame2() {
    //eyes
    happy_em_eyes(); 
//mouth
    basic_smile(0);
    m_Matrix.displayFrame(HOLD);
}
void Face::happy_em_frame3() {
    happy_em_frame2();
    // add cheeks 
    byte cheekColor[3] = { 5,0,1 };
    m_Matrix.mapLEDXY(1, 11, cheekColor); m_Matrix.mapLEDXY(2, 11, cheekColor); m_Matrix.mapLEDXY(13, 11, cheekColor); m_Matrix.mapLEDXY(14, 11, cheekColor);
    m_Matrix.displayFrame(HOLD);
}

void Face::angry_frame1() {
    //eyes
    m_Matrix.mapLEDXY(2, 10, eyeColor); m_Matrix.mapLEDXY(3, 10, eyeColor); m_Matrix.mapLEDXY(12, 10, eyeColor); m_Matrix.mapLEDXY(13, 10, eyeColor);
    //mouth
    angry_mouth();
    m_Matrix.displayFrame(HOLD);
}
void Face::angry_frame2() {
    //eyes
    basic_eyes(0);
    //mouth
    angry_mouth();
    m_Matrix.displayFrame(HOLD);
}
void Face::angry_frame3() {
    //eyes and mouth same as previous frame 
    angry_frame2();
    //start to add eyebrows 
    m_Matrix.mapLEDXY(4, 7, eyebrowColor); m_Matrix.mapLEDXY(11, 7, eyebrowColor);
    m_Matrix.displayFrame(HOLD);
}
void Face::angry_frame4() {
    // mouth same as previous frame 
    angry_mouth();
    //eyes become angry
    angry_eyes();
    //add eyebrows
    m_Matrix.mapLEDXY(3, 6, eyebrowColor); m_Matrix.mapLEDXY(4, 7, eyebrowColor); m_Matrix.mapLEDXY(11, 7, eyebrowColor); m_Matrix.mapLEDXY(12, 6, eyebrowColor);
    m_Matrix.displayFrame(HOLD);
}
void Face::angry_frame5() {
    //eyes and mouth same as previous frame 
    angry_mouth();
    angry_eyes();
    //finish eyebrows
    angry_eyebrows();
    m_Matrix.displayFrame(HOLD);
}
void Face::angry_blink_frame1() {
    angry_mouth();
    angry_eyebrows();
    //eyes
    m_Matrix.mapLEDXY(2, 10, eyeColor); m_Matrix.mapLEDXY(3, 10, eyeColor); m_Matrix.mapLEDXY(12, 10, eyeColor); m_Matrix.mapLEDXY(13, 10, eyeColor);
    m_Matrix.displayFrame(HOLD);
}

/// Sad Frames 
void Face::sad_frame1() {
    // mouth 
    sad_mouth();
    //eyes 
    m_Matrix.mapLEDXY(2, 10, eyeColor); m_Matrix.mapLEDXY(3, 10, eyeColor); m_Matrix.mapLEDXY(12, 10, eyeColor); m_Matrix.mapLEDXY(13, 10, eyeColor);
    m_Matrix.displayFrame(HOLD);
}
void Face::sad_frame2() {
    // mouth 
    sad_mouth();
    //eyes 
    // in mid-blink
    m_Matrix.displayFrame(HOLD);
}
void Face::sad_frame3() {
    // mouth 
    sad_mouth();
    //eyes 
    basic_eyes(1); 
    m_Matrix.displayFrame(HOLD);
}
void Face::sad_frame4() {
    // mouth 
    sad_mouth();
    //eyes 
    m_Matrix.mapLEDXY(2,11,eyeColor);m_Matrix.mapLEDXY(3,10,eyeColor);m_Matrix.mapLEDXY(3,11,eyeColor);m_Matrix.mapLEDXY(12,10,eyeColor);m_Matrix.mapLEDXY(12,11,eyeColor);m_Matrix.mapLEDXY(13,11,eyeColor);
    // eyebrows 
    m_Matrix.mapLEDXY(2, 8, eyebrowColor); m_Matrix.mapLEDXY(13, 8, eyebrowColor);
    m_Matrix.displayFrame(HOLD);
}
void Face::sad_frame5() {
    // mouth 
    sad_mouth();
    //eyes
    m_Matrix.mapLEDXY(2, 11, eyeColor); m_Matrix.mapLEDXY(3, 10, eyeColor); m_Matrix.mapLEDXY(3, 11, eyeColor); m_Matrix.mapLEDXY(12, 10, eyeColor); m_Matrix.mapLEDXY(12, 11, eyeColor); m_Matrix.mapLEDXY(13, 11, eyeColor);

    // eyebrows 
    m_Matrix.mapLEDXY(2, 8, eyebrowColor); m_Matrix.mapLEDXY(3, 7, eyebrowColor); m_Matrix.mapLEDXY(4, 7, eyebrowColor); m_Matrix.mapLEDXY(11, 7, eyebrowColor); m_Matrix.mapLEDXY(12, 7, eyebrowColor); m_Matrix.mapLEDXY(13, 8, eyebrowColor);
    m_Matrix.displayFrame(HOLD);
}
void Face::sad_frame6() {
    // mouth 
    sad_mouth();
    //eyes
    m_Matrix.mapLEDXY(2,11,eyeColor);m_Matrix.mapLEDXY(3,10,eyeColor);m_Matrix.mapLEDXY(3,11,eyeColor);m_Matrix.mapLEDXY(12,10,eyeColor);m_Matrix.mapLEDXY(12,11,eyeColor);m_Matrix.mapLEDXY(13,11,eyeColor);

    // eyebrows 
    m_Matrix.mapLEDXY(2, 8, eyebrowColor); m_Matrix.mapLEDXY(3, 7, eyebrowColor); m_Matrix.mapLEDXY(4, 7, eyebrowColor); m_Matrix.mapLEDXY(5, 6, eyebrowColor); m_Matrix.mapLEDXY(10, 6, eyebrowColor); m_Matrix.mapLEDXY(11, 7, eyebrowColor); m_Matrix.mapLEDXY(12, 7, eyebrowColor); m_Matrix.mapLEDXY(13, 8, eyebrowColor);
    m_Matrix.displayFrame(HOLD);
}
void Face::sad_blink_frame1() {
    // mouth 
    sad_mouth();
    //eyes
    m_Matrix.mapLEDXY(2, 11, eyeColor); m_Matrix.mapLEDXY(3, 11, eyeColor); m_Matrix.mapLEDXY(12, 11, eyeColor); m_Matrix.mapLEDXY(13, 11, eyeColor);
    // eyebrows moved slightly down 
    m_Matrix.mapLEDXY(2, 9, eyebrowColor); m_Matrix.mapLEDXY(3, 8, eyebrowColor); m_Matrix.mapLEDXY(4, 8, eyebrowColor); m_Matrix.mapLEDXY(5, 7, eyebrowColor); m_Matrix.mapLEDXY(10, 7, eyebrowColor); m_Matrix.mapLEDXY(11, 8, eyebrowColor); m_Matrix.mapLEDXY(12, 8, eyebrowColor); m_Matrix.mapLEDXY(13, 9, eyebrowColor);
    m_Matrix.displayFrame(HOLD);
}
void Face::sad_blink_frame2() {
    // mouth 
    sad_mouth();
    //eyes
    // eyebrows moved slightly down 
    m_Matrix.mapLEDXY(2, 9, eyebrowColor); m_Matrix.mapLEDXY(3, 8, eyebrowColor); m_Matrix.mapLEDXY(4, 8, eyebrowColor); m_Matrix.mapLEDXY(5, 7, eyebrowColor); m_Matrix.mapLEDXY(10, 7, eyebrowColor); m_Matrix.mapLEDXY(11, 8, eyebrowColor); m_Matrix.mapLEDXY(12, 8, eyebrowColor); m_Matrix.mapLEDXY(13, 9, eyebrowColor);
    m_Matrix.displayFrame(HOLD);
}


//// Loading Screen
void Face::loading_frame1() {
    m_Matrix.mapLEDXY(6, 7, mouthColor); m_Matrix.mapLEDXY(6, 8, mouthColor); m_Matrix.mapLEDXY(7, 9, mouthColor);
    m_Matrix.displayFrame(HOLD);
}
void Face::loading_frame2() {
    m_Matrix.mapLEDXY(6, 8, mouthColor); m_Matrix.mapLEDXY(7, 9, mouthColor); m_Matrix.mapLEDXY(8, 9, mouthColor);
    m_Matrix.displayFrame(HOLD);
}
void Face::loading_frame3() {
    m_Matrix.mapLEDXY(7, 9, mouthColor); m_Matrix.mapLEDXY(8, 9, mouthColor); m_Matrix.mapLEDXY(9, 8, mouthColor);
    m_Matrix.displayFrame(HOLD);
}
void Face::loading_frame4() {
    m_Matrix.mapLEDXY(8, 9, mouthColor); m_Matrix.mapLEDXY(9, 7, mouthColor); m_Matrix.mapLEDXY(9, 8, mouthColor);
    m_Matrix.displayFrame(HOLD);
}
void Face::loading_frame5() {
    m_Matrix.mapLEDXY(8, 6, mouthColor); m_Matrix.mapLEDXY(9, 7, mouthColor); m_Matrix.mapLEDXY(9, 8, mouthColor);
    m_Matrix.displayFrame(HOLD);
}
void Face::loading_frame6() {
    m_Matrix.mapLEDXY(7, 6, mouthColor); m_Matrix.mapLEDXY(8, 6, mouthColor); m_Matrix.mapLEDXY(9, 7, mouthColor);
    m_Matrix.displayFrame(HOLD);
}
void Face::loading_frame7() {
    m_Matrix.mapLEDXY(6, 7, mouthColor); m_Matrix.mapLEDXY(7, 6, mouthColor); m_Matrix.mapLEDXY(8, 6, mouthColor);
    m_Matrix.displayFrame(HOLD);
}
void Face::loading_frame8() {
    m_Matrix.mapLEDXY(6, 7, mouthColor); m_Matrix.mapLEDXY(6, 8, mouthColor); m_Matrix.mapLEDXY(7, 6, mouthColor);
    m_Matrix.displayFrame(HOLD);
}

///  Loading to Startup Transition 
void Face::startup_frame1(){
    m_Matrix.mapLEDXY(6, 8, mouthColor); m_Matrix.mapLEDXY(6, 9, mouthColor); m_Matrix.mapLEDXY(6, 10, mouthColor);
    m_Matrix.displayFrame(HOLD);
}
void Face::startup_frame2() {
    m_Matrix.mapLEDXY(6, 10, mouthColor); m_Matrix.mapLEDXY(6, 11, mouthColor); m_Matrix.mapLEDXY(6, 12, mouthColor);
    m_Matrix.displayFrame(HOLD);
}
void Face::startup_frame3() {
    m_Matrix.mapLEDXY(6, 11, mouthColor); m_Matrix.mapLEDXY(6, 12, mouthColor); m_Matrix.mapLEDXY(7, 13, mouthColor);
    m_Matrix.displayFrame(HOLD);
}
void Face::startup_frame4() {
    m_Matrix.mapLEDXY(6, 12, mouthColor); m_Matrix.mapLEDXY(7, 13, mouthColor); m_Matrix.mapLEDXY(8, 13, mouthColor);
    m_Matrix.displayFrame(HOLD);
}
void Face::startup_frame5() {
    //mouth
    m_Matrix.mapLEDXY(7, 13, mouthColor); m_Matrix.mapLEDXY(8, 13, mouthColor); m_Matrix.mapLEDXY(9, 12, mouthColor);
    //eyes
    m_Matrix.mapLEDXY(7, 11, eyeColor); m_Matrix.mapLEDXY(8, 11, eyeColor);
    m_Matrix.displayFrame(HOLD);
}
void Face::startup_frame6(){
    //mouth
    m_Matrix.mapLEDXY(8, 13, mouthColor); m_Matrix.mapLEDXY(9, 11, mouthColor); m_Matrix.mapLEDXY(9, 12, mouthColor);
    //eyes
    m_Matrix.mapLEDXY(6, 10, eyeColor); m_Matrix.mapLEDXY(9, 10, eyeColor);
    m_Matrix.displayFrame(HOLD);
}
void Face::startup_frame7() {
    //mouth
    m_Matrix.mapLEDXY(8, 10, mouthColor); m_Matrix.mapLEDXY(9, 11, mouthColor); m_Matrix.mapLEDXY(9, 12, mouthColor);
    //eyes
    m_Matrix.mapLEDXY(5, 10, eyeColor); m_Matrix.mapLEDXY(10, 10, eyeColor);
    m_Matrix.displayFrame(HOLD);
}
void Face::startup_frame8() {
    //mouth
    m_Matrix.mapLEDXY(7, 10, mouthColor); m_Matrix.mapLEDXY(8, 10, mouthColor); m_Matrix.mapLEDXY(9, 11, mouthColor);
    //eyes
    m_Matrix.mapLEDXY(4, 10, eyeColor); m_Matrix.mapLEDXY(11, 10, eyeColor);
    m_Matrix.displayFrame(HOLD);
}
void Face::startup_frame9() {
    //mouth
    m_Matrix.mapLEDXY(6, 11, mouthColor); m_Matrix.mapLEDXY(7, 10, mouthColor); m_Matrix.mapLEDXY(8, 10, mouthColor);
    //eyes
    m_Matrix.mapLEDXY(3, 10, eyeColor); m_Matrix.mapLEDXY(12, 10, eyeColor);
    m_Matrix.displayFrame(HOLD);
}
void Face::startup_frame10() {
    //mouth
    m_Matrix.mapLEDXY(6, 11, mouthColor); m_Matrix.mapLEDXY(6, 12, mouthColor); m_Matrix.mapLEDXY(7, 10, mouthColor);
    //eyes
    m_Matrix.mapLEDXY(2, 10, eyeColor); m_Matrix.mapLEDXY(3, 10, eyeColor); m_Matrix.mapLEDXY(12, 10, eyeColor); m_Matrix.mapLEDXY(13, 10, eyeColor);
    m_Matrix.displayFrame(HOLD);
}
void Face::startup_frame11() {
    //mouth
    m_Matrix.mapLEDXY(6, 11, mouthColor); m_Matrix.mapLEDXY(6, 12, mouthColor); m_Matrix.mapLEDXY(7, 13, mouthColor);
    //eyes
    m_Matrix.mapLEDXY(2, 10, eyeColor); m_Matrix.mapLEDXY(3, 9, eyeColor); m_Matrix.mapLEDXY(3, 10, eyeColor); m_Matrix.mapLEDXY(12, 9, eyeColor); m_Matrix.mapLEDXY(12, 10, eyeColor); m_Matrix.mapLEDXY(13, 10, eyeColor);
    m_Matrix.displayFrame(HOLD);
}
void Face::startup_frame12(){
    m_Matrix.mapLEDXY(6, 12, mouthColor); m_Matrix.mapLEDXY(7, 13, mouthColor); m_Matrix.mapLEDXY(8, 13, mouthColor);
    basic_eyes(0);
    m_Matrix.displayFrame(HOLD);
}

//Surprise Frames 
void Face::surprise_frame1() {
    //eyes
    basic_eyes(0);
    //mouth
    m_Matrix.mapLEDXY(7, 12, mouthColor); m_Matrix.mapLEDXY(7, 13, mouthColor); m_Matrix.mapLEDXY(8, 12, mouthColor); m_Matrix.mapLEDXY(8, 13, mouthColor);
    m_Matrix.displayFrame(HOLD);
}
void Face::surprise_frame2() {
    //eyes
    m_Matrix.mapLEDXY(1, 9, eyeColor); m_Matrix.mapLEDXY(1, 10, eyeColor); m_Matrix.mapLEDXY(2, 8, eyeColor); m_Matrix.mapLEDXY(2, 9, eyeColor); m_Matrix.mapLEDXY(2, 10, eyeColor); m_Matrix.mapLEDXY(3, 8, eyeColor); m_Matrix.mapLEDXY(3, 9, eyeColor); m_Matrix.mapLEDXY(3, 10, eyeColor); m_Matrix.mapLEDXY(12, 8, eyeColor); m_Matrix.mapLEDXY(12, 9, eyeColor); m_Matrix.mapLEDXY(12, 10, eyeColor); m_Matrix.mapLEDXY(13, 8, eyeColor); m_Matrix.mapLEDXY(13, 9, eyeColor); m_Matrix.mapLEDXY(13, 10, eyeColor); m_Matrix.mapLEDXY(14, 9, eyeColor); m_Matrix.mapLEDXY(14, 10, eyeColor);
    //mouth
    surprised_mouth(); 
    //eyebrows
    m_Matrix.mapLEDXY(2, 7, eyebrowColor); m_Matrix.mapLEDXY(3, 7, eyebrowColor); m_Matrix.mapLEDXY(12, 7, eyebrowColor); m_Matrix.mapLEDXY(13, 7, eyebrowColor);
    m_Matrix.displayFrame(HOLD);
}
void Face::surprise_frame3() {
    //eyes
    surprised_eyes(); 
    //mouth
    surprised_mouth(); 
    //eyebrows
    m_Matrix.mapLEDXY(1, 6, eyebrowColor); m_Matrix.mapLEDXY(2, 6, eyebrowColor); m_Matrix.mapLEDXY(3, 6, eyebrowColor); m_Matrix.mapLEDXY(12, 6, eyebrowColor); m_Matrix.mapLEDXY(13, 6, eyebrowColor); m_Matrix.mapLEDXY(14, 6, eyebrowColor);
    m_Matrix.displayFrame(HOLD);
}
void Face::surprise_frame4() {
    //eyes
    surprised_eyes(); 
    //mouth 
    surprised_mouth(); 
    //eyebrows
    m_Matrix.mapLEDXY(1, 5, eyebrowColor); m_Matrix.mapLEDXY(2, 5, eyebrowColor); m_Matrix.mapLEDXY(3, 5, eyebrowColor); m_Matrix.mapLEDXY(12, 5, eyebrowColor); m_Matrix.mapLEDXY(13, 5, eyebrowColor); m_Matrix.mapLEDXY(14, 5, eyebrowColor);
    m_Matrix.displayFrame(HOLD);
}
void Face::surprise_frame5() {
    //eyes
    surprised_eyes();
    //mouth 
    surprised_mouth();
    //eyebrows
    m_Matrix.mapLEDXY(1, 4, eyebrowColor); m_Matrix.mapLEDXY(2, 4, eyebrowColor); m_Matrix.mapLEDXY(3, 4, eyebrowColor); m_Matrix.mapLEDXY(12, 4, eyebrowColor); m_Matrix.mapLEDXY(13, 4, eyebrowColor); m_Matrix.mapLEDXY(14, 4, eyebrowColor);
    m_Matrix.displayFrame(HOLD);
}

//Surprise reverse transition 
void Face::surprise_reverse_frame1() {
    //eyes
    m_Matrix.mapLEDXY(1, 9, eyeColor); m_Matrix.mapLEDXY(1, 10, eyeColor); m_Matrix.mapLEDXY(2, 9, eyeColor); m_Matrix.mapLEDXY(2, 10, eyeColor); m_Matrix.mapLEDXY(2, 11, eyeColor); m_Matrix.mapLEDXY(3, 9, eyeColor); m_Matrix.mapLEDXY(3, 10, eyeColor); m_Matrix.mapLEDXY(3, 11, eyeColor); m_Matrix.mapLEDXY(4, 9, eyeColor); m_Matrix.mapLEDXY(4, 10, eyeColor); m_Matrix.mapLEDXY(11, 9, eyeColor); m_Matrix.mapLEDXY(11, 10, eyeColor); m_Matrix.mapLEDXY(12, 9, eyeColor); m_Matrix.mapLEDXY(12, 10, eyeColor); m_Matrix.mapLEDXY(12, 11, eyeColor); m_Matrix.mapLEDXY(13, 9, eyeColor); m_Matrix.mapLEDXY(13, 10, eyeColor); m_Matrix.mapLEDXY(13, 11, eyeColor); m_Matrix.mapLEDXY(14, 9, eyeColor); m_Matrix.mapLEDXY(14, 10, eyeColor);
    //mouth
    surprised_mouth();
    //eyebrows
    m_Matrix.mapLEDXY(1, 4, eyebrowColor); m_Matrix.mapLEDXY(2, 4, eyebrowColor); m_Matrix.mapLEDXY(3, 4, eyebrowColor); m_Matrix.mapLEDXY(12, 4, eyebrowColor); m_Matrix.mapLEDXY(13, 4, eyebrowColor); m_Matrix.mapLEDXY(14, 4, eyebrowColor);
    m_Matrix.displayFrame(HOLD);
}
void Face::surprise_reverse_frame2() {
    //eyes
    m_Matrix.mapLEDXY(2, 11, eyeColor); m_Matrix.mapLEDXY(3, 11, eyeColor); m_Matrix.mapLEDXY(12, 11, eyeColor); m_Matrix.mapLEDXY(13, 11, eyeColor);
    //mouth
    surprised_mouth();
    //eyebrows
    m_Matrix.mapLEDXY(1, 5, eyebrowColor); m_Matrix.mapLEDXY(2, 5, eyebrowColor); m_Matrix.mapLEDXY(3, 5, eyebrowColor); m_Matrix.mapLEDXY(12, 5, eyebrowColor); m_Matrix.mapLEDXY(13, 5, eyebrowColor); m_Matrix.mapLEDXY(14, 5, eyebrowColor);
    m_Matrix.displayFrame(HOLD);
}
void Face::surprise_reverse_frame3() {
    //eyes
    //mouth
    surprised_mouth();
    //eyebrows
    m_Matrix.mapLEDXY(1, 6, eyebrowColor); m_Matrix.mapLEDXY(2, 6, eyebrowColor); m_Matrix.mapLEDXY(3, 6, eyebrowColor); m_Matrix.mapLEDXY(12, 6, eyebrowColor); m_Matrix.mapLEDXY(13, 6, eyebrowColor); m_Matrix.mapLEDXY(14, 6, eyebrowColor);
    m_Matrix.displayFrame(HOLD);
}
void Face::surprise_reverse_frame4() {
    //eyes
    m_Matrix.mapLEDXY(2, 11, eyeColor); m_Matrix.mapLEDXY(3, 11, eyeColor); m_Matrix.mapLEDXY(12, 11, eyeColor); m_Matrix.mapLEDXY(13, 11, eyeColor);
    //mouth
    m_Matrix.mapLEDXY(7, 12, mouthColor); m_Matrix.mapLEDXY(7, 13, mouthColor); m_Matrix.mapLEDXY(8, 12, mouthColor); m_Matrix.mapLEDXY(8, 13, mouthColor);
    //eyebrows
    m_Matrix.mapLEDXY(1, 7, eyebrowColor); m_Matrix.mapLEDXY(2, 7, eyebrowColor); m_Matrix.mapLEDXY(13, 7, eyebrowColor); m_Matrix.mapLEDXY(14, 7, eyebrowColor);
    m_Matrix.displayFrame(HOLD);
}

//***********  Frame Elements  ************* //

void Face::basic_smile(int h) {
    //mouth
    m_Matrix.mapLEDXY(6, h + 12, mouthColor); m_Matrix.mapLEDXY(7, h + 13, mouthColor); m_Matrix.mapLEDXY(8, h + 13, mouthColor); m_Matrix.mapLEDXY(9, h + 12, mouthColor);
}

void Face::basic_eyes(int h) {
    //eyes
    //  height indicates the height above default (0 is default)
    m_Matrix.mapLEDXY(2, h + 9, eyeColor); m_Matrix.mapLEDXY(2, h + 10, eyeColor); m_Matrix.mapLEDXY(3, h + 9, eyeColor); m_Matrix.mapLEDXY(3, h + 10, eyeColor);
    m_Matrix.mapLEDXY(12, h + 9, eyeColor); m_Matrix.mapLEDXY(12, h + 10, eyeColor); m_Matrix.mapLEDXY(13, h + 9, eyeColor); m_Matrix.mapLEDXY(13, h + 10, eyeColor);
}

void Face::neutral_mouth(){
    m_Matrix.mapLEDXY(6,13,mouthColor);m_Matrix.mapLEDXY(7,13,mouthColor);m_Matrix.mapLEDXY(8,13,mouthColor);

}
void Face::sad_mouth() {
    m_Matrix.mapLEDXY(6, 13, mouthColor); m_Matrix.mapLEDXY(7, 13, mouthColor); m_Matrix.mapLEDXY(8, 13, mouthColor);
}

void Face::happy_em_eyes() {
    //eyes
     m_Matrix.mapLEDXY(1, 9, eyeColor); m_Matrix.mapLEDXY(2, 8, eyeColor); m_Matrix.mapLEDXY(3, 8, eyeColor); m_Matrix.mapLEDXY(4, 9, eyeColor); m_Matrix.mapLEDXY(11, 9, eyeColor); m_Matrix.mapLEDXY(12, 8, eyeColor); m_Matrix.mapLEDXY(13, 8, eyeColor); m_Matrix.mapLEDXY(14, 9, eyeColor);
}

void Face::angry_mouth() {
    //mouth
    m_Matrix.mapLEDXY(6, 13, mouthColor); m_Matrix.mapLEDXY(7, 13, mouthColor); m_Matrix.mapLEDXY(8, 13, mouthColor); m_Matrix.mapLEDXY(9, 13, mouthColor);

}

void Face::angry_eyes() {
    //eyes
    m_Matrix.mapLEDXY(2, 9, eyeColor); m_Matrix.mapLEDXY(2, 10, eyeColor); m_Matrix.mapLEDXY(3, 10, eyeColor); m_Matrix.mapLEDXY(12, 10, eyeColor); m_Matrix.mapLEDXY(13, 9, eyeColor); m_Matrix.mapLEDXY(13, 10, eyeColor);

}

void Face::angry_eyebrows() {
    m_Matrix.mapLEDXY(2, 5, eyebrowColor); m_Matrix.mapLEDXY(3, 6, eyebrowColor); m_Matrix.mapLEDXY(4, 7, eyebrowColor); m_Matrix.mapLEDXY(11, 7, eyebrowColor); m_Matrix.mapLEDXY(12, 6, eyebrowColor); m_Matrix.mapLEDXY(13, 5, eyebrowColor);

}
void Face::surprised_eyes() {
    //eyes
    m_Matrix.mapLEDXY(1, 9, eyeColor); m_Matrix.mapLEDXY(1, 10, eyeColor); m_Matrix.mapLEDXY(2, 8, eyeColor); m_Matrix.mapLEDXY(2, 9, eyeColor); m_Matrix.mapLEDXY(2, 10, eyeColor); m_Matrix.mapLEDXY(2, 11, eyeColor); m_Matrix.mapLEDXY(3, 8, eyeColor); m_Matrix.mapLEDXY(3, 9, eyeColor); m_Matrix.mapLEDXY(3, 10, eyeColor); m_Matrix.mapLEDXY(3, 11, eyeColor); m_Matrix.mapLEDXY(4, 9, eyeColor); m_Matrix.mapLEDXY(4, 10, eyeColor); m_Matrix.mapLEDXY(11, 9, eyeColor); m_Matrix.mapLEDXY(11, 10, eyeColor); m_Matrix.mapLEDXY(12, 8, eyeColor); m_Matrix.mapLEDXY(12, 9, eyeColor); m_Matrix.mapLEDXY(12, 10, eyeColor); m_Matrix.mapLEDXY(12, 11, eyeColor); m_Matrix.mapLEDXY(13, 8, eyeColor); m_Matrix.mapLEDXY(13, 9, eyeColor); m_Matrix.mapLEDXY(13, 10, eyeColor); m_Matrix.mapLEDXY(13, 11, eyeColor); m_Matrix.mapLEDXY(14, 9, eyeColor); m_Matrix.mapLEDXY(14, 10, eyeColor);
}
void Face::surprised_mouth() {
    //mouth
    m_Matrix.mapLEDXY(7, 13, mouthColor); //m_Matrix.mapLEDXY(7, 14, mouthColor); m_Matrix.mapLEDXY(8, 13, mouthColor); m_Matrix.mapLEDXY(8, 14, mouthColor);
}
