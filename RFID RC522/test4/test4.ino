#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9
#define SS_PIN          10
struct User
{
  int UID[4];
  String TimeEnd;
  String TimeStart;
  long TotalCurrent;
  int totalReadCurrent;
};

int UID[4], i;

int ID1[4] = {194, 49, 245, 27}; //Thẻ bật tắt đèn
int led = 8;

int dem = 0;
User PersonOne = { "", "", 0, 0};
User PersonTwo = { "", "", 0, 0};

MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup()
{

  Serial.begin(9600);

  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);

  SPI.begin();
  mfrc522.PCD_Init();


}

void loop()
{

  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }

  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }

  Serial.print("UID của thẻ: ");

  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    UID[i] = mfrc522.uid.uidByte[i];
    Serial.print(UID[i]);
    PersonOne.UID[i] = mfrc522.uid.uidByte[i];
  }
  Serial.println("   ");

  if (PersonOne.UID[i] == ID1[i])
  {
    dem ++;
    Serial.print("Biến Đếm: ");
    Serial.println(dem);

    if ( (dem % 2) == 1) //Số lẻ đèn ON
    {
      digitalWrite(led, HIGH);
      Serial.println("ĐÈN ON");
    }
    else
    {
      digitalWrite(led, LOW);
      Serial.println("ĐÈN OFF");
    }
  }

  else
  {
    Serial.println("SAI THẺ........");
  }

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();

}
