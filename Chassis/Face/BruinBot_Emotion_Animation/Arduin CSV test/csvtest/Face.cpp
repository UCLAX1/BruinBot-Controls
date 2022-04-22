/*
  Face.cpp: code to operate a Face object with different emotion codes.
  Public functions contain emotion sequences such as happy_standby, angry_transition, angry_standby, bounce, etc.
  Private functions contain individual frames with their names/numbers to make it easier to concatenate animation.
  The functions which directly access the LED matrix are contained in the LEDMatrix class.

  Rebecca Celsi 2020
  Created to run a 16x16 NeoPixel WS2812B matrix. 
*/

#include "Face.h"
//#include "LEDMatrix.h" 
#include<string.h>
#include<Vector.h>
#include <iostream>
#include <fstream>
#include <sstream>

// The Face objejct contains a set of emotion objects and an LED matrix object. 
// As the main Arduino code receives commands, it adds the data from each emotion to a "queue"
// of frames which are then displayed by the LED matrix in sequence. 
Face::Face(int pin)
    //:m_Matrix(pin)
{
    
}
void Face::readFile(string fName)
{
    vector<Pixel> pixelList;
    vector<vector<string>> content;
    vector<string> row;
    string line, word;

    fstream file(fName, ios::in);
    if (file.is_open())
    {
        while (getline(file, line))
        {
            //prepare to parse data for a new row.
            row.clear();

            stringstream str(line);

            // read in the data of a single line, separating by commas
            while (getline(str, word, ','))
                row.push_back(word);
            content.push_back(row);
        }
    }
    else
        cout << "Could not open the file\n";

    // The .csv file is now converted to a vector of vectors of strings. 
    // however, it will help to have a function that searches for a keyword
    // and creates a pixel object using the data listed under that keyword. 
    
    int i = 0;
    //name of current frame
    string name = content[i][0];
    cout << name + "= name \n";
    //iterate until reaching the break 
    for (int j = 1; j < 10; j++)
    {
        cout << content[j][0];
        Pixel pixel;
        pixel.x = stoi(content[j][0]);
        pixel.y = stoi(content[j][1]);
        pixel.color = content[i][2];
        pixelList.push_back(pixel);

        cout << pixel.x + "= x";
        cout << pixel.y + "= y";
        cout << pixel.color + "= color";
    }
}


int main() {
    Face myFace(8);
    myFace.readFile("test.csv");
}


/*
vector<Frame> Face::getFrames(string emotion) {

}
vector<Pixel> Face::getPixels(string frame) {

}
string getNextEmotion(string emotion) {

}
string getInterrupEmotion(string emotion) {

}
*/
