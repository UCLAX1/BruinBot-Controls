/*
  Face.h: header file to operate a Face object with different emotion codes.
  See Face.cpp for code documentation and structure explanation. 

  Rebecca Celsi 2022
  Created to run a 16x16 NeoPixel WS2812B matrix.
*/

#ifndef FACE_H
#define FACE_H

//#include "LEDMatrix.h"
//#include"Emotion.h"
#include <string>
#include<stdlib.h> 
#include<iterator>
#include <vector>
#include <queue>

#include<pnew.cpp>
using namespace std;

class Pixel 
{
public: 
    Pixel(int xcoord, int ycoord, vector<int> pixelColor);
    Pixel(const Pixel& old);
    int x;
    int y;
    vector<int> color;
    int getX(); 
    int getY(); 
};

class Frame
{
public:
    Frame(vector<Pixel*>pixels, int del, int h);
    ~Frame();
    int delay;
    vector<Pixel*> pixelList;
};

class Emotion
{
public:
    Emotion(vector<Frame*> frames, Emotion* nextE, Emotion* interruptE, bool completes = true);
    Emotion(vector<Frame*> frames, bool completes = false);
    Emotion(vector<Frame*> frames, Emotion* interruptE, bool completes = false);
    ~Emotion(); 
    vector<Frame*> frameList;
    Emotion* nextEmotion;
    Emotion* interruptEmotion;
    bool completesSelf;
};

class Face
{
public:
    Face(int pin);
    void displayFrame(Frame* currentFrame);
    queue<Frame*> frameQueue; 
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
        vector<int> eyeColor = {0,0,100};
        //default mouth colors: white
        vector<int> mouthColor = { 10,10,10 };
        //default eyebrowColor: dark blue
        vector<int> eyebrowColor = { 10,0,0 };
        // specialty colors for transitional animations
        vector<int> angryEyebrowColor = { 10, 0, 0 };
        vector<int> angryEyeColor = { 120, 0, 0 };

    

    // Helper Functions
    vector<Frame*> concatenate(vector<vector<Frame*>> vectors_to_add);
    vector<Pixel*> concatenate(vector<vector<Pixel*>> vectors_to_add);
    vector<Pixel*> changeColor(vector<Pixel*> ogPixels, vector<int>color);

   
};

#endif
