# Chassis
Code for electronics used in the chassis subsystem, which include touchscreen and LED panels.

## Touchscreen app
Simple GUI app using [PyGames](https://www.pygame.org/docs/tut/PygameIntro.html).

Has 3 buttons for user to select an item for dispense from the payload system.

Currently only prints to the command line for demo purposes.

## Face 
BruinBot's face is a 16x16 WS2812 NeoPixel Matrix, run via an Arduino module on the main Chassis Pi. 

Frame-by-frame animations are now coded for the LED Matrix face, which can be seen running in sequence with the "Face_Demo" Arduino sketch in the Face folder. The dynamically-updating version is called "New_Face_Code" and includes functionality to dynamically modify the facial expression by typing or sending commands to the serial monitor. Commands like "<angry>", "<sad>", "<happy_emphasis>" etc. trigger the face to transition between emotions. A full list of emotions is written in the .cpp files. The expression code depends on the classes Face and LEDMatrix- to run the .ino files with full functionality, create an arduino sketch that includes the .ino file in addition to the .h and .cpp files for Face and LEDMatrix all within the same folder. 

To set up: plug the 3 wires of the face into Ground, 5V, and pin 8 on the Arduino. Create the sketch folder as described above, compile it from the Arduino IDE, and upload to the board. If running the demo code, the emotions will loop infinitely; if running the dynamic code, type commands into the serial monitor enclosed in brackets (ex: <surprise>). If running with the raspberry pi, make sure to enable the connection sequence ("establish_connection" function should be called during void setup()) and make sure the pi is sending the emotion commands correctly.
  
An Excel spreadsheet included with the files was used to turn drawings of the frames into output code. 
