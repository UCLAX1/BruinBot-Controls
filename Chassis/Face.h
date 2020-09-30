/*
  Face.h: header file to operate a Face object with different emotion codes.
  Public functions contain emotion sequences such as happy_standby, angry_transition, angry_standby, bounce, etc.
  Private functions contain individual frames with their names/numbers to make it easier to concatenate animation.
  The functions which directly access the LED matrix are contained in the LEDMatrix class.

  Rebecca Celsi 2020
  Created to run a 16x16 NeoPixel WS2812B matrix.
*/


#ifndef FACE_H
#define FACE_H

#include "Arduino.h"
#include "LEDMatrix.h"

class Face
{
public:
    Face(int pin);
    void happy_standby();
    void bounce(int speed);
    void happy_emphasis(int hold); 
    void angry_transition(); 
    void angry_transition_reverse(); 
    void angry_standby();
    void loading(); 
    void startup(); 
    void surprise(); 
    void surprise_reverse(); 
    void testline(); 

    //the eye and mouth color could potentially be changed outside the face object
    void set_eyeColor(byte RED, byte GREEN, byte BLUE);
    void set_mouthColor(byte RED, byte GREEN, byte BLUE);
    void set_eyebrowColor(byte RED, byte GREEN, byte BLUE);


private:
    //Animation Snippets
    void blink();
    void angry_blink();

    //Single Frames
    void happy_frame1(int h);
    void blink_frame1(); 
        void happy_em_frame1();
        void happy_em_frame2();
        void happy_em_frame3();
    void angry_frame1();
    void angry_frame2();
    void angry_frame3();
    void angry_frame4();
    void angry_frame5();
    void angry_blink_frame1();
         void loading_frame1(); 
         void loading_frame2();
         void loading_frame3();
         void loading_frame4();
         void loading_frame5();
         void loading_frame6();
         void loading_frame7();
         void loading_frame8();
    void startup_frame1(); 
    void startup_frame2();
    void startup_frame3();
    void startup_frame4();
    void startup_frame5();
    void startup_frame6();
    void startup_frame7();
    void startup_frame8();
    void startup_frame9();
    void startup_frame10();
    void startup_frame11();
    void startup_frame12();
    void startup_frame13();
        void surprise_frame1(); 
        void surprise_frame2();
        void surprise_frame3();
        void surprise_frame4();
        void surprise_frame5();
    void surprise_reverse_frame1(); 
    void surprise_reverse_frame2();
    void surprise_reverse_frame3();
    void surprise_reverse_frame4();


    //Frame Elements
    void basic_smile(int h);
    void basic_eyes(int h); 
    void happy_em_eyes();
    void angry_eyes();
    void angry_mouth();
    void angry_eyebrows(); 
    void surprised_eyes();
    void surprised_mouth(); 

    //default eye colors: rgb 0,0,100 (blue) 
    byte eyeColor[3] = { 0,0,100 };
    //default mouth colors: rgb 10,10,10 (white)
    byte mouthColor[3] = { 10,10,10 };
    byte eyebrowColor[3] = {10,0,0};

    LEDMatrix m_Matrix;
};

#endif
