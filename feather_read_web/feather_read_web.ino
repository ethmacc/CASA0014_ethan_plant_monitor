//include the ESP8266WiFi library
#include <ESP8266WiFi.h>
#include <ezTime.h>

Timezone GB;

const char* ssid = "CE-Hub-Student";
const char* password = "casa-ce-gagarin-public-service";
const char* host = "iot.io";

void setup() {
  Serial.begin(115200);
  delay(100);

  //Start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //Print message to show when connected
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  delay(5000);

  Serial.println("-------------------------------");
  Serial.print("Connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  // We now create a URI for the request
  String url = "/data/index.html";
  Serial.print("Requesting URL: ");
  Serial.println(host + url);

  // This will send the request to the server
  client.print(
    String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" +
    "Connection: close\r\n\r\n");

  delay(500);

  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

  Serial.println();
  Serial.println("closing connection");
}
