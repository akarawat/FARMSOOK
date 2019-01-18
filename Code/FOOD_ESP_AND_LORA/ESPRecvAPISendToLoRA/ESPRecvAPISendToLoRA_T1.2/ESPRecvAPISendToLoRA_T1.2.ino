/*
  - แก้ไขจากเวอร์ชั่นใช้ที่บ้าน มีเครื่องหมาย [] มาจาก php แก้ที่ php file ไม่ให้มี []
  - Relay active LOW ต้องระวังตรงเงื่อนไข if else
  - host ใช้เป็น led.scsthai.com ได้เลย
  - url แก้ตามช้งานจริง
  - แก้ไขให้หน่วงเวลาแค่ 3 วินาที
  - เพิ่ม ให้ใช้กับ ไฟสะพาน และมอเตอร์ได้
  - LED Blink with out delay
  - แก้ไข ไฟกระพริบ ให้ทำงานเฉพาะตอนเปิด Servo
  - เพิ่ม Servo.detach หลังจากจบทำงาน
  - ทำงานได้ปกติ แต่จะย้ายไปทำงานบน LORA เนื่องจาก Wifi Router รองรับอค่ 5 Clients
*/
/*
  - ปรับปรุงจากเวอร์ชั่น สวิทช์ 1 ตัว ใช้กับไฟสะพาน และมอเตอร์ปั้ม from : FARMSOOK_FOOD_Servo_V3.ino
  - ใช้กับการให้อาหารทั้ง 3 กล่อง
  - ทำงานร่วมกับ API : http://led.scsthai.com/ledjson_farmsook_multi_foods.php?ledno=farmsook4&ledno2=farmsook5&ledno3=farmsook6
*/
//#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <WiFiManager.h>
WiFiManager wifiManager;
int IntRst = 15; // Pin D8=15,
/*
  int Outp = 13; // Pin D7=13,
  int Outp2 = 12; // Pin D6=12,
  int Outp3 = xx; // Pin Dx=xx,
*/
WiFiClient client;

#include <SoftwareSerial.h>                     // to use pin 4 and 5 as RX and TX ports
SoftwareSerial mySerial(14, 12); // (D5=14)TX, (D6=12)RX

String ledID = "farmsook4";//5760561,9983908,3161443,4037176,
String ledID2 = "farmsook5";
String ledID3 = "farmsook6";

const int ledPin =  LED_BUILTIN;
int ledState = LOW;

int updCnt = 0;
int dots = 0;
long dotTime = 0;
long clkTime = 0;
String date;
String YY, MM, DD;

#define SECONDS_DS(seconds) ((seconds)*1000000UL)
void setup(void) {
  delay(100);
  Serial.begin(115200);
  mySerial.begin(115200);

  pinMode(ledPin, OUTPUT);
  wifiManager.autoConnect("SCS_SMARTPLUG");
  pinMode(IntRst, INPUT);
}

const char* host = "led.scsthai.com";

void loop(void) {
  delay(100);
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
      ///ledjson_farmsook_multi_switch.php?ledno=farmsook2&ledno2=farmsook3
      String url = "/ledjson_farmsook_multi_foods.php?ledno=" + ledID + "&ledno2=" + ledID2 + "&ledno3=" + ledID3 + ""; // แก้ไขใหม่ ไม่ให้ php ส่ง [] ไปใน json

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
      //line = {\"ledno\":\"farmsook4\",\"msg\":\"OFF\",\"secoff\":\"0\"},{\"ledno\":\"farmsook5\",\"msg\":\"OFF\",\"secoff\":\"0\"},{\"ledno\":\"farmsook6\",\"msg\":\"OFF\",\"secoff\":\"0\"}
      Serial.print("Line:");
      Serial.println(line);

      String txtSw1 = getValue(line, ',', 1);
      String txtSw2 = getValue(line, ',', 4);
      String txtSw3 = getValue(line, ',', 7);

      String LEDMsg = getValue(txtSw1, '"', 3);
      String LEDMsg2 = getValue(txtSw2, '"', 3);
      String LEDMsg3 = getValue(txtSw3, '"', 3);

      Serial.println("SW1:" + LEDMsg);
      Serial.println("SW2:" + LEDMsg2);
      Serial.println("SW3:" + LEDMsg3);
      Serial.println("closing connection");

      String txtSer1 = "", txtSer2 = "", txtSer3 = "";
      bool led1 = false, led2 = false, led3 = false;

      if (LEDMsg == "ON") { //On เป็น LOW เพราะ ใช้ Solid State Active Low
        //-->digitalWrite(Outp, LOW);
        //digitalWrite(ledPin, LOW);
        led1 = true;
        Serial.println("SW1 ON");
        txtSer1 = "FS04:ON";
      }
      if (LEDMsg == "OFF") {
        //-->digitalWrite(Outp, HIGH);
        //digitalWrite(ledPin, HIGH);
        Serial.println("SW1 OFF");
        txtSer1 = "FS04:OFF";
      }
      if (LEDMsg2 == "ON") { //On เป็น LOW เพราะ ใช้ Solid State Active Low
        //-->digitalWrite(Outp2, LOW);
        //digitalWrite(ledPin, LOW);
        led2 = true;
        Serial.println("SW2 ON");
        txtSer2 = "FS05:ON";
      }
      if (LEDMsg2 == "OFF") {
        //-->digitalWrite(Outp2, HIGH);
        //digitalWrite(ledPin, HIGH);
        //Serial.println("SW2 OFF");
        txtSer2 = "FS05:OFF";
      }
      if (LEDMsg3 == "ON") { //On เป็น LOW เพราะ ใช้ Solid State Active Low
        //-->digitalWrite(Outp3, LOW);
        //digitalWrite(ledPin, LOW);
        led3 = true;
        Serial.println("SW3 ON");
        txtSer3 = "FS06:ON";
      }
      if (LEDMsg3 == "OFF") {
        //-->digitalWrite(Outp3, HIGH);
        //digitalWrite(ledPin, HIGH);
        Serial.println("SW3 OFF");
        txtSer3 = "FS06:OFF";
      }

      if (led1 || led2 || led3) {
        digitalWrite(ledPin, LOW);
      } else {
        digitalWrite(ledPin, HIGH);
      }
      
      mySerial.println(txtSer1 + "," + txtSer2 + "," + txtSer3);
      Serial.println("Data loaded");

    }

    if (millis() - dotTime > 3000) { // ความถี่การอัพเดท
      dotTime = millis();
      dots = !dots;
      updCnt--;
    }

  }
}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length();

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
