#include <Wire.h> //Needed for I2C to GPS
#include "SparkFunLSM6DS3.h"
#include "SparkFun_Ublox_Arduino_Library.h" //http://librarymanager/All#SparkFun_Ublox_GPS
LSM6DS3 myIMU; //Default constructor is I2C, addr 0x6B
SFE_UBLOX_GPS myGPS;

#define BAUD_RATE 9600
#define LED_PIN 13

const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];        // temporary array for use when parsing
char receivedMsg[numChars] = {0};
String command = "";
boolean newData = false;

void setup()
{
    pinMode(LED_PIN, OUTPUT);

    Wire.begin();
    if (myGPS.begin() == false) //Connect to the Ublox module using Wire port
    {
    Serial.println(F("Ublox GPS not detected at default I2C address. Please check wiring. Freezing."));
    while (1);
    }

    myGPS.setI2COutput(COM_TYPE_UBX); //Set the I2C port to output UBX only (turn off NMEA noise)
    myGPS.saveConfiguration(); //Save the current settings to flash and BBR
    //Call .begin() to configure the IMU
    myIMU.begin();

    Serial.begin(BAUD_RATE);
    while (!Serial) {}; //Wait for user to open terminal

    digitalWrite(LED_PIN, HIGH);
    while (! establish_connection()) {}
    digitalWrite(LED_PIN, LOW);
  
}

void loop()
{
    recvWithStartEndMarkers();
    if (newData == true) {
        strcpy(tempChars, receivedChars);
            // this temporary copy is necessary to protect the original data
            //   because strtok() used in parseData() replaces the commas with \0
        parseData();
        command = String(receivedMsg);
        newData = false;
        if (command == "GPS") {
          returnGPS();
        } 
        else if (command == "ACC") {
          returnACC();
        }
    }
}

void returnGPS()
{
    String s = "GPS:";
    long latitude = myGPS.getLatitude();
    s += String(latitude);
    s += ",";
    long longitude = myGPS.getLongitude();
    s += String(longitude);
    s += ",";
    long altitude = myGPS.getAltitude();
    s += String(altitude);
    s += ",";
    int SIV = myGPS.getSIV();
    s += String(SIV);
    Serial.println(s);
}

void returnACC()
{
    String s = "ACC:";
    s += String(myIMU.readFloatAccelX());
    s += ",";
    s += myIMU.readFloatAccelY();
    s += ",";
    s += myIMU.readFloatAccelZ();
    Serial.println(s);
}

boolean establish_connection(){
    while (!(Serial.available() > 0)) {} // hangs until a msg is received
    char rcv = Serial.read();
    if (rcv == 'i') {
        Serial.println("sensors");
    }

    int start = millis();
    while (!(Serial.available() > 0)) {
        if (millis()-start > 1000) {
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

void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;

    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }
        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

void parseData() {      // split the data into its parts

    char * strtokIndx; // this is used by strtok() as an index

    strtokIndx = strtok(tempChars,",");      // get the first part - the string
    strcpy(receivedMsg, strtokIndx); // copy it to messageFromPC
}
