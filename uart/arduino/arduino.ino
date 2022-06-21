#include <Arduino_JSON.h>

unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;

bool stringComplete = false;
String inputString = "";

void UART();

void setup()
{

    Serial.begin(9600);
    Serial2.begin(115200);
}

void loop()
{
    //  String sData = "{\"current\":\"" + String(7) + "\","
    //                 + "\"uid\":\"" + String(77) + "\","
    //                 + "\"lastTime\":\"" + String(8) + "\","
    //                 + "\"firstTime\":\"" + String(88) + "\"}";
    //
    //  Serial2.println(sData);
    //  Serial2.flush();
    //  Serial.println(sData);
    demoCreation();
    delay(10000);
}

void demoCreation()
{
    Serial.println("creation");
    Serial.println("========");
    JSONVar myObject;

    myObject["amp"] = String(7);
    myObject["uid"] = String(1233124);
    myObject["totalTime"] = "12432";

    Serial.print("myObject.keys() = ");
    Serial.println(myObject.keys());

    // JSON.stringify(myVar) can be used to convert the json var to a String
    String jsonString = JSON.stringify(myObject);

    Serial.print("JSON.stringify(myObject) = ");
    Serial.println(jsonString);
    Serial2.println(jsonString);
    Serial2.flush();
    Serial.println();

    // myObject.keys() can be used to get an array of all the keys in the object
    JSONVar keys = myObject.keys();

    for (int i = 0; i < keys.length(); i++)
    {
        JSONVar value = myObject[keys[i]];

        Serial.print("JSON.typeof(myObject[");
        Serial.print(keys[i]);
        Serial.print("]) = ");
        Serial.println(JSON.typeof(value));

        Serial.print("myObject[");
        Serial.print(keys[i]);
        Serial.print("] = ");
        Serial.println(value);

        Serial.println();
    }

    Serial.println();

    // setting a value to undefined can remove it from the object
    myObject["x"] = undefined;

    // you can also change a value
    myObject["hello"] = "there!";

    Serial.print("myObject = ");
    Serial.println(myObject);
}
