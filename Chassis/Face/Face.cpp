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
    
}

void Face::happy_standby() {
    // Happy_standby is the default screen of BruinBot
    // This screen displays a neutral smiling face and blinks occasionally.
    //The blink is in the middle so the demo looks more natural when other emotions interrupt. 
    happy_frame1(0);
    m_Matrix.displayFrame(2000); //display for 2 seconds
    blink();
    happy_frame1(0);
    m_Matrix.displayFrame(2000); //display for 2 seconds

}

void Face::bounce(int speed) {
  //the "bounce" code is used as a loading screen or maybe some other functions.
  // moves the entire face up and down to bounce off the top or bottom of the frame. 
  // the counter "h" indicates the height of the face below the neutral 0 position. Min is -9 (top) max is 2(bottom)
  // speed is the delay in milliseconds between frames. 

  // "bounce" the face up towards the top 
    for (int h = 0; h > -9; h--) {
        happy_frame1(h); 
        m_Matrix.displayFrame(speed);
    }

    //display top "squished" frame where mouth travels one space upwards relative to the eyes
    basic_eyes(-9);
    basic_smile(-10);
    m_Matrix.displayFrame(speed);

    //bounce the face back down 
    for (int h = -9; h < 2; h++) {
        happy_frame1(h);
        m_Matrix.displayFrame(speed);
    }

    //display bottom "squished" frame 
    basic_eyes(3);
    basic_smile(2); 
    m_Matrix.displayFrame(speed);

    //bounce back to center position 
    for (int h = 2; h > 0; h--) {
        happy_frame1(h);
        m_Matrix.displayFrame(speed);
    }
}

void Face::happy_emphasis(int hold) {

    //transition animation
    happy_frame1(0); 
    m_Matrix.displayFrame(100);
    blink(); 
    happy_em_frame1(); 
    m_Matrix.displayFrame(100);
    happy_em_frame2(); 
    m_Matrix.displayFrame(100);
    happy_em_frame3(); 

    //hold for input time
    m_Matrix.displayFrame(hold);  

    //transition back
    happy_em_frame2();
    m_Matrix.displayFrame(100);
    happy_em_frame1();
    m_Matrix.displayFrame(100);

    //blink to transition to happy_standby
    blink();
}

void Face::angry_transition() {
    set_eyebrowColor(10, 0, 0);
    //transition through blink
    blink_frame1();
    m_Matrix.displayFrame(100);
    basic_smile(0); 
    m_Matrix.displayFrame(100);
    angry_frame1(); //eyes half open, mouth changes position
    m_Matrix.displayFrame(100);

set_eyeColor(30, 0, 75);
angry_frame2();
m_Matrix.displayFrame(100);
set_eyeColor(60, 0, 50);
angry_frame3(); //start to add eyebrows
m_Matrix.displayFrame(100);
set_eyeColor(90, 0, 25);
angry_frame4();
m_Matrix.displayFrame(100);
set_eyeColor(120, 0, 0);
angry_frame5(); //finish eyebrows
m_Matrix.displayFrame(100);
}

void Face::angry_standby() {
    set_eyebrowColor(10, 0, 0);
    // hold, end by blinking
    angry_frame5();
    m_Matrix.displayFrame(2000); //display for 2 seconds
    angry_blink();
    angry_frame5();
    m_Matrix.displayFrame(2000); //display for 2 seconds
}

void Face::angry_transition_reverse() {
    set_eyebrowColor(10, 0, 0);
    int speed = 100;
    angry_blink();
    set_eyeColor(90, 0, 25);
    angry_frame4();
    m_Matrix.displayFrame(speed);
    set_eyeColor(60, 0, 50);
    angry_frame3();
    m_Matrix.displayFrame(speed);
    set_eyeColor(30, 0, 75);
    angry_frame2();
    m_Matrix.displayFrame(speed);
    set_eyeColor(0, 0, 100);

}

void Face::loading() {
    int speed = 100;
    loading_frame1();
    m_Matrix.displayFrame(speed);
    loading_frame2();
    m_Matrix.displayFrame(speed);
    loading_frame3();
    m_Matrix.displayFrame(speed);
    loading_frame4();
    m_Matrix.displayFrame(speed);
    loading_frame5();
    m_Matrix.displayFrame(speed);
    loading_frame6();
    m_Matrix.displayFrame(speed);
    loading_frame7();
    m_Matrix.displayFrame(speed);
    loading_frame8();
    m_Matrix.displayFrame(speed);
}

void Face::startup() {
    int speed = 100;
    startup_frame1();
    m_Matrix.displayFrame(speed);
    startup_frame2();
    m_Matrix.displayFrame(speed);
    startup_frame3();
    m_Matrix.displayFrame(speed);
    startup_frame4();
    m_Matrix.displayFrame(speed);
    set_eyeColor(0, 0, 3);
    startup_frame5();
    m_Matrix.displayFrame(speed);
    set_eyeColor(0, 0, 3);
    startup_frame6();
    m_Matrix.displayFrame(speed);
    set_eyeColor(0, 0, 3);
    startup_frame7();
    m_Matrix.displayFrame(speed);
    set_eyeColor(0, 0, 3);
    startup_frame8();
    m_Matrix.displayFrame(speed);
    set_eyeColor(0, 0, 5);
    startup_frame9();
    m_Matrix.displayFrame(speed);
    set_eyeColor(0, 0, 10);
    startup_frame10();
    m_Matrix.displayFrame(speed);
    set_eyeColor(0, 0, 30);
    startup_frame11();
    m_Matrix.displayFrame(speed);
    set_eyeColor(0, 0, 60);
    startup_frame12();
    m_Matrix.displayFrame(speed);
    set_eyeColor(0, 0, 100);
    happy_frame1(0);
    m_Matrix.displayFrame(100);
    blink(); 
}

void Face::surprise() {
    set_eyebrowColor(0, 0, 5);
    int speed = 75;
    surprise_frame1();
    m_Matrix.displayFrame(speed);
    surprise_frame2();
    m_Matrix.displayFrame(speed);
    surprise_frame3();
    m_Matrix.displayFrame(speed);
    surprise_frame4();
    m_Matrix.displayFrame(speed);
    surprise_frame5();
    m_Matrix.displayFrame(2000); //hold for 2 seconds
}

void Face::surprise_reverse() {
    set_eyebrowColor(0, 0, 5);
    int speed = 100;
    surprise_reverse_frame1();
    m_Matrix.displayFrame(speed);
    surprise_reverse_frame2();
    m_Matrix.displayFrame(speed);
    surprise_reverse_frame3();
    m_Matrix.displayFrame(speed);
    surprise_reverse_frame4();
    m_Matrix.displayFrame(speed);
}


void Face::testline() {
    for (int x = 0; x < 16; x++){
        for (int y = 0; y < 16; y++) {
            m_Matrix.mapLEDXY(x, y, eyeColor);
            m_Matrix.displayFrame(100);
        }
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
    //frame 1
    blink_frame1();
    m_Matrix.displayFrame(100);

    //frame 2
    basic_smile(0);
    m_Matrix.displayFrame(100);

    //frame 3
    blink_frame1();
    m_Matrix.displayFrame(100);
}

void Face::angry_blink() {

    angry_blink_frame1();
    m_Matrix.displayFrame(100);

    angry_mouth();
    angry_eyebrows();
    m_Matrix.displayFrame(100);

    angry_blink_frame1();
    m_Matrix.displayFrame(100);
}

//***********  Single Frames  ************* //

void Face::happy_frame1(int h) {
    //h represents the 'height' of the face on the screen (useful for bounce code)
    //h should be zero for the default position. 
    basic_eyes(h);
    basic_smile(h); 
}

void Face::blink_frame1() {
    //eyes
    m_Matrix.mapLEDXY(2, 10, eyeColor); m_Matrix.mapLEDXY(3, 10, eyeColor); m_Matrix.mapLEDXY(12, 10, eyeColor); m_Matrix.mapLEDXY(13, 10, eyeColor);
    //mouth
    basic_smile(0);
}

void Face::happy_em_frame1() {
    //eyes
    m_Matrix.mapLEDXY(2, 9, eyeColor); m_Matrix.mapLEDXY(3, 9, eyeColor); m_Matrix.mapLEDXY(12, 9, eyeColor); m_Matrix.mapLEDXY(13, 9, eyeColor);
    //mouth
    basic_smile(0); 
}
void Face::happy_em_frame2() {
    //eyes
    happy_em_eyes(); 
//mouth
    basic_smile(0);
}
void Face::happy_em_frame3() {
    happy_em_frame2();
    // add cheeks 
    byte cheekColor[3] = { 5,0,1 };
    m_Matrix.mapLEDXY(1, 11, cheekColor); m_Matrix.mapLEDXY(2, 11, cheekColor); m_Matrix.mapLEDXY(13, 11, cheekColor); m_Matrix.mapLEDXY(14, 11, cheekColor);
}

void Face::angry_frame1() {
    //eyes
    m_Matrix.mapLEDXY(2, 10, eyeColor); m_Matrix.mapLEDXY(3, 10, eyeColor); m_Matrix.mapLEDXY(12, 10, eyeColor); m_Matrix.mapLEDXY(13, 10, eyeColor);
    //mouth
    angry_mouth();
}
void Face::angry_frame2() {
    //eyes
    basic_eyes(0);
    //mouth
    angry_mouth();
}
void Face::angry_frame3() {
    //eyes and mouth same as previous frame 
    angry_frame2();
    //start to add eyebrows 
    m_Matrix.mapLEDXY(4, 7, eyebrowColor); m_Matrix.mapLEDXY(11, 7, eyebrowColor);
}
void Face::angry_frame4() {
    // mouth same as previous frame 
    angry_mouth();
    //eyes become angry
    angry_eyes();
    //add eyebrows
    m_Matrix.mapLEDXY(3, 6, eyebrowColor); m_Matrix.mapLEDXY(4, 7, eyebrowColor); m_Matrix.mapLEDXY(11, 7, eyebrowColor); m_Matrix.mapLEDXY(12, 6, eyebrowColor);
}
void Face::angry_frame5() {
    //eyes and mouth same as previous frame 
    angry_mouth();
    angry_eyes();
    //finish eyebrows
    angry_eyebrows();
}

void Face::angry_blink_frame1() {
    angry_mouth();
    angry_eyebrows();
    //eyes
    m_Matrix.mapLEDXY(2, 10, eyeColor); m_Matrix.mapLEDXY(3, 10, eyeColor); m_Matrix.mapLEDXY(12, 10, eyeColor); m_Matrix.mapLEDXY(13, 10, eyeColor);
}

//// Loading Screen
void Face::loading_frame1() {
    m_Matrix.mapLEDXY(6, 7, mouthColor); m_Matrix.mapLEDXY(6, 8, mouthColor); m_Matrix.mapLEDXY(7, 9, mouthColor);
}
void Face::loading_frame2() {
    m_Matrix.mapLEDXY(6, 8, mouthColor); m_Matrix.mapLEDXY(7, 9, mouthColor); m_Matrix.mapLEDXY(8, 9, mouthColor);
}
void Face::loading_frame3() {
    m_Matrix.mapLEDXY(7, 9, mouthColor); m_Matrix.mapLEDXY(8, 9, mouthColor); m_Matrix.mapLEDXY(9, 8, mouthColor);
}
void Face::loading_frame4() {
    m_Matrix.mapLEDXY(8, 9, mouthColor); m_Matrix.mapLEDXY(9, 7, mouthColor); m_Matrix.mapLEDXY(9, 8, mouthColor);
}
void Face::loading_frame5() {
    m_Matrix.mapLEDXY(8, 6, mouthColor); m_Matrix.mapLEDXY(9, 7, mouthColor); m_Matrix.mapLEDXY(9, 8, mouthColor);
}
void Face::loading_frame6() {
    m_Matrix.mapLEDXY(7, 6, mouthColor); m_Matrix.mapLEDXY(8, 6, mouthColor); m_Matrix.mapLEDXY(9, 7, mouthColor);
}
void Face::loading_frame7() {
    m_Matrix.mapLEDXY(6, 7, mouthColor); m_Matrix.mapLEDXY(7, 6, mouthColor); m_Matrix.mapLEDXY(8, 6, mouthColor);
}
void Face::loading_frame8() {
    m_Matrix.mapLEDXY(6, 7, mouthColor); m_Matrix.mapLEDXY(6, 8, mouthColor); m_Matrix.mapLEDXY(7, 6, mouthColor);
}

///  Loading to Startup Transition 
void Face::startup_frame1(){
    m_Matrix.mapLEDXY(6, 8, mouthColor); m_Matrix.mapLEDXY(6, 9, mouthColor); m_Matrix.mapLEDXY(6, 10, mouthColor);
}
void Face::startup_frame2() {
    m_Matrix.mapLEDXY(6, 10, mouthColor); m_Matrix.mapLEDXY(6, 11, mouthColor); m_Matrix.mapLEDXY(6, 12, mouthColor);
}
void Face::startup_frame3() {
    m_Matrix.mapLEDXY(6, 11, mouthColor); m_Matrix.mapLEDXY(6, 12, mouthColor); m_Matrix.mapLEDXY(7, 13, mouthColor);
}
void Face::startup_frame4() {
    m_Matrix.mapLEDXY(6, 12, mouthColor); m_Matrix.mapLEDXY(7, 13, mouthColor); m_Matrix.mapLEDXY(8, 13, mouthColor);
}
void Face::startup_frame5() {
    //mouth
    m_Matrix.mapLEDXY(7, 13, mouthColor); m_Matrix.mapLEDXY(8, 13, mouthColor); m_Matrix.mapLEDXY(9, 12, mouthColor);
    //eyes
    m_Matrix.mapLEDXY(7, 11, eyeColor); m_Matrix.mapLEDXY(8, 11, eyeColor);
}
void Face::startup_frame6(){
    //mouth
    m_Matrix.mapLEDXY(8, 13, mouthColor); m_Matrix.mapLEDXY(9, 11, mouthColor); m_Matrix.mapLEDXY(9, 12, mouthColor);
    //eyes
    m_Matrix.mapLEDXY(6, 10, eyeColor); m_Matrix.mapLEDXY(9, 10, eyeColor);
}
void Face::startup_frame7() {
    //mouth
    m_Matrix.mapLEDXY(8, 10, mouthColor); m_Matrix.mapLEDXY(9, 11, mouthColor); m_Matrix.mapLEDXY(9, 12, mouthColor);
    //eyes
    m_Matrix.mapLEDXY(5, 10, eyeColor); m_Matrix.mapLEDXY(10, 10, eyeColor);
}
void Face::startup_frame8() {
    //mouth
    m_Matrix.mapLEDXY(7, 10, mouthColor); m_Matrix.mapLEDXY(8, 10, mouthColor); m_Matrix.mapLEDXY(9, 11, mouthColor);
    //eyes
    m_Matrix.mapLEDXY(4, 10, eyeColor); m_Matrix.mapLEDXY(11, 10, eyeColor);
}
void Face::startup_frame9() {
    //mouth
    m_Matrix.mapLEDXY(6, 11, mouthColor); m_Matrix.mapLEDXY(7, 10, mouthColor); m_Matrix.mapLEDXY(8, 10, mouthColor);
    //eyes
    m_Matrix.mapLEDXY(3, 10, eyeColor); m_Matrix.mapLEDXY(12, 10, eyeColor);
}
void Face::startup_frame10() {
    //mouth
    m_Matrix.mapLEDXY(6, 11, mouthColor); m_Matrix.mapLEDXY(6, 12, mouthColor); m_Matrix.mapLEDXY(7, 10, mouthColor);
    //eyes
    m_Matrix.mapLEDXY(2, 10, eyeColor); m_Matrix.mapLEDXY(3, 10, eyeColor); m_Matrix.mapLEDXY(12, 10, eyeColor); m_Matrix.mapLEDXY(13, 10, eyeColor);
}
void Face::startup_frame11() {
    //mouth
    m_Matrix.mapLEDXY(6, 11, mouthColor); m_Matrix.mapLEDXY(6, 12, mouthColor); m_Matrix.mapLEDXY(7, 13, mouthColor);
    //eyes
    m_Matrix.mapLEDXY(2, 10, eyeColor); m_Matrix.mapLEDXY(3, 9, eyeColor); m_Matrix.mapLEDXY(3, 10, eyeColor); m_Matrix.mapLEDXY(12, 9, eyeColor); m_Matrix.mapLEDXY(12, 10, eyeColor); m_Matrix.mapLEDXY(13, 10, eyeColor);
}
void Face::startup_frame12(){
    m_Matrix.mapLEDXY(6, 12, mouthColor); m_Matrix.mapLEDXY(7, 13, mouthColor); m_Matrix.mapLEDXY(8, 13, mouthColor);
    basic_eyes(0);
}

//Surprise Frames 
void Face::surprise_frame1() {
    //eyes
    basic_eyes(0);
    //mouth
    m_Matrix.mapLEDXY(7, 12, mouthColor); m_Matrix.mapLEDXY(7, 13, mouthColor); m_Matrix.mapLEDXY(8, 12, mouthColor); m_Matrix.mapLEDXY(8, 13, mouthColor);
}
void Face::surprise_frame2() {
    //eyes
    m_Matrix.mapLEDXY(1, 9, eyeColor); m_Matrix.mapLEDXY(1, 10, eyeColor); m_Matrix.mapLEDXY(2, 8, eyeColor); m_Matrix.mapLEDXY(2, 9, eyeColor); m_Matrix.mapLEDXY(2, 10, eyeColor); m_Matrix.mapLEDXY(3, 8, eyeColor); m_Matrix.mapLEDXY(3, 9, eyeColor); m_Matrix.mapLEDXY(3, 10, eyeColor); m_Matrix.mapLEDXY(12, 8, eyeColor); m_Matrix.mapLEDXY(12, 9, eyeColor); m_Matrix.mapLEDXY(12, 10, eyeColor); m_Matrix.mapLEDXY(13, 8, eyeColor); m_Matrix.mapLEDXY(13, 9, eyeColor); m_Matrix.mapLEDXY(13, 10, eyeColor); m_Matrix.mapLEDXY(14, 9, eyeColor); m_Matrix.mapLEDXY(14, 10, eyeColor);
    //mouth
    surprised_mouth(); 
    //eyebrows
    m_Matrix.mapLEDXY(2, 7, eyebrowColor); m_Matrix.mapLEDXY(3, 7, eyebrowColor); m_Matrix.mapLEDXY(12, 7, eyebrowColor); m_Matrix.mapLEDXY(13, 7, eyebrowColor);
}
void Face::surprise_frame3() {
    //eyes
    surprised_eyes(); 
    //mouth
    surprised_mouth(); 
    //eyebrows
    m_Matrix.mapLEDXY(1, 6, eyebrowColor); m_Matrix.mapLEDXY(2, 6, eyebrowColor); m_Matrix.mapLEDXY(3, 6, eyebrowColor); m_Matrix.mapLEDXY(12, 6, eyebrowColor); m_Matrix.mapLEDXY(13, 6, eyebrowColor); m_Matrix.mapLEDXY(14, 6, eyebrowColor);
}
void Face::surprise_frame4() {
    //eyes
    surprised_eyes(); 
    //mouth 
    surprised_mouth(); 
    //eyebrows
    m_Matrix.mapLEDXY(1, 5, eyebrowColor); m_Matrix.mapLEDXY(2, 5, eyebrowColor); m_Matrix.mapLEDXY(3, 5, eyebrowColor); m_Matrix.mapLEDXY(12, 5, eyebrowColor); m_Matrix.mapLEDXY(13, 5, eyebrowColor); m_Matrix.mapLEDXY(14, 5, eyebrowColor);
}
void Face::surprise_frame5() {
    //eyes
    surprised_eyes();
    //mouth 
    surprised_mouth();
    //eyebrows
    m_Matrix.mapLEDXY(1, 4, eyebrowColor); m_Matrix.mapLEDXY(2, 4, eyebrowColor); m_Matrix.mapLEDXY(3, 4, eyebrowColor); m_Matrix.mapLEDXY(12, 4, eyebrowColor); m_Matrix.mapLEDXY(13, 4, eyebrowColor); m_Matrix.mapLEDXY(14, 4, eyebrowColor);
}

//Surprise reverse transition 
void Face::surprise_reverse_frame1() {
    //eyes
    m_Matrix.mapLEDXY(1, 9, eyeColor); m_Matrix.mapLEDXY(1, 10, eyeColor); m_Matrix.mapLEDXY(2, 9, eyeColor); m_Matrix.mapLEDXY(2, 10, eyeColor); m_Matrix.mapLEDXY(2, 11, eyeColor); m_Matrix.mapLEDXY(3, 9, eyeColor); m_Matrix.mapLEDXY(3, 10, eyeColor); m_Matrix.mapLEDXY(3, 11, eyeColor); m_Matrix.mapLEDXY(4, 9, eyeColor); m_Matrix.mapLEDXY(4, 10, eyeColor); m_Matrix.mapLEDXY(11, 9, eyeColor); m_Matrix.mapLEDXY(11, 10, eyeColor); m_Matrix.mapLEDXY(12, 9, eyeColor); m_Matrix.mapLEDXY(12, 10, eyeColor); m_Matrix.mapLEDXY(12, 11, eyeColor); m_Matrix.mapLEDXY(13, 9, eyeColor); m_Matrix.mapLEDXY(13, 10, eyeColor); m_Matrix.mapLEDXY(13, 11, eyeColor); m_Matrix.mapLEDXY(14, 9, eyeColor); m_Matrix.mapLEDXY(14, 10, eyeColor);
    //mouth
    surprised_mouth();
    //eyebrows
    m_Matrix.mapLEDXY(1, 4, eyebrowColor); m_Matrix.mapLEDXY(2, 4, eyebrowColor); m_Matrix.mapLEDXY(3, 4, eyebrowColor); m_Matrix.mapLEDXY(12, 4, eyebrowColor); m_Matrix.mapLEDXY(13, 4, eyebrowColor); m_Matrix.mapLEDXY(14, 4, eyebrowColor);

}
void Face::surprise_reverse_frame2() {
    //eyes
    m_Matrix.mapLEDXY(2, 11, eyeColor); m_Matrix.mapLEDXY(3, 11, eyeColor); m_Matrix.mapLEDXY(12, 11, eyeColor); m_Matrix.mapLEDXY(13, 11, eyeColor);
    //mouth
    surprised_mouth();
    //eyebrows
    m_Matrix.mapLEDXY(1, 5, eyebrowColor); m_Matrix.mapLEDXY(2, 5, eyebrowColor); m_Matrix.mapLEDXY(3, 5, eyebrowColor); m_Matrix.mapLEDXY(12, 5, eyebrowColor); m_Matrix.mapLEDXY(13, 5, eyebrowColor); m_Matrix.mapLEDXY(14, 5, eyebrowColor);
}
void Face::surprise_reverse_frame3() {
    //eyes
    //mouth
    surprised_mouth();
    //eyebrows
    m_Matrix.mapLEDXY(1, 6, eyebrowColor); m_Matrix.mapLEDXY(2, 6, eyebrowColor); m_Matrix.mapLEDXY(3, 6, eyebrowColor); m_Matrix.mapLEDXY(12, 6, eyebrowColor); m_Matrix.mapLEDXY(13, 6, eyebrowColor); m_Matrix.mapLEDXY(14, 6, eyebrowColor);
}
void Face::surprise_reverse_frame4() {
    //eyes
    m_Matrix.mapLEDXY(2, 11, eyeColor); m_Matrix.mapLEDXY(3, 11, eyeColor); m_Matrix.mapLEDXY(12, 11, eyeColor); m_Matrix.mapLEDXY(13, 11, eyeColor);
    //mouth
    m_Matrix.mapLEDXY(7, 12, mouthColor); m_Matrix.mapLEDXY(7, 13, mouthColor); m_Matrix.mapLEDXY(8, 12, mouthColor); m_Matrix.mapLEDXY(8, 13, mouthColor);
    //eyebrows
    m_Matrix.mapLEDXY(1, 7, eyebrowColor); m_Matrix.mapLEDXY(2, 7, eyebrowColor); m_Matrix.mapLEDXY(13, 7, eyebrowColor); m_Matrix.mapLEDXY(14, 7, eyebrowColor);
}



//***********  Frame Elements  ************* //

void Face::basic_smile(int h) {
    //mouth
    m_Matrix.mapLEDXY(6, h + 12, mouthColor); m_Matrix.mapLEDXY(7, h + 13, mouthColor); m_Matrix.mapLEDXY(8, h + 13, mouthColor); m_Matrix.mapLEDXY(9, h + 12, mouthColor);
}

void Face::basic_eyes(int h) {
    //eyes
    m_Matrix.mapLEDXY(2, h + 9, eyeColor); m_Matrix.mapLEDXY(2, h + 10, eyeColor); m_Matrix.mapLEDXY(3, h + 9, eyeColor); m_Matrix.mapLEDXY(3, h + 10, eyeColor);
    m_Matrix.mapLEDXY(12, h + 9, eyeColor); m_Matrix.mapLEDXY(12, h + 10, eyeColor); m_Matrix.mapLEDXY(13, h + 9, eyeColor); m_Matrix.mapLEDXY(13, h + 10, eyeColor);
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
