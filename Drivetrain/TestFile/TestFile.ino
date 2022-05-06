#include <Servo.h>
int SERVO_PIN = 9;              
Servo myservo ;

void setup() {
  Serial.begin(9600);
  myservo.attach(SERVO_PIN);
}

void loop() {
 delay(200);                    
 myservo.write(10);
}
