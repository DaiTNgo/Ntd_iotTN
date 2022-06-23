#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <WiFi.h>
#include <Wire.h>

#define RX 16
#define TX 17

#define WIFI_SSID "Phan Trung My"
#define WIFI_PASSWORD "051119999"
#define DATABASE_URL "http://doantn-iot.herokuapp.com/api"

bool stringComplete = false;
String inputString = "";

void UART();

void setup()
{
  Serial.begin(9600);
  Wire.begin();

  //  Serial2.begin(baud-rate, protocol, RX pin, TX pin);
  Serial2.begin(115200, SERIAL_8N1, RX, TX);
  Serial.println("Serial Txd is on pin: " + String(TX));
  Serial.println("Serial Rxd is on pin: " + String(RX));

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}

void loop()
{
  UART();
}

void UART()
{
  // put your main code here, to run repeatedly:
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
      //      JSONVar myObject = JSON.parse(inputString);
      Serial.println(inputString);
      if (WiFi.status() == WL_CONNECTED)
      {
        WiFiClient client;
        HTTPClient http;

        http.begin(client, DATABASE_URL);
        http.addHeader("Content-Type", "application/json");
        //                String httpRequestData = JSON.stringify(myObject);
        int httpResponseCode = http.POST(inputString);
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        http.end();
      }
      else
      {
        Serial.println("WiFi Disconnected");
      }

      inputString = "";
      stringComplete = false;
    }
  }
}
