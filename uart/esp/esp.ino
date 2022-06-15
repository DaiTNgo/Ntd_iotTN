#include <Arduino_JSON.h>
#include <WiFi.h>
#include <Wire.h>

#define RX 16
#define TX 17

unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;

bool stringComplete = false;
String inputString = "";
int lcdColumns = 16;
int lcdRows = 2;

void ReadSensor();
void TestFirebase();

void setup()
{

  Serial.begin(9600);
  Wire.begin();
  //------UART------
  //  Serial2.begin(baud-rate, protocol, RX pin, TX pin);
  Serial2.begin(115200, SERIAL_8N1, RX, TX);
  Serial.println("Serial Txd is on pin: " + String(TX));
  Serial.println("Serial Rxd is on pin: " + String(RX));

}

void loop()
{
  ReadSensor();
}

void ReadSensor() {
  while (Serial2.available())
  {
    char inChar = (char)Serial2.read();
    inputString += inChar;
    if (inChar == '\n')
    {
      stringComplete = true;
    }
    if (stringComplete)
    {
      Serial.println("Chuoi data nhan: ");
      Serial.println(inputString);
      JSONVar myObject = JSON.parse(inputString);
      Serial.print("chuỗi DATA Là:");
      Serial.println(myObject);

      JSONVar current = myObject["current"];
      JSONVar uid = myObject["uid"];
      JSONVar firstTime = myObject["firstTime"];
      JSONVar lastTime = myObject["lastTime"];

      Serial.println(current);
      Serial.println(uid);
      Serial.println(firstTime);
      Serial.println(lastTime);
      inputString = "";
      stringComplete = false;
    }
  }
}
