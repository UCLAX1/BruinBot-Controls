/*
  Face.h: header file to operate a Face object with different emotion codes.
  See Face.cpp for code documentation and structure explanation. 

  Rebecca Celsi 2022
  Created to run a 16x16 NeoPixel WS2812B matrix.
*/

#ifndef FACE_H
#define FACE_H
#include"Arduino.h"
#include"LEDMatrix.h"
#include"String.h"
#include"Vector.h"
#include"QueueList.h"
//#include"Array.h"

class Pixel 
{
public: 
    Pixel(int xcoord, int ycoord,  byte pixelColor);
    Pixel(const Pixel& old);
    int x;
    int y;
    byte color;
};

class Frame
{
public:
    Frame(Vector<Pixel*>pixels, int del, int h);
    ~Frame();
    int delay;
    Vector<Pixel*> pixelList;
};

class Emotion
{
public:
    Emotion(Vector<Frame*> frames, Emotion* nextE, Emotion* interruptE, bool completes = true);
    Emotion(Vector<Frame*> frames, bool completes = false);
    Emotion(Vector<Frame*> frames, Emotion* interruptE, bool completes = false);
    ~Emotion(); 
    Vector<Frame*> frameList;
    Emotion* nextEmotion;
    Emotion* interruptEmotion;
    bool completesSelf;
};

class Face
{
public:
    Face(int pin);
    void displayFrame(Frame* currentFrame);
    QueueList<Frame*> frameQueue; 
    Emotion* currentEmotion;
    Frame* currentFrame;
    void addFrames(Emotion* emotion);
    void clearQueue();

    //LEDMatrix* matrix; 

    // "frame snippets" that contain a handful of pixel objects representing only the mouth, eyes, etc.

    Frame basic_smile;
    Frame basic_eyes;
    
   // Frame neutral_mouth;
    //Frame happy_em_eyes;
    Frame angry_mouth; 
    Frame angry_eyes;
    Frame angry_eyebrows;
    //Frame sad_mouth;
   // Frame surprised_eyes;
   // Frame surprised_mouth;
    

    // "frames" that represent all pixels displayed on the face at a given instant. 

    Frame happy_frame1;
    Frame blink_frame1;
    /*
    Frame neutral_frame1;
    Frame bored_frame1;
    Frame happy_em_frame1;
    Frame happy_em_frame2;
    Frame happy_em_frame3;
    */
    Frame angry_frame1;
    Frame angry_frame2;
    Frame angry_frame3;
    Frame angry_frame4;
    Frame angry_frame5;
    Frame angry_blink_frame1;
    Frame angry_blink_frame2;
    /*
    Frame sad_frame1;
    Frame sad_frame2;
    Frame sad_frame3;
    Frame sad_frame4;
    Frame sad_frame5;
    Frame sad_frame6;
    Frame sad_blink_frame1;
    Frame sad_blink_frame2;
    Frame loading_frame1;
    Frame loading_frame2;
    Frame loading_frame3;
    Frame loading_frame4;
    Frame loading_frame5;
    Frame loading_frame6;
    Frame loading_frame7;
    Frame loading_frame8;
    Frame startup_frame1;
    Frame startup_frame2;
    Frame startup_frame3;
    Frame startup_frame4;
    Frame startup_frame5;
    Frame startup_frame6;
    Frame startup_frame7;
    Frame startup_frame8;
    Frame startup_frame9;
    Frame startup_frame10;
    Frame startup_frame11;
    Frame startup_frame12;
    Frame startup_frame13;
    Frame surprise_frame1;
    Frame surprise_frame2;
    Frame surprise_frame3;
    Frame surprise_frame4;
    Frame surprise_frame5;
    Frame surprise_reverse_frame1;
    Frame surprise_reverse_frame2;
    Frame surprise_reverse_frame3;
    Frame surprise_reverse_frame4;
    */

    // "Emotion snippets" representing a snippet of animation like a blink

    Emotion blink;
    Emotion angry_blink;
    /*
    Emotion sad_blink;
    Emotion neutral_blink;
    */



    // "Emotions" containing a full sequence of frames
   
    Emotion happy_standby;
    Emotion angry_transition_reverse;
    Emotion angry_standby;
    Emotion angry_transition; 
    /*
    Emotion neutral_standby;
    Emotion  happy_emphasis_reverse;
    Emotion  happy_emphasis_hold;
    Emotion  happy_emphasis_transition;
    */
    //etc.
  

private:

    //LEDMatrix m_Matrix;

    // COLORS 
    // usually, a pixel will use one of these variables to set its own color, although occasionally it will define a custom color to use. 
        //default eye color: blue
        int eyeColor[3] = { 0,0,100 };
        //default mouth colors: white
        int mouthColor[3] = { 10,10,10 };
        //default eyebrowColor: dark blue
        int eyebrowColor[3] = { 10,0,0 };
        // specialty colors for transitional animations
        int angryEyebrowColor[3] = { 10, 0, 0 };
        int angryEyeColor[3] = { 120, 0, 0 };

    

    // Helper Functions
    Vector<Frame*> concatenate(Vector<Vector<Frame*>> Vectors_to_add);
    Vector<Pixel*> concatenate(Vector<Vector<Pixel*>> Vectors_to_add);
    Vector<Pixel*> changeColor(Vector<Pixel*> ogPixels, Vector<int>color);

   
};

#endif
