# CASA0014_ethan_plant_monitor

## Overview
<img src="https://github.com/ethmacc/CASA0014_ethan_plant_monitor/assets/60006290/f24ebbc4-d1fe-468c-a3ec-134aeb68d31e" width="1000" />

This repository contains the arduino code for Ethan's plant monitor for the CASA0014 module. It builds upon the standard workshop materials (as seen in the module repository: https://github.com/ucl-casa-ce/casa0014/tree/main/plantMonitor#overview), with a few improvements and customizations.

The goal was to build an IoT device (based on the Adafruit Feather Huzzah) capable of sensing soil moisture, air humidity and temperature of a potted plant over regular intervals and store this data in a timeseries database, as well as express and communicate this data in a novel and engaging way.

Overall though, like all plant monitor projects, I suppose the more intangible aspiration is to help us care for our leafy friends through a data-driven understanding of their optimal environments and watering schedules.

## Dependencies
The main code for this project resides in the plant monitor folder. The following dependencies must be installed for the script to be compiled and uploaded successfully:
- **PubSubClient** by Nick O'Leary - for constructing the mqtt client to publish data to the MQTT server
- **DHT sensor library** by Adafruit - for the DHT22 temperature and humidity sensor
- **ezTime** by Rop Gonggrijp - for generating timestamps for each datapoint
- **ESP8266WifiMulti.h** - for connecting to WiFi networks

Your Arduino IDE should also have the **ESP8266 board package** installed, so as to be able to compile and push sketches to the Feather Huzzah (further detail can be found on Adafruit's website: https://learn.adafruit.com/adafruit-feather-huzzah-esp8266/using-arduino-ide) and access WiFi networks with your ESP8266

## Hardware
You will need the following hardware:
- An Adafruit Feather Huzzah (or similar ESP8266 based-board)
- Resistors, 2 x 10K Ohm and one each of 100 Ohm and 220 Ohm
- DHT22 sensor
- Two nails
- Two short lengths of wire
- Breadboard and connectors for prototyping

And, of course, a plant to test the device with! Note that the author attempted to test this plant monitor on a cactus, but the soil was found to be too dry to read any significant value! A plant species that requires more moist soil is therefore recommended.

<img src="https://github.com/ethmacc/CASA0014_ethan_plant_monitor/assets/60006290/048f29ca-0df1-4a15-9e11-2684abd09f2f" width="400" />

*Not a good idea to use this type of plant!*

## Method

### Physical wiring
The circuit I have used is a variation on the classic arduino soil moisture sensor (the primary one I've looked at is https://www.instructables.com/Moisture-Detection-With-Two-Nails/ by ronnietucker). The basic principle behind this is that it measures resistance between two nail electrodes in the soil, and since moist soil contains water, which conducts electricity, the resistance is lowered when the plant has been recently watered. The version used here makes use of the Feather Huzzah to control when the nail sensor is switched on to take readings (every 1 minute) to prevent electrolysis, which happens whenever current flows through the nails.

The diagram for the circuit is shown below (taken from the CASA0014 workshop notes):
<img width="1000" alt="14e0883730a7e435" src="https://github.com/ethmacc/CASA0014_ethan_plant_monitor/assets/60006290/1ae18925-a535-420e-a78b-a5cd9a122ca1">

Unlike Ronnie Tucker's instructable, multiple resistors are used here:

- One is connected to the DHT22
- While the other three are connected in series as a voltage divider sub-circuit (https://learn.sparkfun.com/tutorials/voltage-dividers/all). This reduces the voltage flowing through the nail sensor circuit before it is returned to the ADC pin to create an analogue reading. This is essential as the Huzzah reads analogue readings up to 1V only.


### Initial setup
The main code in plant monitor folder can be pretty much uploaded to your Feather Huzzah and run as is, however, you will need to write a file named ```arduino_secrets.h```, the contents of which should follow the following format:

```
#define SECRET_SSID "" //add your wifi ssid here in string form within the inverted commas
#define SECRET_PASS "" //add your wifi password here in string form within the inverted commas

#define SECRET_SSID2 "" //add another wifi ssid here if you have another network
#define SECRET_PASS2 "" //add 2nd wifi password

#define SECRET_MQTTUSER "" //add username for logging on to your mqtt server
#define SECRET_MQTTPASS "" // add passsword for logging on to your mqtt server
```
Ensure that this file is saved in the plant_monitor_v1 folder next to the main script.

Example code has been adapted from the ESP8266WifiMulti.h library (https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/examples/WiFiMulti/WiFiMulti.ino) to manage more than one WiFi network - which can be handy if you are moving the plant monitor from one location to another.

NOTE: The gitignore file in this repo should prevent this secret file from getting uploaded into the public domain, but do check before proceeding!

**You should also change the ```syncDate()``` function, to reflect your local timezone, the ```mqtt_server``` variable to the address of your MQTT server and the ```client.publish()``` function inputs to reflect your unique MQTT server and topic structure**

### Unit tests
You may wish to test specific functions of the Feather Huzzah, such as its ability to connect to a WiFi network or MQTT server, before running the main project sketch. For this purpose, a series of test sketches have been included here in this repository:

- MQTT connect - tests the Feather's ability to connect an publish to an MQTT server
- feather read time - tests the Feather's ability to use the ezTime library to generate local timestamps
- feather read web - tests the Feather's ability to connect to a WiFi network and read off the HTML code for a webpage
- test moisture - tests the nail soil moisture sensor setup (your physical plant monitor circuit should be prepared before attempting this test)

### Physical tests
It is a worthy exercise to test all the physical sensors to ensure they are working as expected:

- For the nail soil moisture sensor - put the two nails together to create a connected circuit with the lowest possible resistance. Together with the test moisture script, you should see that this produces a value of about 1000 (it was 1200 for my set up). Then leave the nails alone, but out of the soil and not touching each other. This should ne the maximum possible resistance and you should get a very low reading (12 for my setup).
- For the DHT22 temperature sensor - try moving it outdoors and see if there is any significant change. In the warmer months this may be a bit harder to test, perhaps consider putting it in the refrigerator for a short while?
- For the DHT22 humidity sensor - I've found that putting a palm around the sensor is already enough to cause an increase in the measured humidity value -unless you have very dry hands!

## Data streaming and visualisation
With the physical plant monitor set-up, it's now time to take a look at where the data is going, and different options for storing and visualising your plant data.

### Browsing realtime data with MQTT explorer
The first thing to check before getting on to storing the data in a databse is that the sensor readings are in fact being published to the MQTT server. This can be done by downloading and installing MQTT Explorer (http://mqtt-explorer.com/) which is a MQTT client with a graphical interface. It allows you to browse data on your MQTT server and what is being published to it, by topic, in real time:

<img width="1280" alt="image" src="https://github.com/ethmacc/CASA0014_ethan_plant_monitor/assets/60006290/dcd8997b-5833-4eb4-a752-eeaebb9eeeb5">

**Bear in mind you may have to wait for up to 1 minute for the data to be published** (as this is what is set in the arduino code). If you cannot see your data being published, you will need to check that the Arduino is connecting to both the WiFi network and MQTT server successfully and is publishing data to the correct topic.

### Storing historic data

#### Raspberry Pi as a gateway
If you have a WiFi-capable Raspberry Pi (Pi3 B or later) you can configure it to act as a gateway to read and store data from the MQTT server in an influxdb database. For this project, a Pi 4 was used with a headless setup (i.e. no monitor, accessed via the command line and SSH). Instructions on how to set up your Raspberry Pi can be found on here: https://www.tomshardware.com/reviews/raspberry-pi-headless-setup-how-to,6028.html

![IMG_6585](https://github.com/ethmacc/CASA0014_ethan_plant_monitor/assets/60006290/47d3460e-caa5-4541-9b9c-e4030b9ad62b)

#### Influxdb timeseries database
Once you have your Raspberry Pi set up and you are logged in, you will need to install influxdb, which is the timeseries database that I have used. The instructions to do this can be found here: https://pimylifeup.com/raspberry-pi-influxdb/. 

After this is done, you can then login to influxdb by entering ```<your-hostname-here>:8086``` into a web browser, creating a new account and signing in. You will then need to install the Raspberry Pi template in the templates section and create a new bucket in the Load Data section to collect your plant data from the MQTT server.

#### Telegraf setup
We need a means of collecting timeseries data and streaming this into influxdb. I did this with telegraf, which can be installed with the following in you Raspberry Pi's command line terminal:

```sudo apt-get update && sudo apt-get install telegraf -y```

You will then need to define the PATH variables, as well as the API keys to allow telegraf to communicate with influxdb. I did this by running the following commands in the terminal:

```
export INFLUX_HOST=<your_raspberrypi_ip_address:8086>
export INFLUX_ORG=<your_organisation>
```
Following this, edit the telegraf configuration file to include your API keys (generated from the influxdb page) and define input and output plugins for MQTT. Detailed information on how to do this and the syntax to use can be found in influxdb's own documentation: https://docs.influxdata.com/telegraf/v1/configuration/

Don't forget to restart influxdb and telegraf for the changes to take effect:
```
sudo systemctl stop telegraf
sudo systemctl stop influxdb
sudo systemctl start influxdb
sudo systemctl start telegraf
```

### Viewing your timeseries data
All going well, by clicking onthe data explorer tab, you should now be able to navigate by tag to display your timeseries data:

<img width="1000" alt="image" src="https://github.com/ethmacc/CASA0014_ethan_plant_monitor/assets/60006290/9d398dbe-5b10-4522-89a5-73dcaec6d8ab">

## Data visualisation with Grafana
Should you wish to display your monitored data in a more aesthetically pleasing format, Grafana may be another option. You can get to this by entering ```<your-hostname>:3000``` in a web browser, creating a new account and signing in.

Create a new panel using the menu on your left and go to the query input box at the bottom.

You can either write your own query or copy the query from influxdb's data explorer to Grafana:

```
from(bucket: "mqtt-data")
  |> range(start: v.timeRangeStart, stop: v.timeRangeStop)
  |> filter(fn: (r) => r["_measurement"] == "mqtt_consumer")
  |> filter(fn: (r) => r["_field"] == "value")
  |> filter(fn: (r) => r["host"] == "stud-pi-uclqlel")
  |> filter(fn: (r) => r["plant-topics"] == "student/CASA0014/plant/uclqlel/moisture" or r["plant-topics"] == "student/CASA0014/plant/uclqlel/humidity" or r["plant-topics"] == "student/CASA0014/plant/uclqlel/temperature")
  |> aggregateWindow(every: v.windowPeriod, fn: mean, createEmpty: false)
  |> yield(name: "mean")
```
There are also various options for different types of data display, such as dials. I have configured mine as below, to show both point readings in time, as well as the data over a period of time: 

<img width="1000" alt="Screenshot 2023-11-06 172912" src="https://github.com/ethmacc/CASA0014_ethan_plant_monitor/assets/60006290/d08773e8-d372-4b46-a721-c7a2467dd369">

I tried to choose colour schemes and graphics appropriate to each data type, e.g. for soil moisture, I used a blue-yellow colormap, such that the timeseries graph and the dial would go from blue when the soil was freshly watered to yellow as the plant used up more of the moisture in the soil.

Do note that leaving the setup for a couple of days, it appears that there is some latency/lag whenever grafana is restarted. If you're not getting data displayed on your dashboard immediately - don't panic, wait a couple of minutes and refresh the page.

### Results
As expected for the controlled environment of the lab, temperature and relative humidity did fluctuate over the week I had the plant monitor on, but the trend was relatively stable overall. More interestingly, the soil moisture level started out high just after I had watered the plant (about 95), but it slowly dropped in an almost exponential curve, petering out at about 22. Decrease in soil moisture was extremely rapid over the first few days, but slowed down significantly towards the end of the week. Not being a botanist, I suspect this could be for a couple of reasons:
1. The plant, a succulent, had already absorbed enough water into its reserves, like a cactus, so it slowed down its water intake
2. The soil moisture value of around 20 represents dry soil that has already been largely depleted of water by the plant.

## Unity
I followed up the Grafana dashboard with a simple Unity application that changes the terrain and skybox textures whenever the plant is in distress:

<img width="726" alt="Screenshot 2023-11-06 180635" src="https://github.com/ethmacc/CASA0014_ethan_plant_monitor/assets/60006290/224356a7-ad5c-4467-8713-0f87c9e6d816">

_The landscape is green when the plant is watered, healthy and warm_

<img width="726" alt="Screenshot 2023-11-06 180622" src="https://github.com/ethmacc/CASA0014_ethan_plant_monitor/assets/60006290/6b2b3bf1-be01-4088-8695-b18235e0b2cb">

_The landscape is dry and arid when the soil moisture is low_

<img width="726" alt="Screenshot 2023-11-06 181144" src="https://github.com/ethmacc/CASA0014_ethan_plant_monitor/assets/60006290/1e72838b-13b1-4107-a4c4-b62e4cfa907f">

_The landscape is snowy when the plant is cold and the air humidity is high_

https://github.com/ethmacc/CASA0014_ethan_plant_monitor/assets/60006290/67e86437-78f1-440b-ad32-c07ce825c29c

_Demo of the Unity app in action_

This was done by sculpting a simple landscape object in Unity, following this tutorial: https://www.youtube.com/watch?v=ddy12WHqt-M. I then used scripts (```mqttController.cs``` and ```mqttReceiver.cs```) from the CASA0019 workshop extras to pull in data from the MQTT server (https://workshops.cetools.org/codelabs/CASA0019-unity-mqtt/index.html?index=..%2F..index#0) and then linked this to a controller that would change the terrain and skybox textures at runtime using a terrain texture swapping script modified from the Unity forums (```SwapTerrain.cs```, adapted from https://forum.unity.com/threads/changing-terrain-texture-on-runtime.1216140/). The C# code as well as other asset data for this Unity project can be found in the ```plant_monitor_unity``` folder.

The mqtt scripts modify the M2MQTT classes from the M2MQTT library such that they can be used to assign a subscribed value from the MQTT broker to a TextMeshPro object in the Unity GUI. The SwapTerrain script is much less complex, and simply swaps out the ```.diffuseTexture``` attribute of the terrain out for a new assigned texture. With the help of a conditional statement, this can than be used to control the terrain texture using the values received from the MQTT subscription.

<img width="726" alt="Unity screenshot" src="https://github.com/ethmacc/CASA0014_ethan_plant_monitor/assets/60006290/92c74470-1293-4b98-be86-af728e714a26">

_Sculpting the terrain in Unity_

If you wish to make this for yourself, you will need to import the asset folder in the ```plant_monitor_unity``` folder. You will also need to import the following free third-party assets from the Unity store. These have been excluded from this repository for copyright reasons:

- Ancient Ruins and Plants (https://assetstore.unity.com/packages/3d/props/exterior/ancient-ruins-and-plants-20191) by Comeback
- Fantasy Skybox FREE (https://assetstore.unity.com/packages/2d/textures-materials/sky/fantasy-skybox-free-18353) by Render Knight
- Grass Flowers Pack Free (https://assetstore.unity.com/packages/2d/textures-materials/nature/grass-flowers-pack-free-138810) by ALP
- Outdoor Ground Textures (https://assetstore.unity.com/packages/2d/textures-materials/floors/outdoor-ground-textures-12555) by A dog's life software
- Terrain Sample Asset Pack (https://assetstore.unity.com/packages/3d/environments/landscapes/terrain-sample-asset-pack-145808) by Unity

You will also need to import the Asset folder from the M2MQTT Github repo (https://github.com/CE-SDV-Unity/M2MqttUnity/tree/master/Assets)

## Future Implementations
- Smoother transistion between terrain textures/seasons in the Unity app
- More life in the Unity landscapes - maybe animated animals?
- Test the plant monitor with a battery pack. This, together with the multiple WiFi networks defined with the WiFiMulti library, should allow the plant monitor to become a portable setup - monitor your plant on the go!
