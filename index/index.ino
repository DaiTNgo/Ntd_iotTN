#include <SoftwareSerial.h>
#include <SimpleKalmanFilter.h>

SimpleKalmanFilter bo_loc(1,1,0.2);
SoftwareSerial mySerial(12,13); //RX, TX

const int ACS = A0; // Chan noi voi ACS

const int Voltage = A1; // chan noi voi mach do ap

unsigned int sensorValue1 = 0;
unsigned int sensorValue2 =0 ;
unsigned int dem = 0;

unsigned long  tim = 0;

float vo = 0,
float ampe = 0;
float volt = 0;
float dien = 0;
float vo_k = 0;

unsigned int max1 = 0;
unsigned int min1 = 1023;
unsigned int max2 = 0;

volatitle byte pulseCount ;

float flowRate;

unsigned int flowSec,
unsigned int tottalMilliLitres = 0;



/* int OutPin = A0; // Lưu chân ra của cảm biến */
void setup() {
  //Đối với một chân analog bạn không cần pinMode
  /* Serial.begin(9600);//Mở cổng Serial ở mức 9600 */
  Serial.begin(115200);

  mySerial.begin(115200);

  /* pinMode(S201,INPUT); */
  /* digitalWrite(S201,HIGH); */

  pulseCount = 0;
  flowRate = 0.0;
  flowSec = 0;
  tottalMilliLitres = 0;

  attachInterrupt(0,pulseCount,FALLING);

}
 
void loop() {
  int value = analogRead(OutPin);     // Ta sẽ đọc giá trị hiệu điện thế của cảm biến
                                      // Giá trị được số hóa thành 1 số nguyên có giá trị
                                      // trong khoảng từ 0 đến 1023
  float volt = value / 5.0 * 20.0;  // Bây giờ ta chỉ cần tính ra giá trị dòng điện
                                      // Với mạch 30A ta sửa lại thành * 30.0
  Serial.println(volt);//Xuất ra serial Monitor. Nhấn Ctrl+Shift+M để xem                                     
  delay(10);

  while(millis() - tim <250){
    sensorValue1 = analogRead(ACS);
    sensorValue2 = analogRead(Voltage);

    if(max1 < sensorValue1){
      max1 = sensorValue1;
    }
    if(
      min1 > sensorValue1
    )
    {
      min1 = sensorValue1
    }
    if(max2 < sensorValue2){
      max2 = sensorValue2;
    }

  }
  dem++;

  tim = millis();

  vo = (max1 - min1) / 1024.0 * 2.5 /1.4142;
  vo_k = bo_loc.updateEstimate(vo); // loc nhieu voi thuat toan kalman

  max1 = 0;
  min1 = 1023;

  if(dem % 4 ==0){
    ampe = vo_k * 10.0;
    volt = ((max2 /102.4) + 1.4) / 1.4142 / 60 * 220.0 + 3;

    dien = dien + (volt * ampe * 0.9);

    max2 = 0;
  }

  if(dem >=40){
    mySerial.write(int(dien/ 10) >> 8);
    mySerial.write(int(dien/10));
    mySerial.write((tottalMilliLitres + 30000) >> 8);
    mySerial.write(tottalMilliLitres + 30000);

    dem = 0;
    tottalMilliLitres = 0;
    dien = 0;
  }

  Serial.println(millis());
}

void pulseCounter() {
  pulseCount ++;
}
