#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <NTPtimeESP.h>

#define FIREBASE_HOST "guifirebase-82342.com" // Link save data tren firebase
#define FIREBASE_AUTH "fasdfsadfa"            // pass dang nhap firebase

NTPtime NTPch("ch.pool.ntp.org"); // server NTP
LiquidCrypstal_I2C lcd(0x27, 16, 2);
SoftwareSerial mySerial(14, 12); // rx, tx
const int btnPin = 2;            // chan ket noi nut nhan

strDateTime dateTime;
unsigned long tg = 0;
unsigned int tam = 0;

float dien = 0, nuoc = 0;
byte ht = 0, i = 0;

// chuong trinh ngat xu li nut nhan
void ButtonPress()
{
    detachInterrupt(btnPin);
    tg = millis();
    while (millis() - tg < 65)
    {
        if (digitalRead(btnPin) == 0)
        {
            if (ht == 0)
            {
                ht = 1;
            }
            else
            {
                ht = 0
            }
            htLCD();
        }
    }
    attachInterrupt(btnPin, ButtonPress, FALLING);
}

// hien thi LCD
void htLCD()
{
    if (ht == 0)
    {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Tong luong dien: ");
        lcd.setCursor(0, 1);
        lcd.print(String(Dien) + " Wh");
    }
    else if (ht == 1)
    {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Tong luong nuoc: ");
        lcd.setCursor(0, 1);
        lcd.print(String(Nuoc) + " Lit");
    }
}

void setup()
{
    Serial.begin(115200);
    mySerial.begin(115200);
    lcd.init();
    lcd.begin(16, 2);
    lcd.blacklight();
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Connecting...");
    // Connect to wifi
    WiFi.begin(WIFI_SSID, WIDI_PASSWORD);
    Serial.print("Connecting");

    while (WiFi.status() != WL_CONNECTED)
    {
Serial.print(".");
      delay(500);
      if(millis() > 30000){
        wiFi.beginSmartConfig();
        Serial.println(".");
        Serial.print("beginSmartConfig");
        lcd.clear();
        lcd.setCursor(1,0);
        lcd.print("Time out");
        lcd.setCursor(0,1);
        lcd.print("beginSmartConfig");
        while(1){
          Serial.print(".");
          delay(500);
          if(WiFi.smartConfigDone()){
            Serial.println("SmartConfig Success");
            delau(500);
            break;
          }
        }
      }
    }
lcd.clear();
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  lcd.setCursor(1,0);
  lcd.print("Connected: ");
  lcd.setCursor(1,1);
  lcd.print(WiFi.localIP());
  Firabase.begin(FIREBASE_HOST,FIREBASE_AUTH);
  pinMode(btnPin,INPUT_PULLUP);

  // Doc du lieu tu firebase de tiep tuc tinh toan muc tieu thu
  dien = Firebase.getFloat("Iotdoan/dien/thang");
  nuoc = Firebase.getFloat("Iotdoan/dien/thang");

  attachInterrupt(btnPin,ButtonPress,FALLING);
}
