//include the ESP8266WiFi library and PubSubClient library for MQTT connection
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Import secret variables from arduino_secrets
#include "arduino_secrets.h"

const char* ssid = SECRET_SSID;
const char* password = SECRET_PASS;
const char* mqttuser = SECRET_MQTTUSER;
const char* mqttpass = SECRET_MQTTPASS;

// set MQTT server
const char* mqtt_server = "mqtt.cetools.org";

// Instantiate WiFi and MQTT client
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
  // Set up builtin LED
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, HIGH);

  Serial.begin(115200);
  delay(100);

  startWifi();

  client.setServer(mqtt_server, 1884);
  client.setCallback(callback);
}

void loop() {
  delay(5000);
  sendMQTT();
}

// Function to connect to WiFi and show connection status
void startWifi(){
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
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

void sendMQTT() {

  // If not connected to MQTT server, attempt to connect
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  ++value;
  // Print hello world messages iteratively and publish them to the server
  snprintf (msg, 50, "hello world #%ld", value);
  Serial.print("Publish message: ");
  Serial.println(msg);
  client.publish("student/CASA0014/plant/uclqlel", msg);
}
