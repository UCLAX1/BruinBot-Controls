#include "SparkFun_ProDriver_TC78H670FTG_Arduino_Library.h" //Click here to get the library: http://librarymanager/All#SparkFun_ProDriver
PRODRIVER nodMotor; // Controls up and down movement
PRODRIVER swivelMotor;  // Controls swiveling movement
String command = "nothing";
String swivelCommand = "nothing";
String nodCommand = "nothing";
String stopCommand = "nothing";

void setup() {
   Serial.begin(115200);
   Serial.println("Waiting for command");

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

  // Set motor step resolution for both motors
  nodMotor.settings.stepResolutionMode = PRODRIVER_STEP_RESOLUTION_FIXED_1_16; // 1/16 step
  swivelMotor.settings.stepResolutionMode = PRODRIVER_STEP_RESOLUTION_FIXED_1_16; 

   nodMotor.begin();
   swivelMotor.begin();
}

void loop() {
  if (Serial.available() > 0){
    command = Serial.readString();
    command.remove(command.length()-1);
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
  }
  



  if (swivelCommand == "Turn Right"){
    swivelMotor.step(50, 1);
    Serial.println("Turning Right");
  }
  
  if (swivelCommand == "Turn Left"){
    swivelMotor.step(50, 0);
    Serial.println("Turning Left");
  }

  if (nodCommand == "Nod Up"){
    nodMotor.step(50, 0);
    Serial.println("Nodding Up");
  }

  if (nodCommand == "Nod Down"){
    nodMotor.step(50, 1);
    Serial.println("Nodding Down");
  }

  if (stopCommand == "Stop Turning"){
    swivelCommand = "nothing";
    Serial.println("Stopped Turning");
    stopCommand = "nothing";
  }
  if (stopCommand == "Stop Nodding"){
    nodCommand = "nothing";
    Serial.println("Stopped Nodding");
    stopCommand = "nothing";
  }
}
