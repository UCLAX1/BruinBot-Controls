/*
  Face.cpp: code to operate a Face object with different emotion codes.
  Code Structure: 
        A Face object is created with a pin variable (representing the physical pin the LED matrix is attached to). Upon construction , it executes a long initializer list which sets up every other member object according to pre-created data (meaning, I decoded the pixel coordinates for every pixel of every frame). The objects constructed build upon each other in complexity. They are as follows: 

        a. Pixel: 
            - contains x, y, and color data. 
        b. Frame: 
            - contains a vector of Pixel pointers that make up that frame 
            - contains suggested delay time for frame (can be overwritten) 
        c. Emotion: 
            - contains a vector of Frame pointers that make up the emotion in sequence 
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
//#include "LEDMatrix.h" 
#include<string>
#include<vector>
#include<queue>
#include<iostream>
#include <sys/utime.h>
#include <ctime>
#include <windows.h>
#define quote(x) #x


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// **********************************************   COMPONENT CONSTRUCTORS   *******************************************

// Present the constructors for each class type.
Pixel::Pixel(int xcoord, int ycoord, vector<int> pixelColor) 
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
int Pixel::getX() {
    return x;
}
int Pixel::getY() {
    return y;
}
Frame::Frame(vector<Pixel*> pixels, int del = 100, int h = 0)
{
    // Assign world-level pointer info to member variables. 
    delay = del;
    pixelList = pixels;
}
// destruct the dynamically-allocated pixelList when destructing a frame. 
Frame::~Frame() {
    vector<Pixel*>::iterator objectIt;
    for (objectIt = begin(pixelList); objectIt != end(pixelList); ++objectIt) {
        delete* objectIt;
    }
}
// Constructor: Construct each emotion by passing in all its relevant data and assigning it directly.  
Emotion::Emotion(vector<Frame*> frames, Emotion* nextE, Emotion* interruptE, bool completes)
{
    // Assign world-level pointer info to member variables. 
    nextEmotion = nextE;
    interruptEmotion = interruptE; 
    completesSelf = completes;
    frameList = frames;
}
// This alternate constructor is used for an emotion where both its pointers point at itself. 
// It is used for happy_standby as well as frame snippets. 
Emotion::Emotion(vector<Frame*> frames, bool completes)
{
    // Assign world-level pointer info to member variables. 
    nextEmotion = this;
    interruptEmotion = this;
    // by default, an emotion is set to continue its actions even when receiving an interrupt. 
    completesSelf = completes;
    frameList = frames;
}
//this constructor can be used for any emotion that loops (i.e. its nextEmotion is itself)
Emotion::Emotion(vector<Frame*> frames, Emotion* interruptE, bool completes)
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
    vector<Frame*>::iterator objectIt;
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
)


    //m_Matrix(pin)
{
    //Rest of Face constructor
    currentEmotion = &happy_standby;

}
// Test the emotion web code here, eventually transfer to Arduino. 
    int main() {

        // "void Setup"  {
            Face myFace(8); 

            // set up queue with its starting emotion
            cout << "Made face!"; 
           
            //push all frames of the first emotion to the queue. 
            myFace.addFrames(myFace.currentEmotion);

            myFace.currentFrame = myFace.frameQueue.front();
        
            time_t startTime = time(nullptr)*1000;
            time_t prevMillis = time(nullptr)*1000;
            time_t delay = myFace.currentFrame->delay;
            int total_frames = 0;
        // }

        // This loop represents the "void loop" of the arduino
        // put a time limit on it so its not infinite
        while (time(nullptr) - startTime < 1000000) {
            //cout << "Top of the while loop!";
          
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
            time_t currentMillis = time(nullptr) * 1000;

            // if enough time has elapsed to switch to the next frame
            //if ((currentMillis - prevMillis) > myFace.currentFrame->delay) {
            if ((currentMillis - prevMillis) > 20) {
                
                cout << "delay passed: " << myFace.currentFrame->delay << " ms \n";
                // check if frame needs to be refilled (only the current frame is left in it)
                if (size(myFace.frameQueue) == 1) {
                    //the current frame is the last frame in the list 
                    // frame queue is empty; find the new emotion and add it to the queue. 
                    //if (not interrupted) 
                    myFace.currentEmotion = myFace.currentEmotion->nextEmotion;

                    //push all frames of the new emotion to the queue. 
                    myFace.addFrames(myFace.currentEmotion);
                    cout << "pushed new emotions " << '\n';
                }

                // pop the current frame (which has now completed displaying) 
                myFace.frameQueue.pop();
                myFace.currentFrame = myFace.frameQueue.front();
                delay = myFace.currentFrame->delay; 
                prevMillis = currentMillis;
                total_frames++;

                // display new frame here (only displays once, when first initialized)
                myFace.displayFrame(myFace.currentFrame);
                cout << "current frame # of pixels: " << myFace.currentFrame->pixelList.size() << '\n';
                cout << "Current time: " << currentMillis; 
            }
        }
    }
        
  
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// ***************************************   HELPER FUNCTIONS   *****************************************

/*
Handwritten 'concatenate vectors in one line' function because apparently none exist.Instead of taking in a variable number of arguments, the function instead requires that all input args (which are all vectors of component pointers) are first combined into one vector to be parsed. The following two "concatenate" functions are identical, but one handles Frames and one handles Pixels because c++ won't let me create a function with variable return type (and it doesn't make sense to use pointers here)
*/
// copy constructor 


// feed in a vector of pixel pointers and get out the same list, with the color of every pixel chanegd to a new color
    
void Face::displayFrame(Frame* frame) {
    vector<Pixel*> ::iterator pixIt;
    for (pixIt = begin(frame->pixelList); pixIt != end(frame->pixelList); ++pixIt) {
        Pixel newPix = **pixIt;
        int x = newPix.x;
        int y = newPix.y;
        vector<int> color = newPix.color;
        // myFace.matrix.display(x, y, byte(color)); 
    }
    cout << "changed frame ";
}
vector<Pixel*>  Face::changeColor(vector<Pixel*> ogPixels, vector<int>color) {
    
    vector<Pixel*> newPixels;
    vector<Pixel*> ::iterator pixIt;
    for (pixIt = begin(ogPixels); pixIt != end(ogPixels); ++pixIt) {
        // point to the old pixel to extract the x and y values from it 
        Pixel* oldPix = *pixIt;
        newPixels.insert(newPixels.end(), new Pixel(oldPix->x, oldPix->y, color));
    }
    return newPixels;
}


// Add an emotion's frames to the current frame queue. (all pass by reference)
void  Face::addFrames(Emotion* emotion) {
    vector<Frame*> ::iterator frameIt;
    for (frameIt = begin(emotion->frameList); frameIt != end(emotion->frameList); ++frameIt) {
        frameQueue.push(*frameIt);
    }
}

// clears the entire queue except for the current (front) frame
void  Face::clearQueue() {
    // save the current frame
    Frame* currentFrame = frameQueue.front();
    while (!frameQueue.empty()) {
        frameQueue.pop();
    }
    frameQueue.push(currentFrame);
}
   
vector<Frame*> Face::concatenate(vector<vector<Frame*>> vectors_to_add) {

    vector<Frame*> combined;
    // create an iterator to iterate through the multiple "arguments"
    vector<vector<Frame*>> ::iterator argsIt;
    for (argsIt = begin(vectors_to_add); argsIt != end(vectors_to_add); ++argsIt) {

        // create an iterator to iterate through the multiple components in each entry
        vector<Frame*> ::iterator objectIt;
        for (objectIt = begin(*argsIt); objectIt != end(*argsIt); ++objectIt) {

            // add the frame to the combined vector. 
            combined.push_back(*objectIt);
        }
    }
    return combined;
}
// Duplicate concatenate function for Pixels. 
vector<Pixel*> Face::concatenate(vector<vector<Pixel*>> vectors_to_add) {

    vector<Pixel*> combined;

    // create an iterator to iterate through the multiple "arguments"
    vector<vector<Pixel*>> ::iterator argsIt;
    for (argsIt = begin(vectors_to_add); argsIt != end(vectors_to_add); ++argsIt) {

        // create an iterator to iterate through the multiple components in each entry
        vector<Pixel*> ::iterator objectIt;
        for (objectIt = begin(*argsIt); objectIt != end(*argsIt); ++objectIt) {

            // add the pixel to the combined vector. 
            combined.push_back(*objectIt);
        }
    }
    return combined;
}
