# General sensors
Code for sensors that monitor the general status of the robot. Includes GPS, IMU, and ultrasonic proximity sensors.

## GPS_ACC
Arduino code for reading data from [SparkFun IMU module](https://www.sparkfun.com/products/13339) using the [LSM6DS3 library](https://github.com/sparkfun/SparkFun_LSM6DS3_Arduino_Library) and reading data from [SparkFun GPS breakout](https://www.sparkfun.com/products/15210) using the [SparkFun Ublox library](https://github.com/sparkfun/SparkFun_Ublox_Arduino_Library).

Communicates to the main controller using USB serial.

When a command is received, the data is read from the requested module and sent to the main controller.
