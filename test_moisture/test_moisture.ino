uint8_t soilPin = 0; // one nail goes to +5V, the other nail goes to this analogue pin
int moisture_val;

int sensorVCC = 13; // Control voltage with pin D13
int counter = 0;

void setup() {
  Serial.begin(115200); //open serial port
  pinMode(sensorVCC, OUTPUT); //initialise pin for output
  digitalWrite(sensorVCC, LOW);
}

void loop() {
  counter++; //iterative counter
  if (counter > 1) { // change this value to set "not powered" time
    // power the sensor
    digitalWrite(sensorVCC, HIGH);
    delay(1000);
    // read the value from the sensor:
    moisture_val = analogRead(soilPin); // read the resistance
    //stop power (to prevent electrolysis)
    digitalWrite(sensorVCC, LOW);
    delay(100);
    counter = 0;
  }
  //wait
  Serial.print("sensor = ");
  Serial.println(moisture_val);
  delay(100);
}
