#include <Servo.h>
int SERVO_PIN = 9;
int GROUND_JOY_PIN = A3;            
int VOUT_JOY_PIN = A2;              
int XJOY_PIN = A1;                  
Servo myservo ;

void setup() {
  Serial.begin(9600);
  pinMode(VOUT_JOY_PIN, OUTPUT) ;    //pin A3 shall be used as output
  pinMode(GROUND_JOY_PIN, OUTPUT) ;  //pin A2 shall be used as output
  digitalWrite(VOUT_JOY_PIN, HIGH) ; //set pin A3 to high (+5V)
  digitalWrite(GROUND_JOY_PIN,LOW) ; //set pin A3 to low (ground)
  myservo.attach(9);
}

void loop() {
 delay(200);                    
 int joystickXVal = analogRead(XJOY_PIN) ;  //read joystick input on pin A1
 Serial.print(joystickXVal);                //print the value from A1
 Serial.println(" = input from joystick");  //print "=input from joystick" next to the value
 Serial.print((joystickXVal+520)/10);       //print a from A1 calculated, scaled value
 Serial.println(" = output to servo");      //print "=output to servo" next to the value
 Serial.println() ;
 myservo.write((joystickXVal+520)/10);
}
