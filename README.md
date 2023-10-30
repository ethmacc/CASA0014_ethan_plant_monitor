# CASA0014_ethan_plant_monitor

## Overview
This repository contains the arduino code for Ethan's plant monitor for the CASA0014 module. It builds upon the standard workshop materials (as seen in this repository: https://github.com/ucl-casa-ce/casa0014/tree/main/plantMonitor#overview), with a few improvements and customizations.

The goal was to build an IoT device (based on the Adafruit Feather Huzzah) capable of sensing soil moisture, air humidity and temperature of a potted plant and store historic data in a timeseries database, as well as express and communicate this data in a novel and engaging way.

## Dependencies
The main code for this project resides in the plant monitor v1 folder. The following dependencies must be installed for the script to be compiled and uploaded successfully:
- Adafruit Unified Sensor - essential for compiling and pushing code from the Arduino IDE to the Feather Huzzah
- PubSubClient by Nick O'Leary - for constructing the mqtt client to publish data to the MQTT server
- DHT sensor library by Adafruit - for the DHT22 temperature and humidity sensor
- ezTime by Rop Gonggrijp - for generating timestamps for each datapoint

## Hardware
You will need the following hardware:
- An Adafruit Feather Huzzah (or similar ESP8266 based-board)
- Resistors, 2 x 10K Ohm and one each of 100 Ohm and 220 Ohm
- DHT22 sensor
- Two nails
- Two short lengths of wire

## Method
### Initial setup
### Physical wiring
### Functions in the code
### What's next?
