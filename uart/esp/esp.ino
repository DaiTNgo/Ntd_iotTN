#include <Arduino_JSON.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <Wire.h>

#define RX 16
#define TX 17

#define WIFI_SSID "Phan Trung My"
#define WIFI_PASSWORD "051119999"
#define DATABASE_URL "http://doantn-iot.herokuapp.com/api"

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
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");

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
      Serial.print("JSON.typeof(myObject) = ");
      Serial.println(JSON.typeof(myObject));
      Serial.print("chuỗi DATA Là:");
      Serial.println(myObject);
      if (WiFi.status() == WL_CONNECTED) {
        WiFiClient client;
        HTTPClient http;

        http.begin(client, DATABASE_URL);
        http.addHeader("Content-Type", "application/json");
        String httpRequestData = JSON.stringify(myObject);
        int httpResponseCode = http.POST(httpRequestData);
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        http.end();
      }
      else {
        Serial.println("WiFi Disconnected");
      }
      // JSONVar current = myObject["current"];
      // JSONVar uid = myObject["uid"];
      // JSONVar firstTime = myObject["firstTime"];
      // JSONVar lastTime = myObject["lastTime"];
      //
      // Serial.println(current);
      // Serial.println(uid);
      // Serial.println(firstTime);
      // Serial.println(lastTime);
      inputString = "";
      stringComplete = false;
    }
  }
}
