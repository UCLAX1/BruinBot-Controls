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

//#include "LEDMatrix.h"
//#include"Emotion.h"
#include<string.h>
#include<Vector.h>
using namespace std;

struct Frame {
    string name;
    int delay;
};
struct Pixel {
    int x;
    int y;
    string color;
};

class Face
{
public:
    Face(int pin);
    // The Face contains exactly one instance of each type of emotion (which is each a different class type.)
    // The data for the frames making up each emotion is contained within their class definitions; however, 
    // The emotion "web" or chain of progression from one emotion to the next is defined inside the face. 
    // Each emotion is contructed with pointers to its nextEmotion and interruptEmotion.
    // The resulting structure is kind of similar to a linked list. 
    // The capitalized phrases are the class types; the lowercase phrases are the instances of those classes. 

     /*
    possible emotions:
    happy_standby
    neutral_standby
    happy_emphasis(int hold);
    surprise
    surprise_reverse
    angry_transition
    angry_transition_reverse();
    angry_standby
    sad_standby
    sad_transition
    sad_transition_reverse
    loading
    startup
    bounce
    */

    /*
    Happy_standby* happy_standby(happy_standby, happy_standby); 
    Neutral_standby* neutral_standby(happy_standby, happy_standby);
    Happy_Emphasis_Reverse* happy_emphasis_reverse(happy_standby, happy_standby);
    Happy_Emphasis_Hold* happy_emphasis_hold(happy_emphasis_reverse, happy_emphasis_reverse));
    Happy_Emphasis_Transition* happy_emphasis_transition(happy_emphasis_hold, happy_emphasis_reverse);
    */

    //vector<Frame> getFrames(string emotion);
    //vector<Pixel> getPixels(string frame);
    //string getNextEmotion(string emotion);
    //string getInterrupEmotion(string emotion); 
    //bool completesSelf(string emotion); 
    //LEDMatrix* matrix; 
    void readFile(string fileName); 
    /*
    //the eye and mouth color could potentially be changed outside the face object
    void set_eyeColor(byte RED, byte GREEN, byte BLUE);
    void set_mouthColor(byte RED, byte GREEN, byte BLUE);
    void set_eyebrowColor(byte RED, byte GREEN, byte BLUE);
    */
private:
    //Animation Snippets
    void blink();
    void angry_blink();
    void sad_blink(); 
    void neutral_blink(); 
    void bored_blink(); 

    //Single Frames
    void happy_frame1(int h);
    void blink_frame1(); 
    void neutral_frame1(); 
    void bored_frame1();
    void happy_em_frame1();
    void happy_em_frame2();
    void happy_em_frame3();
    void angry_frame1();
    void angry_frame2();
    void angry_frame3();
    void angry_frame4();
    void angry_frame5();
    void angry_blink_frame1();
    void angry_blink_frame2();
    void sad_frame1();
    void sad_frame2();
    void sad_frame3();
    void sad_frame4();
    void sad_frame5();
    void sad_frame6();
    void sad_blink_frame1();
    void sad_blink_frame2();
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
    void neutral_mouth();
    void happy_em_eyes();
    void angry_eyes();
    void angry_mouth();
    void sad_mouth();
    void angry_eyebrows(); 
    void surprised_eyes();
    void surprised_mouth();
    
    /*
    //default eye colors: rgb 0,0,100 (blue) 
    byte eyeColor[3] = { 0,0,100 };
    //default mouth colors: rgb 10,10,10 (white)
    byte mouthColor[3] = { 10,10,10 };
    byte eyebrowColor[3] = {10,0,0};
    */
    //LEDMatrix m_Matrix;
};

#endif
