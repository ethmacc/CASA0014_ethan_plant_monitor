#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ezTime.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTTYPE DHT22 // specify DHT model number

// DHT Sensor setup
uint8_t DHTPin = 12; // on Pin 12 of the Huzzah
DHT dht(DHTPin, DHTTYPE); // Initialize DHT sensor
float Temperature;
float Humidity;

// soil moisture sensor setup
uint8_t soilPin = 0; // one nail goes to +5V, the other nail goes to this analogue pin
int sensorVCC = 13; // Control voltage with pin D13
int Moisture = 1; // initial value just in case web page is loaded

// Wifi and MQTT
#include "arduino_secrets.h" // enter secret wifi ssid and passwd in this file

const char* ssid = SECRET_SSID2; // set ssid for lab and ssid2 for home
const char* password = SECRET_PASS2;
const char* mqttuser = SECRET_MQTTUSER;
const char* mqttpass = SECRET_MQTTPASS;

ESP8266WebServer server(80);
const char* mqtt_server = "mqtt.cetools.org";
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

// Date and time
Timezone GB;

void setup() {
  // Set up LED to be controllable via broker
  // Initialize the BUILTIN_LED pin as an output
  // Turn the LED off by aking the voltage HIGH
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, HIGH);

  // Set up the outputs to control the soil sensor switch
  pinMode(sensorVCC, OUTPUT);
  digitalWrite(sensorVCC, LOW);

  Serial.begin(115200); //open serial port
  delay(100);

  // start DHT sensor
  pinMode(DHTPin, INPUT);
  dht.begin();

  // run initialization functions
  startWifi();
  startWebserver();
  syncDate();

  // start MQTT server
  client.setServer(mqtt_server, 1884);
  client.setCallback(callback);
}

void loop() {
  // handler for receiving requests to webserver
  server.handleClient();

  if (minuteChanged()) {
    readMoisture();
    sendMQTT();
    Serial.println(GB.dateTime("H:i:s")); // UTC.dateTime("l, d-M-y H:i:s.v T")
  }

  client.loop();
}

void readMoisture(){ // change this value to set "not powered" time
  // power the sensor
  digitalWrite(sensorVCC, HIGH);
  delay(1000);
  // read the value from the sensor:
  Moisture = analogRead(soilPin); // read the resistance
  //stop power (to prevent electrolysis)
  digitalWrite(sensorVCC, LOW);
  delay(100);
  // Serial.print("Wet ");
  // Serial.println(Moisture); // read the value from the nails
}

void startWifi() {
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  // check to see if connected and wait until you are
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void syncDate() {
  // get real date and time
  waitForSync();
  Serial.println("UTC: " + UTC.dateTime());
  GB.setLocation("Europe/London");
  Serial.println("London time: " + GB.dateTime());
}

void startWebserver() {
  // when connected and IP address obtained start HTTP server
  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void sendMQTT() {

  // If not connected to MQTT server, attempt to connect
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  Temperature = dht.readTemperature(); // Get temperature val
  snprintf(msg, 50, "%.1f", Temperature);
  Serial.print("Publish message for t: ");
  Serial.println(msg);
  client.publish("student/CASA0014/plant/uclqlel/temperature", msg);

  Humidity = dht.readHumidity(); // Get hum val
  snprintf(msg, 50, "%.0f", Humidity);
  Serial.print("Publish message for h: ");
  Serial.println(msg);
  client.publish("student/CASA0014/plant/uclqlel/humidity", msg);

  // Moisture  already read by read moisture function
  snprintf(msg, 50, "%.0i", Moisture);
  Serial.print("Publish message for m: ");
  Serial.println(msg);
  client.publish("student/CASA0014/plant/uclqlel/moisture", msg);
}

// Receive message on the Feather and turn LED on/off depending on payload
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  //Switch on the LED if an l was received as the first char
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW); // LOW means LED on for ESP
  } else {
    digitalWrite(BUILTIN_LED, HIGH); // turn LED off
  }
}

// Function to reconnect to MQTT server
void reconnect() {
  // Loop until reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);

    //Attempt to connect
    if(client.connect(clientId.c_str(), mqttuser, mqttpass)) {
      Serial.println("connected");
      // ... subscribe to msgs on the broker
      client.subscribe("student/CASA0014/plant/uclqlel/inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      //Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void handle_OnConnect() {
  Temperature = dht.readTemperature(); // Get temperature value
  Humidity = dht.readHumidity(); // Get humidity value
  server.send(200, "text/html", SendHTML(Temperature, Humidity, Moisture));
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not Found");
}

String SendHTML(float Temperaturestat, float Humiditystat, int Moisturestat) {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>ESP8266 DHT22 Report</title>\n";
  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align center;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
  ptr += "p {font-size: 24px;color: #444444;margin-bottom: 10px;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<div id=\"webpage\">\n";
  ptr += "<h1>ESP8266 Huzzah DHT22 Report</h1>\n";

  ptr += "<p>Temperature: ";
  ptr += (int)Temperaturestat;
  ptr += " C</p>";
  ptr += "<p>Humidity: ";
  ptr += (int)Humiditystat;
  ptr += "%</p>";
  ptr += "<p>Moisture: ";
  ptr += Moisturestat;
  ptr += "</p>";
  ptr += "<p>Sampled on: ";
  ptr += GB.dateTime("l,");
  ptr += "<br>";
  ptr += GB.dateTime("d-M-y H:i:s T");
  ptr += "</p>";

  ptr += "</div>\n";
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}