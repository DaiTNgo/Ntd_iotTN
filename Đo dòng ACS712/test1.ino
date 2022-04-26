#include "ACS712.h"

ACS712 sensor(ACS712_30A, A0); //Cảm biến 30A, kết nối chân A0
//ACS712_05B
//ACS712_20A
//ACS712_30A
int zero;
float I;
float I_TB;
float tong;
float mA;

void setup() 
{
  Serial.begin(9600);
  Serial.println("Đảm bảo không có dòng điện đi qua cảm biến trong quá trình cân bằng");
  delay(1000);
  Serial.println("Calibrating...");
  delay(200);
  zero = sensor.calibrate();
  Serial.println("Quá trình cân bằng hoàn tất!!!");
  Serial.print("Điểm Zero của cảm biến = "); 
  Serial.println(zero); 
}

void loop() 
{
    unsigned int time = millis();
  
  for (int i=0; i<100; i++)
  {
    I = sensor.getCurrentDC(); //Đo dòng DC
    tong = tong + I;
}

  I_TB = tong/100 ;
  tong = 0;
  mA = I_TB * 1000;   
  delay(1000);
}
