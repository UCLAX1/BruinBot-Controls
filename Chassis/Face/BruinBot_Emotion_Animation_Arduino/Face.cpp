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
#include "stdarg.h"
#include "stdio.h"
#include "stdlib.h"
#include "QueueList.h"
//#include<iostream>
//#include <sys/utime.h>
//#include <ctime>
//#include <windows.h>
//#define quote(x) #x
// 
// 
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



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// **********************************************   COMPONENT CONSTRUCTORS   *******************************************

// Present the constructors for each class type.
Pixel::Pixel(int xcoord, int ycoord, int pixelColor[3])
{
    x = xcoord; 
    y = ycoord;
    copy(pixelColor, color, 3);
}

Pixel::Pixel( const Pixel& old) { //pass in a reference to an old pixel
    x = old.x;
    y = old.y;
    copy(old.color, color, 3);
}

//Frame receives a pointer to the first entry of an array of Pixel pointers, as well as an int number of pixels. They immediately copy the array into their own data (so the pointer that is read in is not needed again). 
Frame::Frame(int num_pixels, Pixel** pixels, int h = 0)
{
    // Assign world-level pointer info to member variables. 
    numPixels = num_pixels;
    copy(pixels, pixelList, numPixels);
}
// destruct the dynamically-allocated pixelList when destructing a frame. 
Frame::~Frame() {

    for (int i = 0; i < numPixels; i++) {
        delete pixelList[i];
    }
}
// Constructor: Construct each emotion by passing in all its relevant data and assigning it directly.  
Emotion::Emotion(Frame* frames[], int num_frames, Emotion* nextE, Emotion* interruptE, bool completes)
{
    // Assign world-level pointer info to member variables. 
    nextEmotion = nextE;
    interruptEmotion = interruptE; 
    completesSelf = completes;
    copy(frames, frameList, num_frames);
}
// This alternate constructor is used for an emotion where both its pointers point at itself. 
// It is used for happy_standby as well as frame snippets. 
Emotion::Emotion(Frame* frames[], int num_frames, bool completes)
{
    // Assign world-level pointer info to member variables. 
    nextEmotion = this;
    interruptEmotion = this;
    // by default, an emotion is set to continue its actions even when receiving an interrupt. 
    completesSelf = completes;
    copy(frames, frameList, MAX_PIXELS);
}
//this constructor can be used for any emotion that loops (i.e. its nextEmotion is itself)
Emotion::Emotion(Frame* frames[], int num_frames, Emotion* interruptE, bool completes)
{
    // Assign world-level pointer info to member variables. 
    nextEmotion = this;
    interruptEmotion = interruptE;
    // by default, an emotion is set to continue its actions even when receiving an interrupt. 
    completesSelf = completes;
    copy(frames, frameList, MAX_PIXELS);
}
// Destruct list of frames (some emotions contain synamically-allocated frames)
Emotion::~Emotion() {
    
    for (int i = 0; i < numFrames; i++) {
        delete frameList[i];
    }
}

// ********************************  STUPID LONG-ASS LISTS OF PIXELS ******************************************
// 
//   I promise you I wrote all this code in a way that was 10000% more efficient and then I had to rewrite everything because of the dumb limitations of the Arduino programming language. 

// These lists represent the lists of pixels that will later be imported into each frame. Each list comes with a constant int of the same name representing its length. 


const int BASIC_SMILE = 4; 
const int BASIC_EYES = 8;
const int ANGRY_MOUTH= 4; 

const int ANGRY_EYES = 6; 
Pixel* angry_eyebrows_p[] = {
    new Pixel(2,5, angryEyebrowColor),
    new Pixel(3,6, angryEyebrowColor),
    new Pixel(4,7, angryEyebrowColor),
    new Pixel(11,7, angryEyebrowColor),
    new Pixel(12,6, angryEyebrowColor),
    new Pixel(13,5, angryEyebrowColor)
};
const int ANGRY_EYEBROWS = 6; 
Pixel* blink_frame1_p[] = {
    new Pixel(2, 10, eyeColor),
    new Pixel(3, 10, eyeColor),
    new Pixel(12, 10, eyeColor),
    new Pixel(13, 10, eyeColor)
};
const int BLINK_FRAME1 = 4; 





// The Face objejct contains a set of emotion objects and an LED matrix object. 
Face::Face(int pin)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// **********************************************   FRAME AND EMOTION INITIALIZATION   *********************************

// Initializer list for the Face class contains all of the actual data making up the frames and emotions 
// Frames: construct frames with a # of pixels and a list of pixels
// Use "concatenate" function to combine lists with other lists (useful for adding 'snippets' to the frameList or pixelList of an object)

:
// ****************************************************    FRAME SNIPPETS   **************************************************

//Construct the "Frame Snippets" from the pre-written list of pixels. 
basic_smile(
    BASIC_SMILE,       
    makePArray(
        BASIC_SMILE, 
        new Pixel(6, 12, mouthColor),
        new Pixel(7, 13, mouthColor),
        new Pixel(8, 13, mouthColor),
        new Pixel(9, 12, mouthColor)
    )
),
basic_eyes(
    BASIC_EYES, 
    makePArray(
        BASIC_EYES,
        new Pixel(2, 9, eyeColor),
        new Pixel(2, 10, eyeColor),
        new Pixel(3, 9, eyeColor),
        new Pixel(3, 10, eyeColor),
        new Pixel(12, 9, eyeColor),
        new Pixel(12, 10, eyeColor),
        new Pixel(13, 9, eyeColor),
        new Pixel(13, 10, eyeColor)
    )
),
angry_mouth(
    ANGRY_MOUTH,     
    makePArray(
        ANGRY_MOUTH,
        new Pixel(6,13, mouthColor),
        new Pixel(7,13, mouthColor),
        new Pixel(8,13, eyeColor),
        new Pixel(9,13, eyeColor)
    )
),
angry_eyes(
    ANGRY_EYES,
    makePArray(
        ANGRY_EYES,
        new Pixel(2, 9, angryEyeColor),
        new Pixel(2, 10, angryEyeColor),
        new Pixel(3, 10, angryEyeColor),
        new Pixel(12, 10, angryEyeColor),
        new Pixel(13, 9, angryEyeColor),
        new Pixel(13, 10, angryEyeColor)
    )
),
angry_eyebrows(
    ANGRY_EYEBROWS,
    makePArray(
        ANGRY_EYEBROWS,
        new Pixel(2, 5, angryEyebrowColor),
        new Pixel(3, 6, angryEyebrowColor),
        new Pixel(4, 7, angryEyebrowColor),
        new Pixel(11, 7, angryEyebrowColor),
        new Pixel(12, 6, angryEyebrowColor),
        new Pixel(13, 5, angryEyebrowColor)
    )
),
// *********************************************************    FRAMES   **************************************************
happy_frame1(
    BASIC_EYES + BASIC_SMILE,
    concatenate(
        BASIC_EYES,
        BASIC_SMILE,
        basic_eyes.pixelList, 
        basic_smile.pixelList
    )
),
blink_frame1(
    4 + BASIC_SMILE,
    concatenate(
        4,
        BASIC_SMILE,
        makePArray(
            4,
            new Pixel(2, 10, eyeColor),
            new Pixel(3, 10, eyeColor),
            new Pixel(12, 10, eyeColor),
            new Pixel(13, 10, eyeColor)
        ),
        basic_smile.pixelList
    )
),
angry_frame1(
    4 + ANGRY_MOUTH,
    concatenate(
        4, ANGRY_MOUTH,
        makePArray(
            4, 
            new Pixel(2, 10, eyeColor),
            new Pixel(3, 10, eyeColor),
            new Pixel(12, 10, eyeColor),
            new Pixel(13, 10, eyeColor)
            ),
        angry_mouth.pixelList
    )
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
*/
// *****************************************************   EMOTIONS *****************************************************

blink(concatenate()
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

// Copy an array of Pixels into another array (since arrays are immutable) 
void copy(Pixel* OGPixels[], Pixel* newArray[], int arraySize) { //Copy function
    for (int i = 0; i < arraySize; i++) {
        newArray[i] = OGPixels[i];
    }
}
// Same thing but with Frames
void copy(Frame* OGFrames[], Frame* newArray[], int arraySize) { //Copy function
    for (int i = 0; i < arraySize; i++) {
        newArray[i] = OGFrames[i];
    }
}
// Same thing but with bytes
void copy(const int OGints[], int newArray[], int arraySize) { //Copy function
    for (int i = 0; i < arraySize; i++) {
        newArray[i] = OGints[i];
    }
}
//Display a frame using the tools of the LEDMatrix class. 
void Face::displayFrame(Frame* frame) {
    Vector<Pixel*> ::iterator pixIt;
    for (pixIt = begin(frame->pixelList); pixIt != end(frame->pixelList); ++pixIt) {
        Pixel newPix = **pixIt;
        int x = newPix.x;
        int y = newPix.y;
        Vector<int> color = newPix.color;
        m_matrix.mapLEDXY(x, y, color); 
    }
    cout << "changed frame ";
}
// feed in a Vector of pixel pointers and get out the same list, with the color of every pixel chanegd to a new color

Vector<Pixel*>  Face::changeColor(Vector<Pixel*> ogPixels, byte color) {
    
    Vector<Pixel*> newPixels;
    Vector<Pixel*> ::iterator pixIt;
    for (pixIt = begin(ogPixels); pixIt != end(ogPixels); ++pixIt) {
        // point to the old pixel to extract the x and y values from it 
        Pixel* oldPix = *pixIt;
        newPixels.insert(newPixels.end(), new Pixel(oldPix->x, oldPix->y, color));
    }
    return newPixels;
}


// Add an emotion's frames to the current frame queue. (all pass by reference)
void  Face::addFrames(Emotion* emotion) {
    Vector<Frame*> ::iterator frameIt;
    for (frameIt = begin(emotion->frameList); frameIt != end(emotion->frameList); ++frameIt) {
        frameQueue.push(*frameIt);
    }
}

// clears the entire queue except for the current (front) frame
void  Face::clearQueue() {
    // save the current frame
    Frame* currentFrame = frameQueue.peek();
    while (!frameQueue.isEmpty) {
        frameQueue.pop();
    }
    frameQueue.push(currentFrame);
}
// makes an array out of a list of arguments in one line (just like is possible in the std library) 
Frame** makeFArray(int num, ...) {
    va_list args;
    static Frame* newArray[20];
    //Initializing arguments to store all values after num
    va_start(args, num);
    //Sum all the inputs; we still rely on the function caller to tell us how many there are
    for (int x = 0; x < num; x++)
    {
        newArray[x] = va_arg(args, Frame*);
    }
    va_end(args);                  // Cleans up the list
    return newArray;
}
Pixel** makePArray(int num, ...) {
    va_list args;
    static Pixel* newArray[20];
    //Initializing arguments to store all values after num
    va_start(args, num);
    //Sum all the inputs; we still rely on the function caller to tell us how many there are
    for (int x = 0; x < num; x++)
    {
        newArray[x] = va_arg(args, Pixel*);
    }
    va_end(args);                  // Cleans up the list
    return newArray;
}
// Concatenate for frames: adds two arrays of frames together. 
//concatenate returns a pointer to a static, new array created that contains all of the values we wanted to add together. 

Frame** Face::concatenate(int size1, int size2, Frame* list1[], Frame* list2[]) {
    static Frame* newArray[20]; 
    int i = 0; 
    for (int j = 0; j < size1; j++) {
        newArray[i] = list1[j]; 
        i++;
    }
    for (int j = 0; j < size2; j++) {
        newArray[i] = list2[j];
        i++;
    }
    return newArray;
}
// Duplicate concatenate function for Pixels. 
Pixel** Face::concatenate(int size1, int size2, Pixel* list1[], Pixel* list2[]) {
    static Pixel* newArray[20];
    int i = 0;
    for (int j = 0; j < size1; j++) {
        newArray[i] = list1[j];
        i++;
    }
    for (int j = 0; j < size2; j++) {
        newArray[i] = list2[j];
        i++;
    }
    return newArray;
}
