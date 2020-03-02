# BruinBot controls
Code for sensor interfacing, actuator control, and communications.

## High level architecture
BruinBot has many subsystems, each with their own sensors actuators, controlled by microcontrollers (Arduino boards).

A main microprocessor (Raspberry Pi) coordinates the subsystems by sending commands for actuation as well as for sensor data retrieval.

The folders in this repository are organized by subsystems.