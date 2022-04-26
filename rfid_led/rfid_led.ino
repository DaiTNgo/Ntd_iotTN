#include <SPI.h>
#include <MFRC522.h> // thu vien "RFID".
#define SS_PIN 10
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN);
unsigned long uidDec, uidDecTemp; // hien thi so UID dang thap phan

const int LED1 = 2; // LED do
const int LED2 = 3; // LED xanh

int period = 1000;
unsigned long time_now = 0;

unsigned long ticketNumber;

/**
 *1. Khi người dùng quẹt thẻ, xác định được người dùng => mở ổ điện
 * uid của người dùng sẽ được lưu vào đâu ???
 * 2. Khi người dùng rút sạc.
 * sau 10s ổ sạc sẽ tắt, gửi dữ liệu của ổ sạc vừa tắt đó lên server với uid của người vừa sạc
 * 3. làm sao biết được ổ nào không còn người sạc nữa
 * mình sẽ dựa vào dòng điện đọc được mà phán doán xem có còn người sạc ổ đó hay không.
 *
 */

class User
{
public:
    int port;
    int uid;
    String firstTime;
    String lastTime;
    unsigned long breakFirstTime;

    User(int _port, int _uid, unsigned long _breakFirstTime, String _firstTime, String _lastTime)
    {
        port = _port;
        uid = _uid;
        breakFirstTime = _breakFirstTime;
        firstTime = _firstTime;
        lastTime = _lastTime;
    }
};

User user_1(0, 0, 0, "", "");
User user_2(1, 0, 0, "", "");
User user_3(2, 0, 0, "", "");
User user_4(3, 0, 0, "", "");
User user_5(4, 0, 0, "", "");

int powerOutlet[5]; // mảng chứa trạng thái của 5 ổ điện => mức 1 nếu ổ điện đang được sử dụng

int portReadCurrent[5] = {22, 23, 24, 25, 26}

void
setup()
{
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    Serial.begin(9600);
    SPI.begin();
    mfrc522.PCD_Init();
    Serial.println("Prilozhite kartu / Waiting for card...");
}

void loop()
{
    // Tim the moi
    if (!mfrc522.PICC_IsNewCardPresent())
    {
        return;
    }

    // Doc the
    if (!mfrc522.PICC_ReadCardSerial())
    {
        return;
    }

    uidDec = 0;
    Serial.println("MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM");
    Serial.println("================================================");

    // Hien thi so UID cua the
    Serial.println("Serijnyj nomer karty / Card UID: ");
    for (byte i = 0; i < mfrc522.uid.size; i++)
    {
        uidDecTemp = mfrc522.uid.uidByte[i];
        uidDec = uidDec * 256 + uidDecTemp;
    }
    Serial.print("            [");
    Serial.print(uidDec);
    if (uidDec == 696335238)
    {
        digitalWrite(LED1, !digitalRead(LED1));
        delay(5000);
    }
    else
    {
    }
    //--------------------------------
    if (uidDec == 3501146325)
    {
        digitalWrite(LED2, !digitalRead(LED2));
        delay(5000);
    }
    else
    {
    }
    Serial.println("]");
    Serial.println("================================================");

    // Hien thi loai the
    byte piccType = mfrc522.PICC_GetType(mfrc522.uid.sak); // Tra cuu dinh dang the
    Serial.println("Tip karty / Card type: ");

    Serial.print(" [");
    Serial.print(mfrc522.PICC_GetTypeName(piccType)); // Chuyen dinh dang the ve kieu doc
    Serial.println("]");
    Serial.println("================================================");
    if (piccType != MFRC522::PICC_TYPE_MIFARE_UL)
    { // Neu khong dung dinh dang the

        Serial.println("Neizvestnaja karta / Not a valid card: Type");
        Serial.print("            [");
        Serial.print(piccType);
        Serial.println("]");
        Serial.println("================================================");
        Serial.println("WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW");

        // Halt PICC
        mfrc522.PICC_HaltA(); // dung lai
        return;
        delay(1);
    }

    // quẹt thẻ xác định danh tính chuẩn
    if (true)
    {
        // xem thử port nào chưa sử dụng thì bật lên sử dụng
        for (int i = 0; i < powerOutlet.length; i++)
        {
            if (powerOutlet[i] == 0)
            {
                powerOutlet[i] = 1;

                switch (i)
                {
                case 0:
                    user_1.uid = uidDec;
                    break;
                case 1:
                    user_2.uid = uidDec;
                    break;
                case 2:
                    user_3.uid = uidDec;
                    break;
                case 3:
                    user_4.uid = uidDec;
                    break;
                case 4:
                    user_5.uid = uidDec;
                    break;
                default:
                    break;
                }
                break;
            }
        }
    }

    // đợi thời gian để cắm thiết bị sạc =>15s => tắt sau 15s
    for (int i = 0; < powerOutlet.length; i++)
    {

        // read dữ liệu port ở vị trí được bật
        if (powerOutlet[i] == 1)
        {

            switch (i)
            {
            case 0:
                user_1.breakFirstTime = millis();
                break;
            case 1:
                user_2.breakFirstTime = millis();
                break;
            case 2:
                user_3.breakFirstTime = millis();
                break;
            case 3:
                user_4.breakFirstTime = millis();
                break;
            case 4:
                user_5.breakFirstTime = millis();
                break;
            default:
                break;
            }
            // read port portReadCurrent[i];
            /**
             * if read current < threshold
             * if (millis() - ti        me_now > period)
             * {
             * wait approx. [period] ms
             * time_now = millis();
             * }
             *
             *powerOutlet[i] = 0;
             *send data->server
             */
        }
    }

    // luồng chạy ra sau để nhận biết được trong 5 port thì có port họ hết quá trình sạc thiết bị

    // Halt PICC
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCryptol();
}
