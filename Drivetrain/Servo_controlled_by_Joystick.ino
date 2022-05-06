// Talon Motor Controller is modeled as a Servo in this code

#include <Servo.h>;
Servo Talon;  
int servoPin = 5;  // Output PWM Pin to Talon Motor Controller
int VRx = A0;  // Joystick Input Pin

int xPosition = 0;  // Represents Joystick Position
int percent = 0;  // Throttle percentage. Ranges from -100 (Full Reverse) to 100 (Full Forward)
int writeValue = 0; 

void setup() {
  Serial.begin(9600);
  pinMode(VRx, INPUT);
  Talon.attach(servoPin);
}

void loop() {
  xPosition = analogRead(VRx); // Reads Joystick Position along vertical axis
  //percent = map(xPosition, 0, 1023, -100, 100);  // Map this position to a range between -100 and 100
  percent = 0;
  while(percent < 25){
    percent++;
    Serial.println(percent);
    Talon.writeMicroseconds(1500 + (percent*5));
  }
  delay(100);
  while(percent > -25){
    percent--;
    Serial.println(percent);
    Talon.writeMicroseconds(1500 + (percent*5));
  }
  
  
  /*if (percent == 1){  // When the joystick is in the neutral position, the map function will output 1 instead of 0. This "if" statement fixes the problem.
    percent = 0;
  }
  */
    Talon.writeMicroseconds(1500 + (percent*5)); // Depending on the throttle percentage, send a PWM signal with period ranging from 1 to 2 ms to the Talon, 
                                                 // where 1.5 ms represents the neutral position. 
}
