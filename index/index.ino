#include <LiquidCrystal_I2C.h>
#include <Arduino_JSON.h>
#include <SPI.h>
#include <MFRC522.h>
#include "ACS712.h"
#include <Wire.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
MFRC522 mfrc522(SS_PIN, RST_PIN);

#define SS_PIN 10 // SDA
#define RST_PIN 9
/**
   @brief
   SCK 52
   MOSI 51
   MISO 50
   3.3 3.3v

*/
int period = 15000;
int thresholdCurrent = 100;

class User
{
public:
    int port;
    unsigned long uid;
    unsigned long firstTime;
    unsigned long lastTime;
    unsigned long breakTime;
    float amp;
    User(int _port, unsigned long _uid, float _current, unsigned long _breakFirstTime, unsigned long _firstTime, unsigned long _lastTime)
    {
        port = _port;
        uid = _uid;
        breakTime = _breakFirstTime;
        firstTime = _firstTime;
        lastTime = _lastTime;
        amp = _current;
    }
};

User user_0(0, 0, 0, 0, 0, 0);
User user_1(1, 0, 0, 0, 0, 0);
User user_2(2, 0, 0, 0, 0, 0);
User user_3(3, 0, 0, 0, 0, 0);
User user_4(4, 0, 0, 0, 0, 0);

User users[5] = {user_0, user_1, user_2, user_3, user_4};

int portPowerOutlet[5] = {23, 25, 27, 29, 31}; // port đọc dòng điện của ổ sạc

ACS712 sensor_0(ACS712_05B, A0);
ACS712 sensor_1(ACS712_05B, A1);
ACS712 sensor_2(ACS712_30A, A2);
ACS712 sensor_3(ACS712_30A, A3);
ACS712 sensor_4(ACS712_30A, A4);

ACS712 sensors[5] = {sensor_0, sensor_1, sensor_2, sensor_3, sensor_4};

const int numberPort = 1;
void setup()
{
    Serial.begin(9600);
    Serial2.begin(115200);
    SPI.begin();
    mfrc522.PCD_Init();
    Serial.println("Prilozhite kartu / Waiting for card...");
    Serial.println("Đảm bảo không có dòng điện đi qua cảm biến trong quá trình cân bằng");
    Serial.println("Calibrating...");
    for (int i = 0; i < numberPort; i++)
    {
        sensors[i].calibrate();
        delay(1000);
    }

    Serial.println("Quá trình cân bằng hoàn tất!!!");

    lcd.init();
    lcd.begin(16, 2);
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Xin chao ^^");
    lcd.setCursor(0, 1);
    lcd.print("Moi ban quet the");
}

void loop()
{
    unsigned long uidDec = 0;
    unsigned long uidDecTemp = 0;

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Xin chao ^^");
    lcd.setCursor(0, 1);
    lcd.print("Moi ban quet the");
    if (!mfrc522.PICC_IsNewCardPresent())
    {
        // đợi thời gian để cắm thiết bị sạc =>15s => tắt sau 15s
        for (int i = 0; i < numberPort; i++)
        {
            if (digitalRead(portPowerOutlet[i]) == 1)
            {
                float tong = 0;
                float I = 0;

                Serial.println("Vào đọc cổng sạc đang mở.");
                for (int x = 0; x < 100; x++)
                {
                    I = sensors[i].getCurrentAC();
                    tong = tong + I;
                }
                float I_current = tong / 10; // / 100 * 1000 => mA
                Serial.print("Dòng sạc mA hiện tại là: ");
                Serial.println(I_current);
                if (I_current < thresholdCurrent)
                {
                    Serial.print("Threshold Current.");
                    if (millis() - users[i].breakTime < period)
                    {
                        Serial.println("Period.");
                        continue;
                    }
                    else
                    {
                        Serial.println("Đóng cổng sạc sao khi thấy quá thời gian chờ.");
                        users[i].lastTime = millis();
                        unsigned long totalTime = users[i].lastTime - users[i].firstTime;
                        SendPower(users[i].amp, users[i].uid, totalTime);

                        digitalWrite(portPowerOutlet[i], 0);
                        users[i].uid = 0;
                        users[i].breakTime = 0;
                        users[i].firstTime = 0;
                        users[i].lastTime = 0;
                        users[i].amp = 0;
                    }
                }
                else
                {
                    users[i].breakTime = millis();
                    if (users[i].amp < I_current)
                    {
                        users[i].amp = I_current;
                    }

                    Serial.println("Dong sac của thiết bị hiện tại là:");
                    Serial.println(users[i].amp);
                }
            }
        }

        return;
    }

    if (!mfrc522.PICC_ReadCardSerial())
    {
        return;
    }

    Serial.println("Card UID: ");
    for (byte i = 0; i < mfrc522.uid.size; i++)
    {
        uidDecTemp = mfrc522.uid.uidByte[i];
        uidDec = uidDec * 256 + uidDecTemp;
    }
    Serial.print(uidDec);

    // Hien thi loai the
    byte piccType = mfrc522.PICC_GetType(mfrc522.uid.sak); // Tra cuu dinh dang the
    Serial.println("Tip karty / Card type: ");
    Serial.print(mfrc522.PICC_GetTypeName(piccType));
    // CHECK: condition
    if (piccType != MFRC522::PICC_TYPE_MIFARE_UL || true)
    {
        Serial.println(" Type Card in else.");
        Serial.print(piccType);
        Serial.println(" Type Card is not valid");

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Card isn't valid");
        lcd.setCursor(0, 1);
        lcd.print("Sorry, Again.");
        delay(3000);
        mfrc522.PICC_HaltA();
        mfrc522.PCD_StopCrypto1();
        return;
    }

    if (uidDec == 3258053915 || uidDec == 3005101082)
    {
        int count = 0;
        for (int i = 0; i < numberPort; i++)
        {
            if (digitalRead(portPowerOutlet[i]) == 0)
            {
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Port");
                lcd.setCursor(6, 0);
                lcd.print(i);
                lcd.setCursor(8, 0);
                lcd.print("Open.");
                lcd.setCursor(0, 1);
                lcd.print("You can charge");
                delay(3000);

                digitalWrite(portPowerOutlet[i], 1);

                users[i].uid = uidDec;
                users[i].breakTime = millis();
                users[i].firstTime = millis();
                Serial.print("UID của thiết bị đang sạc là: ");
                Serial.println(users[i].uid);

                break;
            }
            else
            {
                count++;
            }
            if (count == 5)
            {
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Sorry,");
                lcd.setCursor(0, 1);
                lcd.print("System is Full");
                delay(3000);
            }
        }
    }
    else
    {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Card isn't Valid.");
        lcd.setCursor(0, 1);
        lcd.print("Sorry, Again.");
        delay(3000);
    }

    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
}

void SendPower(float amp, unsigned long uid, unsigned long totalTime)
{
    JSONVar myObject;

    myObject["amp"] = String(amp);
    myObject["uid"] = String(uid);
    myObject["totalTime"] = String(totalTime);

    String jsonString = JSON.stringify(myObject);

    Serial2.println(jsonString);
    Serial2.flush();
}
