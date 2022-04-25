/*
  ACS712 Current Sensor
  modified on 06 Sep 2020
  by Mohammad Reza Akbari @ Electropeak
  Home
*/


int analogPin = A0; // Current sensor output

const int averageValue = 500;
long int sensorValue = 0;  // variable to store the sensor value read

float voltage = 0;
float current = 0;

void setup() {
  Serial.begin(9600);           //  setup serial
}

void loop() {

  for (int i = 0; i < averageValue; i++)
  {
    sensorValue += analogRead(analogPin);

    // wait 2 milliseconds before the next loop
    delay(2);
  }

  sensorValue = sensorValue / averageValue;
  voltage = sensorValue * 5.0 / 1024.0;
  current = (voltage - 2.5) / 0.185;

  Serial.print("ADC Value: ");
  Serial.print(sensorValue);

  Serial.print("   ADC Voltage: ");
  Serial.print(voltage);
  Serial.print("V");

  Serial.print("   Current: ");
  Serial.print(current);
  Serial.println("A");
}
