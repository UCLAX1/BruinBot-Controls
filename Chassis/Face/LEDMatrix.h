/*
  The LEDMatrix class contains all the functions which write directly to the LED matrix.
  This class contains the driver function RGB_update as well as the publicly accessible function MapLEDXY
  Most of the core code in this class is originally created by Kevin Darrah.

  Rebecca Celsi 2020
*/

#ifndef LEDMATRIX_H
#define LEDMATRIX_H

#include "Arduino.h"

class LEDMatrix
{
public:
    LEDMatrix(int pin);
    void mapLEDXY(int y, int x, byte color[]);
    void clearLEDs();
    void displayFrame(int hold); 

private:
    void RGB_update(int LED, byte RED, byte GREEN, byte BLUE);

    int m_pin;
    int numberOfLEDs = 256;// total number of RGB LEDs; don't change this
    byte RGB[768]; 
};

#endif
