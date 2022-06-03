/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 https://www.arduino.cc/en/Tutorial/LibraryExamples/Sweep
*/

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  myservo.write(120);
}

void loop() {
  int pos1 = 95;
  int pos2 = 150;
  int t = 15;

  for (int i = pos1; i < pos2; i++) {
    myservo.write(i);
    delay(t);
  }
  delay(3000);

  for (int i = pos2; i > pos1; i--) {
    myservo.write(i);
    delay(t);
  }
  delay(3000);

}
