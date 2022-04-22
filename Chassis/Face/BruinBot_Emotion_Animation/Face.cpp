/*
  Face.cpp: code to operate a Face object with different emotion codes.
  Code Structure: 
        A Face object is created with a pin variable (representing the physical pin the LED matrix is attached to). Upon construction , it executes a long initializer list which sets up every other member object according to pre-created data (meaning, I decoded the pixel coordinates for every pixel of every frame). The objects constructed build upon each other in complexity. They are as follows: 

        a. Pixel: 
            - contains x, y, and color data. 
        b. Frame: 
            - contains a Vector of Pixel pointers that make up that frame 
            - contains suggested delay time for frame (can be overwritten) 
        c. Emotion: 
            - contains a Vector of Frame pointers that make up the emotion in sequence 
            - contains a "nextEmotion" (pointer to another Emotion object)
            - contains an "interruptEmotion" (pointer to another Emotion object)
            - contains a bool "completes_self" which indicates if the emotion should interrupt its own animation while playing 
        d. Face: 
            - contains constructed member objects of each different emotion, which in turn contains lists of frames containing lists of pixels. 
  
  Some of the frames are more accurately categorized as "frame snippets," containg only the data to display the mouth, eyes, etc. Likewise, some of the emotions are more like "emotion snippets" containing a snippet of animation like a blink or looping action. These 'snippets' can be concatenated into the pixelList or frameList of a frame or emotion upon construction, eliminating duplicate code. 
  
  The emotions' values of "nextEmotion", "interruptEmotion", and "completesSelf" are used to inform the behavior of the "emotion web," an algorithmic flowchart that determines how one emotion flows to the next. This behavior is coded outside of the Face class, in the Arduino file running the main loop. 

  Please keep in mind that my motivation while writing this was to set up the full behavior of each pixel, frame, and emotion in as few lines of code as possible. Yes, there are places where I could imporve memory by destructing pixels or frames after they are displayed; but any handling of the data outside an object's constructor quickly adds up to hundreds of lines of near-duplicate code and I just aint about that. 
     
  The functions which directly access the LED matrix are contained in the LEDMatrix class.
 
  Rebecca Celsi 2022
  Created to run a 16x16 NeoPixel WS2812B matrix. 
*/

#include "Face.h"
#include "LEDMatrix.h" 
#include "Arduino.h"
//#include<iostream>
//#include <sys/utime.h>
//#include <ctime>
//#include <windows.h>
//#define quote(x) #x



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// **********************************************   COMPONENT CONSTRUCTORS   *******************************************

// Present the constructors for each class type.
Pixel::Pixel(int xcoord, int ycoord, byte pixelColor) 
{
    x = xcoord; 
    y = ycoord;
    color = pixelColor;
}

Pixel::Pixel( const Pixel& old) { //pass in a reference to an old circle
    x = old.x;
    y = old.y;
    color = old.color;
}

Frame::Frame(Vector<Pixel*> pixels, int del = 100, int h = 0)
{
    // Assign world-level pointer info to member variables. 
    delay = del;
    pixelList = pixels;
}
// destruct the dynamically-allocated pixelList when destructing a frame. 
Frame::~Frame() {
    Vector<Pixel*>::VectorIterator objectIt;
    for (objectIt = begin(pixelList); objectIt != end(pixelList); ++objectIt) {
        delete* objectIt;
    }
}
// Constructor: Construct each emotion by passing in all its relevant data and assigning it directly.  
Emotion::Emotion(Vector<Frame*> frames, Emotion* nextE, Emotion* interruptE, bool completes)
{
    // Assign world-level pointer info to member variables. 
    nextEmotion = nextE;
    interruptEmotion = interruptE; 
    completesSelf = completes;
    frameList = frames;
}
// This alternate constructor is used for an emotion where both its pointers point at itself. 
// It is used for happy_standby as well as frame snippets. 
Emotion::Emotion(Vector<Frame*> frames, bool completes)
{
    // Assign world-level pointer info to member variables. 
    nextEmotion = this;
    interruptEmotion = this;
    // by default, an emotion is set to continue its actions even when receiving an interrupt. 
    completesSelf = completes;
    frameList = frames;
}
//this constructor can be used for any emotion that loops (i.e. its nextEmotion is itself)
Emotion::Emotion(Vector<Frame*> frames, Emotion* interruptE, bool completes)
{
    // Assign world-level pointer info to member variables. 
    nextEmotion = this;
    interruptEmotion = interruptE;
    // by default, an emotion is set to continue its actions even when receiving an interrupt. 
    completesSelf = completes;
    frameList = frames;
}
// Destruct list of frames (some emotions contain synamically-allocated frames)
Emotion::~Emotion() {
    Vector<Frame*>::VectorIterator objectIt;
    for (objectIt = begin(frameList); objectIt != end(frameList); ++objectIt) {
        delete* objectIt;
    }
}
// The Face objejct contains a set of emotion objects and an LED matrix object. 
Face::Face(int pin)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// **********************************************   FRAME AND EMOTION INITIALIZATION   *********************************

// Initializer list for the Face class contains all of the actual data making up the frames and emotions 
// Frames: construct frames with a list of pixels and a delay time (optional)
// Default delay time is 100 ms (10 frames/sec) 
// Use "concatenate" function to combine lists with other lists (useful for adding 'snippets' to the frameList or pixelList of an object)

:
basic_smile(
    {
    new Pixel(6, 12, mouthColor),
    new Pixel(7, 13, mouthColor),
    new Pixel(8, 13, mouthColor),
    new Pixel(9, 12, mouthColor)
    }),

basic_eyes(
    {
    new Pixel(2, 9, eyeColor),
    new Pixel(2, 10, eyeColor),
    new Pixel(3, 9, eyeColor),
    new Pixel(3, 10, eyeColor),
    new Pixel(12, 9, eyeColor),
    new Pixel(12, 10, eyeColor),
    new Pixel(13, 9, eyeColor),
    new Pixel(13, 10, eyeColor)
    }),
angry_mouth(
    {
    new Pixel(6,13, mouthColor),
    new Pixel(7,13, mouthColor),
    new Pixel(8,13, eyeColor),
    new Pixel(9,13, eyeColor)
    }),
angry_eyes(
    {
     new Pixel(2,9, angryEyeColor),
    new Pixel(2, 10, angryEyeColor),
    new Pixel(3, 10, angryEyeColor),
    new Pixel(12, 10, angryEyeColor),
    new Pixel(13,9, angryEyeColor),
    new Pixel(13,10, angryEyeColor)
    }),
angry_eyebrows(
    {
    new Pixel(2,5, angryEyebrowColor),
    new Pixel(3,6, angryEyebrowColor),
    new Pixel(4,7, angryEyebrowColor),
    new Pixel(11,7, angryEyebrowColor),
    new Pixel(12,6, angryEyebrowColor),
    new Pixel(13,5, angryEyebrowColor)
    }),

// *********************************************************    FRAMES   **************************************************
happy_frame1(
    concatenate({
        basic_eyes.pixelList,
        basic_smile.pixelList
    })
),
blink_frame1(
    concatenate({
        {
        new Pixel(2, 10, eyeColor),
        new Pixel(3, 10, eyeColor),
        new Pixel(12, 10, eyeColor),
        new Pixel(13, 10, eyeColor)
        },
        basic_smile.pixelList
    })
),
angry_frame1(
    concatenate({
        {
        new Pixel(2, 10, eyeColor),
        new Pixel(3, 10, eyeColor),
        new Pixel(12, 10, eyeColor),
        new Pixel(13, 10, eyeColor)
        },
        angry_mouth.pixelList
    })
),
angry_frame2(
    concatenate({
        changeColor(basic_eyes.pixelList, { 30, 0, 75 }), 
        angry_mouth.pixelList 
    })
),
angry_frame3(
    concatenate({
        {
        new Pixel(4, 7, angryEyebrowColor),
        new Pixel(11, 7, angryEyebrowColor)
        },
        changeColor(basic_eyes.pixelList, {60, 0, 50}),
        angry_mouth.pixelList,
       
    })
),
angry_frame4(
    concatenate({
        {
        new Pixel(3,6, angryEyebrowColor),
        new Pixel(4,7, angryEyebrowColor),
        new Pixel(11,7, angryEyebrowColor),
        new Pixel(12,6, angryEyebrowColor)
        },
        changeColor(angry_eyes.pixelList, {90, 0, 25}),
        angry_mouth.pixelList
    })
),
angry_frame5(
    concatenate({
        angry_mouth.pixelList,
        angry_eyes.pixelList,
        angry_eyebrows.pixelList
    })
),
angry_blink_frame1(
    concatenate({
       {
       new Pixel(2, 10, angryEyeColor),
       new Pixel(3, 10, angryEyeColor),
       new Pixel(12, 10, angryEyeColor),
       new Pixel(13, 10, angryEyeColor),
       },
       angry_mouth.pixelList,
       angry_eyebrows.pixelList
    })
),
angry_blink_frame2(
    concatenate({
        angry_mouth.pixelList,
        angry_eyebrows.pixelList
        })
),
// *****************************************************   EMOTIONS *****************************************************

blink(
   { &blink_frame1, &basic_smile, &blink_frame1 }
),
angry_blink(
    { &angry_blink_frame1, &angry_blink_frame2, &angry_blink_frame1 }
),

// in rare cases, an emoion will need to alter the delay time of a frame, in which case it will create a duplicate frame with a different delay. 
happy_standby(
    concatenate({
        { new Frame(happy_frame1.pixelList, 2000)},
        blink.frameList,
        { &happy_frame1 }
    })
),
angry_transition_reverse(
    { &angry_frame5, &angry_frame4, &angry_frame3, &angry_frame2, &angry_frame1 },
    &happy_standby,
    &happy_standby
),
angry_standby({
    concatenate({
        {new Frame(angry_frame5.pixelList, 2000)},
        angry_blink.frameList,
        {&angry_frame5}
    }),
    &angry_transition_reverse,
    true
}),

angry_transition(
    { &angry_frame1, &angry_frame2, &angry_frame3, &angry_frame4, &angry_frame5 },
    &angry_standby,
    &angry_transition_reverse,
    true
),


    m_matrix(pin)
{
    //Rest of Face constructor
    currentEmotion = &happy_standby;

}

  
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// ***************************************   HELPER FUNCTIONS   *****************************************

/*
Handwritten 'concatenate Vectors in one line' function because apparently none exist.Instead of taking in a variable number of arguments, the function instead requires that all input args (which are all Vectors of component pointers) are first combined into one Vector to be parsed. The following two "concatenate" functions are identical, but one handles Frames and one handles Pixels because c++ won't let me create a function with variable return type (and it doesn't make sense to use pointers here)
*/
// copy constructor 


// feed in a Vector of pixel pointers and get out the same list, with the color of every pixel chanegd to a new color
    
void Face::displayFrame(Frame* frame) {
    Vector<Pixel*> ::VectorVectorIterator pixIt;
    for (pixIt = begin(frame->pixelList); pixIt != end(frame->pixelList); ++pixIt) {
        Pixel newPix = **pixIt;
        int x = newPix.x;
        int y = newPix.y;
        Vector<int> color = newPix.color;
        m_matrix.mapLEDXY(x, y, color); 
    }
    cout << "changed frame ";
}
Vector<Pixel*>  Face::changeColor(Vector<Pixel*> ogPixels, byte color) {
    
    Vector<Pixel*> newPixels;
    Vector<Pixel*> ::VectorIterator pixIt;
    for (pixIt = begin(ogPixels); pixIt != end(ogPixels); ++pixIt) {
        // point to the old pixel to extract the x and y values from it 
        Pixel* oldPix = *pixIt;
        newPixels.insert(newPixels.end(), new Pixel(oldPix->x, oldPix->y, color));
    }
    return newPixels;
}


// Add an emotion's frames to the current frame queue. (all pass by reference)
void  Face::addFrames(Emotion* emotion) {
    Vector<Frame*> ::VectorIterator frameIt;
    for (frameIt = begin(emotion->frameList); frameIt != end(emotion->frameList); ++frameIt) {
        frameQueue.push(*frameIt);
    }
}

// clears the entire queue except for the current (front) frame
void  Face::clearQueue() {
    // save the current frame
    Frame* currentFrame = frameQueue.peek();
    while (!frameQueue.empty()) {
        frameQueue.pop();
    }
    frameQueue.push(currentFrame);
}
   
Vector<Frame*> Face::concatenate(Vector<Vector<Frame*>> vectors_to_add) {

    Vector<Frame*> combined;
    // create an VectorIterator to iterate through the multiple "arguments"
    Vector<Vector<Frame*>> ::VectorIterator argsIt;
    for (argsIt = begin(vectors_to_add); argsIt != end(vectors_to_add); ++argsIt) {

        // create an VectorIterator to iterate through the multiple components in each entry
        Vector<Frame*> ::VectorIterator objectIt;
        for (objectIt = begin(*argsIt); objectIt != end(*argsIt); ++objectIt) {

            // add the frame to the combined Vector. 
            combined.push_back(*objectIt);
        }
    }
    return combined;
}
// Duplicate concatenate function for Pixels. 
Vector<Pixel*> Face::concatenate(Vector<Vector<Pixel*>> vectors_to_add) {

    Vector<Pixel*> combined;

    for (Vector<Pixel*> pixelList : vectors_to_add) {

        // create an VectorIterator to iterate through the multiple components in each entry
        //Vector<Pixel*> ::VectorIterator objectIt;
        for (Pixel* pixel : pixelList)) {

            // add the pixel to the combined Vector. 
            combined.push_back(pixel);
        }
    return combined;
}
