#include "SparkFun_ProDriver_TC78H670FTG_Arduino_Library.h" //Click here to get the library: http://librarymanager/All#SparkFun_ProDriver
#include "ezButton.h"
PRODRIVER nodMotor; // Controls up and down movement
PRODRIVER swivelMotor;  // Controls swiveling movement
String command = "nothing";
String swivelCommand = "nothing";
String nodCommand = "nothing";
String stopCommand = "nothing";

int turnSwitch = 49;
int nodSwitch = 47;

void readSerialPort();

void setup() {
   Serial.begin(9600);

  // Initialize Driver Pins on Arduino Mega
  nodMotor.settings.standbyPin = 13;
  nodMotor.settings.enablePin = 11;
  nodMotor.settings.mode0Pin = 10;
  nodMotor.settings.mode1Pin = 9;
  nodMotor.settings.mode2Pin = 8;
  nodMotor.settings.mode3Pin = 7;
  nodMotor.settings.errorPin = 12;

  swivelMotor.settings.standbyPin = A15;
  swivelMotor.settings.enablePin = A13;
  swivelMotor.settings.mode0Pin = A12;
  swivelMotor.settings.mode1Pin = A11;
  swivelMotor.settings.mode2Pin = A10;
  swivelMotor.settings.mode3Pin = A9;
  swivelMotor.settings.errorPin = A14;
  // uncomment this when the raspberry pi is connected
  // while (!establish_connection()) {}
  
  // Set motor step resolution for both motors
  nodMotor.settings.stepResolutionMode = PRODRIVER_STEP_RESOLUTION_FIXED_1_16; // 1/16 step
  swivelMotor.settings.stepResolutionMode = PRODRIVER_STEP_RESOLUTION_FIXED_1_16; 
  nodMotor.begin();
  swivelMotor.begin();

  pinMode(turnSwitch, INPUT_PULLUP);

  // Home Turn Motor
  Serial.println("Homing Turn Motor");
  while(digitalRead(turnSwitch)){
    swivelMotor.step(1,0);
  }
  while(!digitalRead(turnSwitch)){
    swivelMotor.step(1,1);
  }
  
  int startTime = millis();
  // Home Nod Motor
  Serial.println("Homing Nod Motor");
  while(digitalRead(nodSwitch)){
     if(millis() - startTime < 10000){
      nodMotor.step(1,1);
    }
    else{
      nodMotor.step(1,0);
    }
  }
  while(!digitalRead(nodSwitch)){
    nodMotor.step(1,1);
  }



   Serial.println("Waiting for command");
}

void loop() {
   readSerialPort();
 
    //command.remove(command.length()-1);
    if (command.startsWith("Turn") > 0){
      swivelCommand = command;
      Serial.println(swivelCommand);
    }
    else if (command.startsWith("Nod") > 0){
      nodCommand = command;
    }
    else if (command.startsWith("Stop") > 0){
      stopCommand = command;
    }
  



  if (swivelCommand == "Turn Right"){
    swivelMotor.step(10, 1);
    Serial.println("Turning Right");
  }
  
  else if (swivelCommand == "Turn Left"){
    swivelMotor.step(10, 0);
    Serial.println("Turning Left");
  }

  if (nodCommand == "Nod Up"){
    nodMotor.step(10, 0);
    Serial.println("Nodding Up");
  }

  else if (nodCommand == "Nod Down"){
    nodMotor.step(10, 1);
    Serial.println("Nodding Down");
  }

  if (stopCommand == "Stop Turning"){
    swivelCommand = "nothing";
    Serial.println("Stopped Turning");
    stopCommand = "nothing";
  }
  
  else if (stopCommand == "Stop Nodding"){
    nodCommand = "nothing";
    Serial.println("Stopped Nodding");
    stopCommand = "nothing";
  }
}

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
        Serial.println("head");
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
