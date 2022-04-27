#include <SPI.h>
#include <MFRC522.h> // thu vien "RFID".
#define SS_PIN 10
#define RST_PIN 9

#include "ACS712.h"

// Cảm biến 30A, kết nối chân A0
// ACS712_05B
// ACS712_20A
// ACS712_30A
float tong;
float I;
float I_TB;
float mA;
float p;

MFRC522 mfrc522(SS_PIN, RST_PIN);
unsigned long uidDec, uidDecTemp; // hien thi so UID dang thap phan

const int LED1 = 2; // LED do
const int LED2 = 3; // LED xanh

int period = 1000;
unsigned long time_now = 0;

unsigned long ticketNumber;

/**
  1. Khi người dùng quẹt thẻ, xác định được người dùng => mở ổ điện
   uid của người dùng sẽ được lưu vào đâu ???
   2. Khi người dùng rút sạc.
   sau 10s ổ sạc sẽ tắt, gửi dữ liệu của ổ sạc vừa tắt đó lên server với uid của người vừa sạc
   3. làm sao biết được ổ nào không còn người sạc nữa
   mình sẽ dựa vào dòng điện đọc được mà phán doán xem có còn người sạc ổ đó hay không.

*/

class User
{
public:
    int port;
    int uid;
    unsigned long firstTime;
    unsigned long lastTime;
    unsigned long breakFirstTime;
    float avgCurrent;
    User(int _port, int _uid, unsigned long _breakFirstTime, unsigned long _firstTime, unsigned long _lastTime)
    {
        port = _port;
        uid = _uid;
        breakFirstTime = _breakFirstTime;
        firstTime = _firstTime;
        lastTime = _lastTime;
    }
};

User user_0(0, 0, 0, 0, 0);
User user_1(1, 0, 0, 0, 0);
User user_2(2, 0, 0, 0, 0);
User user_3(3, 0, 0, 0, 0);
User user_4(4, 0, 0, 0, 0);

User Users[5] = {user_0, user_1, user_2, user_3, user_4};
int powerOutlet[5]; // mảng chứa trạng thái của 5 ổ điện => mức 1 nếu ổ điện đang được sử dụng
int portPowerOutlet[5] = {22, 23, 24, 25, 26};

ACS712 sensor_0(ACS712_30A, A0);
ACS712 sensor_1(ACS712_30A, A1);
ACS712 sensor_2(ACS712_30A, A2);
ACS712 sensor_3(ACS712_30A, A3);
ACS712 sensor_4(ACS712_30A, A4);

void setup()
{
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    Serial.begin(9600);
    SPI.begin();
    mfrc522.PCD_Init();
    Serial.println("Prilozhite kartu / Waiting for card...");
    Serial.println("Đảm bảo không có dòng điện đi qua cảm biến trong quá trình cân bằng");
    delay(1000);
    Serial.println("Calibrating...");
    delay(200);
    sensor.calibrate();
    Serial.println("Quá trình cân bằng hoàn tất!!!");
}

void loop()
{
    // Tim the moi
    if (!mfrc522.PICC_IsNewCardPresent())
    {
        // đợi thời gian để cắm thiết bị sạc =>15s => tắt sau 15s
        for (int i = 0; < powerOutlet.length; i++)
        {
            // read dữ liệu port ở vị trí được bật
            if (powerOutlet[i] == 1)
            {
                for (int i = 0; i < 100; i++)
                {
                    I = sensor.getCurrentAC();
                    tong = tong + I;
                }
                float I_current = tong / 100;
                I_TB = (I_current + I_TB) / 2;
                tong = 0;
                p = 220 * I_TB;
                mA = I_TB * 1000;
                // Users[i].breakFirstTime
                if (I_current * 1000 < 100)
                {
                    if (millis() - Users[i].breakFirstTime < 15000)
                    {
                        continue;
                    }
                    else
                    {
                        Users[i].lastTime = millis();
                        powerOutlet[i] = 0;
                    }
                }
                else
                {
                    Users[i].breakFirstTime = millis();
                    if (Users[i].firstTime == 0)
                    {
                        Users[i].firstTime = millis();
                    }
                    Users[i].avgCurrent = I_TB;
                }
            }
        }

        return;
    }
    // Doc the
    if (!mfrc522.PICC_ReadCardSerial())
    {
        return;
    }
    uidDec = 0;

    // Hien thi so UID cua the
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

    if (piccType != MFRC522::PICC_TYPE_MIFARE_UL)
    {
        // FIXME: Neu khong dung dinh dang the, thì thoát
        Serial.println("Not a valid card: Type");
        Serial.print("[");
        Serial.print(piccType);
        Serial.println("]");
        Serial.println("================================================");
        mfrc522.PICC_HaltA();
        mfrc522.PCD_StopCrypto1();
        return;
    }

    if (uidDec == 3258053915 || uidDec == 3005101082)
    {
        // xem thử port nào chưa sử dụng thì bật lên sử dụng
        // nếu port thứ i thì user_[i]
        for (int i = 0; i < powerOutlet.length; i++)
        {
            if (powerOutlet[i] == 0)
            {
                powerOutlet[i] = 1;
                digitalWrite(portPowerOutlet[i], 1);

                Users[i].uid = uidDec;
                Users[i].breakFirstTime = millis();
            }
        }
    }
    else
    {
        Serial.println("Sai the");
    }

    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
}
