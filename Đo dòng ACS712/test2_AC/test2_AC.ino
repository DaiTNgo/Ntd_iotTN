//Thư viện: https://github.com/rkoptev/ACS712-arduino

#include "ACS712.h"

ACS712 sensor(ACS712_30A, A0); //Cảm biến 30A, kết nối chân A0
//ACS712_05B
//ACS712_20A
//ACS712_30A
float tong;
float I;
float I_TB;
float ma;

void setup() 
{
  Serial.begin(9600);

  // calibrate cảm biến
  // Trong quá trình cali phải đảm bảo KHÔNG có dòng điện đi qua cảm biến
  Serial.println("Đảm bảo không có dòng điện đi qua cảm biến trong quá trình cân bằng");
  delay(1000);
  Serial.println("Calibrating...");
  delay(200);
  sensor.calibrate();
  Serial.println("Quá trình cân bằng hoàn tất!!!");
}

void loop() 
{
  for (int i=0; i<=100; i++)
  {
    I = sensor.getCurrentAC(); //Đo dòng AC
    tong = tong + I;
  }
  I_TB = tong/100;
  tong = 0;
  ma = I_TB * 1000;   
  delay(100);
}
