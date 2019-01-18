// Tommy Test by TTGO_ESP32_LORA SX1278
// Upload by ESP32_Dev_Module
// รับค่าจาก OLED ที่รับมาจากเว็บ http://led.scsthai.com/ledjson_farmsook_multi_foods.php?ledno=farmsook4&ledno2=farmsook5&ledno3=farmsook6
// ได้ค่าแล่วส่งไปเปิด Servo 9G

/*
   ตัวอย่าง Data ที่รับ
  Received packet 'FMS,SW1:OFF,SW2:OFF,SW3:OFF' with RSSI -37
  Received packet 'FMS,SW1:OFF,SW2:OFF,SW3:OFF' with RSSI -37
*/

#include <SPI.h>
#include <LoRa.h>
#include <Servo.h>

// WIFI_LoRa_32 ports

// GPIO5  -- SX1278's SCK
// GPIO19 -- SX1278's MISO
// GPIO27 -- SX1278's MOSI
// GPIO18 -- SX1278's CS
// GPIO14 -- SX1278's RESET
// GPIO26 -- SX1278's IRQ(Interrupt Request)

#define SS      18
#define RST     14
#define DI0     26
#define BAND    433E6

void setup() {
  Serial.begin(115200);
  while (!Serial); //if just the the basic function, must connect to a computer
  delay(1000);

  Serial.println("LoRa Receiver");

  SPI.begin(5, 19, 27, 18);
  LoRa.setPins(SS, RST, DI0);

  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}
Servo myservo;
int pos = 0;
int servo = 16;
void loop() {
  // try to parse packet
  String strRecv = "";
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet '");

    // read packet
    while (LoRa.available()) {
      //Serial.print((char)LoRa.read());
      strRecv += (char)LoRa.read();
    }
    //FMS,SW1:OFF,SW2:OFF,SW3:OFF
    Serial.println(strRecv);
    String txtSw0 = SplitString(strRecv, ',', 0);
    if (txtSw0 == "FMS") {
      String txtSw1 = SplitString(strRecv, ',', 1);
      String txtSw2 = SplitString(strRecv, ',', 2);
      String txtSw3 = SplitString(strRecv, ',', 3);
      
        Serial.println(txtSw0);
        Serial.println(txtSw1);
        Serial.println(txtSw2);
        Serial.println(txtSw3);
      
      //String arrSw1 = SplitString(txtSw1, ':', 0);
      String stsSw1 = SplitString(txtSw1, ':', 1);
      //String arrSw2 = SplitString(txtSw2, ':', 0);
      String stsSw2 = SplitString(txtSw2, ':', 1);
      //String arrSw3 = SplitString(txtSw3, ':', 0);
      String stsSw3 = SplitString(txtSw3, ':', 1);

      //food4,food5,food6
      if (stsSw1 == "ON") {
        Serial.println(">>" + txtSw1);
        myservo.attach(servo);
        myservo.write(0);
        delay(1000);
        myservo.write(180);
        delay(1000);
        myservo.write(0);
        myservo.detach();
        delay(3000);
        /*
        for (pos = 0; pos <= 180; pos += 1) {
          myservo.write(pos);
          delay(1);
        }
        for (pos = 180; pos >= 0; pos -= 1) {
          myservo.write(pos);
          delay(1);
        }*/
      } else {

      }

    }
    // print RSSI of packet
    //>Serial.print("' with RSSI ");
    //>Serial.println(LoRa.packetRssi());
  }
}

String SplitString(String data, char separator, int index)
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
