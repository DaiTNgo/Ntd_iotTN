// Structs
struct Customer
{
  String Id;
  String TimeEnd;
  String TimeStart;
  long TotalCurrent;
  int totalReadCurrent;
};

//class RGB_LED
//{
//private:
//    // Create the three variables for each pin on an RGB LED
//    int red;
//    int green;
//    int blue;
//
//public:
//    // Constructor for the clas
//    RGB_LED(int newRedPin, int newGreenPin, int newBluePin)
//    {
//        // Set the pins in private to the new pins
//        red = newRedPin;
//        blue = newBluePin;
//        green = newGreenPin;
//        // Set the pin mode of each pin to output
//        pinMode(red, OUTPUT);
//        pinMode(green, OUTPUT);
//        pinMode(blue, OUTPUT);
//    }
//
//    void setColor(int newRed, int newGreen, int newBlue)
//    {
//        // analogWrite() to each color pin
//        // analogWrite(pinName, 0-255);
//        analogWrite(red, newRed);
//        analogWrite(green, newGreen);
//        analogWrite(blue, newBlue);
//    }
//
//    void setColor(RGB rgb)
//    {
//        analogWrite(red, rgb.r);
//        analogWrite(green, rgb.g);
//        analogWrite(blue, rgb.b);
//    }
//
//    void off()
//    {
//        // Turn each led pin off
//        // Hint: analogWrite(pinName, 0)
//        analogWrite(red, 0);
//        analogWrite(green, 0);
//        analogWrite(blue, 0);
//    }
//};

// class Customer
// {

//     String Id;
//     String TimeEnd;
//     String TimeStart;

// public:
//     Customer(String UserId, String UserTimeEnd, String UserTimeStart)
//     {
//         Id = UserId;
//         TimeEnd = UserTimeEnd;
//         TimeStart = UserTimeStart;
//     }
//     void printHello()
//     {
//         Serial.print("Hello world");
//     }
// };

// The RGB LED object


void setup()
{
  Serial.begin(9600);
  Customer PersonOne = {"", "", "", 0, 0};
  Customer PersonTwo = {"", "", "", 0, 0};
  // No code required
}

void loop()
{

//  Serial.println("my service people lvalue: ");
//  Serial.println((int)&myServicePeople);
//  Serial.println("your service people lvalue: ");
//  Serial.println((int)&yourServicePeople);
//
//  yourServicePeople.ID = 205;
//
//  Serial.println("my service people id lvalue: ");
//  Serial.println(myServicePeople.ID);
//  Serial.println("your service people id lvalue: ");
//  Serial.println(yourServicePeople.ID);
//
//  myServicePeople = yourServicePeople;
//  Serial.println("After");
//  Serial.println("my service people id lvalue: ");
//  Serial.println(myServicePeople.ID);
//  Serial.println("your service people id lvalue: ");
//  Serial.println(yourServicePeople.ID);
//
//  Serial.println("my service people lvalue: ");
//  Serial.println((int)&myServicePeople);
//  Serial.println("your service people lvalue: ");
//  Serial.println((int)&yourServicePeople);
//  Serial.println("=================");
//
//  myServicePeople.ID = 12345;
//  Serial.println("After");
//  Serial.println("my service people id lvalue: ");
//  Serial.println(myServicePeople.ID);
//  Serial.println("your service people id lvalue: ");
//  Serial.println(yourServicePeople.ID);
//  Serial.println("=================");
//  customer_1.printHello();


}
