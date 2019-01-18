// FarmSook By Tommy
// Board LoRa_OLED_ESP32_T1 Link https://www.arduinothai.com/product/1480/lora-868mhz-915mhz-esp32-0-96-oled
// Simple wifi

#include <WiFi.h>

const char* ssid     = "akps2";
const char* password = "ronin9449x";
String ledID = "farmsook4";//5760561,9983908,3161443,4037176,
String ledID2 = "farmsook5";
String ledID3 = "farmsook6";

void setup() {
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

}

const char* host = "led.scsthai.com";
void loop() {
  delay(100);

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
  
}

