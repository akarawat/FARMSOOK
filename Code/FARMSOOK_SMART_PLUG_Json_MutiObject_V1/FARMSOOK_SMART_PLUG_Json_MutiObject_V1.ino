/*
  - ปรับปรุงจากเวอร์ชั่น สวิทช์ 1 ตัว
  - เพิ่มส่วน json รับค่ามากกว่า 1 object
  - ไม่ได้ใช้ Json Object แต่ใช้ SubString แทนเลย
  - ใช้กับไฟสะพาน และมอเตอร์ปั้ม
  - ทำงานร่วมกับ http://led.scsthai.com/ledjson_farmsook_multi_switch.php?ledno=farmsook2&ledno2=farmsook3
  
*/
//#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <WiFiManager.h>
WiFiManager wifiManager;
int IntRst = 15; // Pin D8=15,
int Outp = 13; // Pin D7=13,
int Outp2 = 12; // Pin D6=12,
WiFiClient client;

String ledID = "farmsook2";//5760561,9983908,3161443,4037176,
String ledID2 = "farmsook3";
String LEDMsg = "";
String LEDMsg2 = "";

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
  wifiManager.autoConnect("SCS_SMARTPLUG");
  pinMode(IntRst, INPUT);
  pinMode(Outp, OUTPUT);
  digitalWrite(Outp, LOW);
  pinMode(Outp2, OUTPUT);
  digitalWrite(Outp2, LOW);
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
      String url = "/ledjson_farmsook_multi_switch.php?ledno=" + ledID + "&ledno2=" + ledID2 + ""; // แก้ไขใหม่ ไม่ให้ php ส่ง [] ไปใน json

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
      //line = "{\"ledno\":\"farmsook2\",\"msg\":\"OFF\",\"secoff\":\"0\"},{\"ledno\":\"farmsook3\",\"msg\":\"OFF\",\"secoff\":\"0\"}";
      Serial.print("Line:");
      Serial.println(line);

      String txtSw1 = getValue(line, ',', 1);
      String txtSw2 = getValue(line, ',', 4);
      
      String LEDMsg = getValue(txtSw1, '"', 3);
      String LEDMsg2 = getValue(txtSw2, '"', 3);

      Serial.println(LEDMsg);
      Serial.println(LEDMsg2);
      Serial.println("closing connection");

      if (LEDMsg == "ON") { //On เป็น LOW เพราะ ใช้ Solid State Active Low
        digitalWrite(Outp, LOW);
        Serial.println("SW1 ON");
      }
      if (LEDMsg == "OFF") {
        digitalWrite(Outp, HIGH);
        Serial.println("SW1 OFF");
      }
      if (LEDMsg2 == "ON") { //On เป็น LOW เพราะ ใช้ Solid State Active Low
        digitalWrite(Outp2, LOW);
        Serial.println("SW2 ON");
      }
      if (LEDMsg2 == "OFF") {
        digitalWrite(Outp2, HIGH);
        Serial.println("SW2 OFF");
      }
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
