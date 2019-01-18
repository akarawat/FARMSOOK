/*
  สำหรับหน้าจอ OLED
   ถ้าจะแสดงเป็น Scrolling ให้ใช้ การแสดงแบบ Buffer เข้าไปแสดงทีละแถว
  **ถ้าจะแสดงทีละแถวให้เขียนเข้าไปตรงๆได้เลย
*/
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
  - เปลี่ยนจาก ทำงานบน ESP8622 มาทำงานบน LoRA32 
  #V1 มาจาก T4.1
*/
#include <SPI.h>
#include <LoRa.h>

// OLED Start
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306.h" // alias for `#include "SSD1306Wire.h"`
//OLED pins to ESP32 GPIOs via this connecthin:
//OLED_SDA -- GPIO4
//OLED_SCL -- GPIO15
//OLED_RST -- GPIO16

#include <WiFi.h>

SSD1306  display(0x3c, 4, 15);
// OLED ENd

#define SS      18
#define RST     14
#define DI0     26
#define BAND    433E6 //915E6 //433E6

//String txt = "";
const char* txt;
const char* txt2;
const char* txt3;
void printBuffer(void) {
  // Initialize the log buffer
  // allocate memory to store 8 lines of text and 30 chars per line.
  display.setLogBuffer(5, 30);

  // Some test data
  const char* test[] = {
    "Hello",
    "FarmSook" ,
    txt,
    "Show off",
    "how",
    "the log buffer",
    "is",
    "working.",
    "Please wait",
    "for",
    "connecting..."
  };

  for (uint8_t i = 0; i < 11; i++) {
    display.clear();
    // Print to the screen
    display.println(test[i]);
    // Draw it to the internal screen buffer
    display.drawLogBuffer(0, 0);
    // Display it on the screen
    display.display();
    delay(500);
  }
}

const char* ssid     = "akps2";
const char* password = "ronin9449x";
String ledID = "farmsook4";//5760561,9983908,3161443,4037176,
String ledID2 = "farmsook5";
String ledID3 = "farmsook6";

void setup() {
  Serial.begin(115200);
  while (!Serial); //if just the the basic function, must connect to a computer
  delay(1000);

  // OLED Start
  pinMode(16, OUTPUT);
  digitalWrite(16, LOW);    // set GPIO16 low to reset OLED
  delay(50);
  digitalWrite(16, HIGH); // while OLED is running, must set GPIO16 in high
  display.init();
  // display.flipScreenVertically();
  display.setContrast(255);
  txt = "Started...";
  printBuffer();
  delay(1000);
  display.clear();
  // OLED End

  Serial.println("LoRa Receiver");
  SPI.begin(5, 19, 27, 18);
  LoRa.setPins(SS, RST, DI0);
  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  // Wifi Start
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
  // Wifi End

}
int i = 1;
const char* host = "led.scsthai.com";
void loop() {
  delay(100);

  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
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
  //line = {\"ledno\":\"farmsook4\",\"msg\":\"OFF\",\"secoff\":\"0\",\"ddt\":\"17:12:28\"},{\"ledno\":\"farmsook5\",\"msg\":\"OFF\",\"secoff\":\"0\",\"ddt\":\"17:12:28\"},{\"ledno\":\"farmsook6\",\"msg\":\"OFF\",\"secoff\":\"0\",\"ddt\":\"17:12:28\"}
  Serial.print("Line:");
  Serial.println(line);

  i++;
  String strRecv = "" + String(i);

  String txtSw1 = getValue(line, ',', 1);
  String txtSw2 = getValue(line, ',', 5);
  String txtSw3 = getValue(line, ',', 9);
  String txtSw4 = getValue(line, ',', 11); // ดึง Date Time

  String LEDMsg = getValue(txtSw1, '"', 3);
  String LEDMsg2 = getValue(txtSw2, '"', 3);
  String LEDMsg3 = getValue(txtSw3, '"', 3);
  String LEDMsg4 = getValue(txtSw4, '"', 3);

  String sLora = "SW1:" + LEDMsg + "," + "SW2:" + LEDMsg2 + "," + "SW3:" + LEDMsg3;
  Serial.println("SW1:" + LEDMsg);
  Serial.println("SW2:" + LEDMsg2);
  Serial.println("SW3:" + LEDMsg3);
  Serial.println("closing connection");

  // OLED Start
  display.clear();
  display.println(LEDMsg4);
  display.drawLogBuffer(0, 0);
  display.display();

  display.clear();
  display.println("[1:" + LEDMsg + "] [2:" + LEDMsg2 + "] [3:" + LEDMsg3 + "]");
  display.drawLogBuffer(0, 0);
  display.display();
  delay(500);
  // OLED End

  // LoRA Start
  LoRa.beginPacket();
  LoRa.print("FMS,");
  LoRa.print(string2char(sLora));
  LoRa.endPacket();
  // LoRA End

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
char* string2char(String command) {
  if (command.length() != 0) {
    char *p = const_cast<char*>(command.c_str());
    return p;
  }
}
