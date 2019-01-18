/*
  - แก้ไขจากเวอร์ชั่นใช้ที่บ้าน มีเครื่องหมาย [] มาจาก php แก้ที่ php file ไม่ให้มี []
  - Relay active LOW ต้องระวังตรงเงื่อนไข if else
  - host ใช้เป็น led.scsthai.com ได้เลย
  - url แก้ตามช้งานจริง
  - แก้ไขให้หน่วงเวลาแค่ 3 วินาที
  - เพิ่ม ให้ใช้กับ ไฟสะพาน และมอเตอร์ได้
  - LED Blink with out delay
*/
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <WiFiManager.h>

#include <Servo.h>


WiFiManager wifiManager;
int IntRst = 15; // Pin D8=15,
int Outp = 13; // Pin D7=13,
WiFiClient client;

const int ledPin =  LED_BUILTIN;
int ledState = LOW; 
unsigned long previousMillis = 0; 
const long interval = 200; 

String ledID = "farmsook4";//5760561,9983908,3161443,4037176,
String LEDMsg = "";
String weatherString;
int updCnt = 0;
int dots = 0;
long dotTime = 0;
long clkTime = 0;
String date;
String YY, MM, DD;

#define SECONDS_DS(seconds) ((seconds)*1000000UL)

// Declare the Servo pin
int servoPin = 12; //Pin D6=12, D5=14
// Create a servo object
Servo Servo1;

void setup(void) {
  delay(100);
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

  wifiManager.autoConnect("SCS_SMARTPLUG");
  pinMode(IntRst, INPUT);
  pinMode(Outp, OUTPUT);
  digitalWrite(Outp, LOW);

  //Servo1.attach(servoPin);
}

const char* host = "led.scsthai.com";

void loop(void) {
  delay(100);
  Servo1.attach(servoPin);

  int stateBtn = digitalRead(IntRst);
  if (stateBtn == 1) {
    Serial.println("Reset : " + stateBtn);
    wifiManager.resetSettings();
  } else {
    //digitalWrite(Outp, LOW);

    // Clock Start
    if (updCnt <= 0) { // ทุก 1 นาทีอัพเดทข้อมูล
      //updCnt = 10;
      updCnt = 1;
      Serial.println("Getting data ...");

      //getWeatherData();

      Serial.print("connecting to ");
      Serial.println(host);

      // Use WiFiClient class to create TCP connections
      WiFiClient client;
      const int httpPort = 80;
      if (!client.connect(host, httpPort)) {
        Serial.println("connection failed");
        return;
      }

      String url = "/ledjson_farmsook.php?ledno=" + ledID + ""; // แก้ไขใหม่ ไม่ให้ php ส่ง [] ไปใน json

      Serial.print("Requesting URL: ");
      Serial.println(url);

      client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                   "Host: " + host + "\r\n" +
                   "Connection: close\r\n\r\n");
      unsigned long timeout = millis();
      while (client.available() == 0) {
        if (millis() - timeout > 5000) {
          Serial.println(">>> Client Timeout !");
          client.stop();
          return;
        }
      }

      String line;
      while (client.available()) {
        line = client.readStringUntil('\r');
        Serial.print(line);
      }

      Serial.println();
      //line = "{\"ledno\":\"farmsook1\",\"msg\":\"ON\"}";
      Serial.print("Line:");
      Serial.println(line);

      DynamicJsonBuffer jsonBuf;
      JsonObject &root = jsonBuf.parseObject(line);
      if (!root.success())
      {
        Serial.println("parseObject() failed");
        return;
      }
      Serial.println(root["msg"].as<String>());
      LEDMsg = root["msg"].as<String>();

      weatherString = LEDMsg;
      Serial.println(weatherString);
      Serial.println("closing connection");

      if (weatherString == "ON") { //On เป็น LOW เพราะ ใช้ Solid State Active Low
        Serial.println("ON");
        digitalWrite(Outp, LOW);
        Servo1.write(0);

        delay(1000);
        Servo1.write(180);
        Servo1.detach();

      }
      if (weatherString == "OFF") {
        Serial.println("OFF");
        digitalWrite(Outp, HIGH);
        Servo1.write(180);

      }
      Serial.println("Data loaded");
    }
    if (millis() - dotTime > 2000) { // ความถี่การอัพเดท
      dotTime = millis();
      dots = !dots;
      updCnt--;
    }

  }
  //
   unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    // set the LED with the ledState of the variable:
    digitalWrite(ledPin, ledState);
  }
}

