#include "ACS712.h"

ACS712 sensor(ACS712_30A, A0); //Cảm biến 30A, kết nối chân A0
//ACS712_05B
//ACS712_20A
//ACS712_30A
float tong;
float I;
float I_TB;
float ma;
float p;

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
    //Serial.print("I = "); Serial.print(I); Serial.print("   ");
    tong = tong + I;
    //Serial.print("Tổng = "); Serial.println(tong);
  }
  I_TB = tong/100;
  tong = 0;
  p = 220 * I_TB ; //Tính công suất P = U x I
  //Serial.print("\t\t\t");
  Serial.print("I = "); Serial.print(I_TB); Serial.print("A"); Serial.print("   ");
  ma = I_TB * 1000;   
  Serial.print("mA = "); Serial.print(ma); Serial.print("mA"); Serial.print("   ");
  Serial.print("P = "); Serial.print(p); Serial.println("W");
  delay(100);
}
