/*************************************************** 
  This is an example for our Adafruit 16-channel PWM & Servo driver
  Servo test - this will drive 8 servos, one after the other on the
  first 8 pins of the PCA9685

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/815
  
  These drivers use I2C to communicate, 2 pins are required to  
  interface.

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);
// you can also call it with a different address and I2C interface
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40, Wire);

// Depending on your servo make, the pulse width min and max may vary, you 
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
#define SERVOMIN  150 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // This is the 'maximum' pulse length count (out of 4096)
#define USMIN  600 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX  2400 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates
#define FORWARD 1000 // "forward" direction for payload servo in us
#define SNACKTIME 2600 // ms of delay to dispense a snack
#define BELTTIME 4000 // ms of delay to run the belt

// our servo # counter
uint8_t servonum = 0;
String command = "";
int motorpin1 = 8;
int motorpin2 = 9;

void setup() {
  Serial.begin(115200);
  Serial.println("payload");

  pwm.begin();
  /*
   * In theory the internal oscillator (clock) is 25MHz but it really isn't
   * that precise. You can 'calibrate' this by tweaking this number until
   * you get the PWM update frequency you're expecting!
   * The int.osc. for the PCA9685 chip is a range between about 23-27MHz and
   * is used for calculating things like writeMicroseconds()
   * Analog servos run at ~50 Hz updates, It is importaint to use an
   * oscilloscope in setting the int.osc frequency for the I2C PCA9685 chip.
   * 1) Attach the oscilloscope to one of the PWM signal pins and ground on
   *    the I2C PCA9685 chip you are setting the value for.
   * 2) Adjust setOscillatorFrequency() until the PWM update frequency is the
   *    expected value (50Hz for most ESCs)
   * Setting the value here is specific to each individual I2C PCA9685 chip and
   * affects the calculations for the PWM update frequency. 
   * Failure to correctly set the int.osc value will cause unexpected PWM results
   */
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates

  delay(10);
  pwm.writeMicroseconds(1, 0);
  pwm.writeMicroseconds(5, 0);
  pwm.writeMicroseconds(9, 0);
  pwm.writeMicroseconds(13, 0);
  pwm.writeMicroseconds(0, 1800);
  pinMode(motorpin1, OUTPUT);
  pinMode(motorpin2, OUTPUT);
}


void loop() {
//  readSerialPort();/
  while (Serial.available()) {  // check for incoming serial data
    delay(3);
    command = Serial.readString();
  }
  // http://adafruit.github.io/Adafruit-PWM-Servo-Driver-Library/html/class_adafruit___p_w_m_servo_driver.html#aa91cf057ec01505292401e4fdceb57ac
  // pwm.writeMicroseconds (channel, microseconds)
  // depends on servo, generally 500~2500 us, with neutral ~1500 us
  // 0 us =  off, 1500 = neutral position but powered.
  if(command == "snack1"){
    Serial.println("Dispensing snack 1");
    pwm.writeMicroseconds(1, FORWARD);
    delay(SNACKTIME);
    pwm.writeMicroseconds(1, 0);
    belt(4500);
    arm();
  }
  else if(command == "snack2"){
    Serial.println("Dispensing snack 2");
    pwm.writeMicroseconds(5, FORWARD);
    delay(SNACKTIME);
    pwm.writeMicroseconds(5, 0);
    belt(3750);
    arm();
  }
  else if(command == "snack3"){
    Serial.println("Dispensing snack 3");
    pwm.writeMicroseconds(9, FORWARD);
    delay(SNACKTIME);
    pwm.writeMicroseconds(9, 0);
    belt(3000);
    arm();
  }
   else if(command == "snack4"){
    Serial.println("Dispensing snack 4");
    pwm.writeMicroseconds(13, FORWARD);
    delay(SNACKTIME);
    pwm.writeMicroseconds(13, 0);
    belt(2500);
    arm();
  }
  else if(command == "arm"){
    Serial.println("moving arm");
    pwm.writeMicroseconds(0, 1350);
    delay(SNACKTIME);
    for(int i = 1350; i <= 1800; i += 5){
      pwm.writeMicroseconds(0, i);
      delay(25);
    }
    delay(100);
    pwm.writeMicroseconds(0, 0);
  }
  else if(command == "belt"){
    Serial.println("moving belt");
    digitalWrite(motorpin1, LOW);
    digitalWrite(motorpin2, HIGH);
    delay(BELTTIME);
    digitalWrite(motorpin1, LOW);
    digitalWrite(motorpin2, LOW);
  }
  command = "";
}


void arm(){
  Serial.println("moving arm");
  for(int i = 1800; i >= 1350; i -= 5){
    pwm.writeMicroseconds(0, i);
    delay(15);
  }
  delay(SNACKTIME);
  for(int i = 1350; i <= 1800; i += 5){
    pwm.writeMicroseconds(0, i);
    delay(25);
  }
  delay(100);
  pwm.writeMicroseconds(0, 0);
}
void belt(int beltTime){
  Serial.println("moving belt");
  digitalWrite(motorpin1, LOW);
  digitalWrite(motorpin2, HIGH);
  delay(beltTime);
  digitalWrite(motorpin1, LOW);
  digitalWrite(motorpin2, LOW);
}

// command reads data sent to the serial port from the raspberry pi
void readSerialPort() {
  command = "";
  if (Serial.available()) {
    delay(10);
    while (Serial.available() > 0) {
      command += (char)Serial.read();
    }
    Serial.flush();
  }
}

//This function estbalishes a connection with the raspberry pi (currently it is never called because the raspberry pi is not being used)
boolean establish_connection() {
    while (!(Serial.available() > 0)) {} // hangs until a msg is received
    char rcv = Serial.read();
    if (rcv == 'i') {
        Serial.println("payload");
    }

    int start = millis();
    while (!(Serial.available() > 0)) {
        if (millis() - start > 1000) {
            return false;
        }
    }
    rcv = Serial.read();
    if (rcv == 'd') {
        // Pi has acknowledged this arduino
        return true;
    }
    else {
        return false;
    }
}
