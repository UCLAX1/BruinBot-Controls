# Chassis
Code for electronics used in the chassis subsystem, which include touchscreen and LED panels.

## Touchscreen app
Simple GUI app using [PyGames](https://www.pygame.org/docs/tut/PygameIntro.html).

Has 3 buttons for user to select an item for dispense from the payload system.

Currently only prints to the command line for demo purposes.

## Face 
BruinBot's face is a 16x16 WS2812 NeoPixel Matrix, run via an Arduino module on the main Chassis Pi. 

Frame-by-frame animations are now coded for the LED Matrix face, which can be seen with the demo Arduino code in the Face folder. The BruinBot Face Library contains the classes Face and LEDMatrix. 

The emotions are public functions of the Face class, which can be called from an external program once system integration is underway. For now, the demo shows a sequence of the emotions on a loop. 
