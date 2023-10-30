# CASA0014_ethan_plant_monitor

## Overview
This repository contains the arduino code for Ethan's plant monitor for the CASA0014 module. It builds upon the standard workshop materials (as seen in the module repository: https://github.com/ucl-casa-ce/casa0014/tree/main/plantMonitor#overview), with a few improvements and customizations.

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

And, of course, a plant to test the device with! Note that the author attempted to test this plant monitor on a cactus, but the soil was found to be too dry to read any significant value! A plant species that requires more moist soil is therefore recommended.

## Method
### Initial setup
The main code in plant monitor v1 can be pretty much run as is, however, you will need to write a file named ```arduino_secrets.h```, the contents of which should follow the following format:

```
#define SECRET_SSID "" //add your wifi ssid here in string form within the inverted commas
#define SECRET_PASS "" //add your wifi password here in string form within the inverted commas

#define SECRET_SSID2 "" //add another wifi ssid here if you have another network
#define SECRET_PASS2 "" //add 2nd wifi password

#define SECRET_MQTTUSER "" //add username for logging on to your mqtt server
#define SECRET_MQTTPASS "" // add passsword for logging on to your mqtt server
```
Ensure that this file is saved in the plant_monitor_v1 folder next to the main script
The gitignore file in this repo should prevent this secret file from getting uploaded into the public domain, but do check before proceeding!

You may also wish to change the ```syncDate()``` function at line 111, to reflect your local timezone

### Physical wiring
### Functions in the code
### What's next?
