/**
   BasicHTTPClient.ino

    Created on: 24.05.2015

*/

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#define USE_SERIAL Serial

ESP8266WiFiMulti WiFiMulti;

#include <SoftwareSerial.h>                     // to use pin 4 and 5 as RX and TX ports
SoftwareSerial mySerial(4, 5); // RX, TX

void setup() {
  mySerial.begin(115200);
  mySerial.println("Farmsook, Hello");


  USE_SERIAL.begin(115200);
  // USE_SERIAL.setDebugOutput(true);

  USE_SERIAL.println();
  USE_SERIAL.println();
  USE_SERIAL.println();

  for (uint8_t t = 4; t > 0; t--) {
    USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
    USE_SERIAL.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  //WiFiMulti.addAP("akps2", "ronin9449x");
  WiFiMulti.addAP("HUAWEI-BCDA", "LJM1ENBB");

}

String txt = "";
void loop() {
  txt = "";
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    HTTPClient http;

    USE_SERIAL.print("[HTTP] begin...\n");
    // configure traged server and url
    //http.begin("https://192.168.1.12/test.html", "7a 9c f4 db 40 d3 62 5a 6e 21 bc 5c cc 66 c8 3e a1 45 59 38"); //HTTPS

    //http.begin("http://api.thingspeak.com/update?api_key=5KRSELKLK4RF5PJU&field1=30"); //HTTP
    //http.begin("http://api.thingspeak.com/update?api_key=5KRSELKLK4RF5PJU&field1=" + txt); //HTTP
    http.begin("http://led.scsthai.com/ledjson.php?ledno=homeplug1"); //HTTP
    Serial.print("Send:");
    Serial.println(txt);

    USE_SERIAL.print("[HTTP] GET...\n");
    // start connection and send HTTP header
    int httpCode = http.GET();

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        USE_SERIAL.println(payload);
      }
    } else {
      USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }


  delay(15000);
}

