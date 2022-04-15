
//(0.19 * annalog - 25) for 20A
//(0.044 * annalog - 3.78) for 20A
void setup(){
Serial.begin(9600);
}
void loop() [
float average = 0;
for(int i = 0; i < 1000 ; i++){
average = average + (0.0264 * analogRead(A0) - 13.51) /1000;
delay(1);
}
Serial.println(average);
}