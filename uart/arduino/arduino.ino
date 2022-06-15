#include <Arduino_JSON.h>

unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;

bool stringComplete = false;
String inputString = "";

void ReadSensor();

void setup()
{

  Serial.begin(9600);
  Serial2.begin(115200);
}

void loop()
{
  String sData = "{\"current\":\"" + String(7) + "\","
                 + "\"uid\":\"" + String(77) + "\","
                 + "\"lastTime\":\"" + String(8) + "\","
                 + "\"firstTime\":\"" + String(88) + "\"}";

  Serial2.println(sData);
  Serial2.flush();
  Serial.println(sData);
  delay(1000);
}
