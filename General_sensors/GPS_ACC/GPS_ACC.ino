#include <Wire.h> //Needed for I2C to GPS
#include "SparkFunLSM6DS3.h"
#include "SparkFun_Ublox_Arduino_Library.h" //http://librarymanager/All#SparkFun_Ublox_GPS
LSM6DS3 myIMU; //Default constructor is I2C, addr 0x6B
SFE_UBLOX_GPS myGPS;
long lastGPSTime = 0; //Simple local timer. Limits amount if I2C traffic to Ublox module.
long lastAccTime = 0;
char received_byte; // 1 -> GPS, 2->Acc
void getGps()
{
  Serial.print("GPS:");
  long latitude = myGPS.getLatitude();
  Serial.print(latitude);
  Serial.print(",");
  long longitude = myGPS.getLongitude();
  Serial.print(longitude);
  Serial.print(",");
  long altitude = myGPS.getAltitude();
  Serial.print(altitude);
  Serial.print(",");
  byte SIV = myGPS.getSIV();
  Serial.println(SIV);
}
void getAcc()
{
  Serial.print("ACC:");
  Serial.print(myIMU.readFloatAccelX(), 4);
  Serial.print(",");
  Serial.print(myIMU.readFloatAccelY(), 4);
  Serial.print(",");
  Serial.println(myIMU.readFloatAccelZ(), 4);
}
/*
void getGps()
{   
    long latitude = myGPS.getLatitude();
    Serial.print(F("Lat: "));
    Serial.print(latitude);
    long longitude = myGPS.getLongitude();
    Serial.print(F(" Long: "));
    Serial.print(longitude);
    Serial.print(F(" (degrees * 10^-7)"));
    long altitude = myGPS.getAltitude();
    Serial.print(F(" Alt: "));
    Serial.print(altitude);
    Serial.print(F(" (mm)"));
    byte SIV = myGPS.getSIV();
    Serial.print(F(" SIV: "));
    Serial.println(SIV);
}
void getAcc()
{
    Serial.print("Accelerometer: ");
    Serial.print(" X = ");
    Serial.print(myIMU.readFloatAccelX(), 4);
    Serial.print(" Y = ");
    Serial.print(myIMU.readFloatAccelY(), 4);
    Serial.print(" Z = ");
    Serial.println(myIMU.readFloatAccelZ(), 4);
}
*/
void setup()
{
  Serial.begin(9600);
  while (!Serial) {}; //Wait for user to open terminal
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
}
void loop()
{
  //Query module only every second. Doing it more often will just cause I2C traffic.
  //The module only responds when a new position is available
  while (!(Serial.available() > 0)) {}
  received_byte = Serial.read();
  if (received_byte == '1') {
    getGps();
  } else if (received_byte == '2') {
    getAcc();
  } else {
    Serial.print("\nUnsupported Byte!\n");
  }
}