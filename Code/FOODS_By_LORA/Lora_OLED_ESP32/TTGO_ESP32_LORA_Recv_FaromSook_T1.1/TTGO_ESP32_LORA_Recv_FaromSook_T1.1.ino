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
    Serial.println(strRecv);
    //String txtSw1 = SplitString(line, ',', 1);
    
    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
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
