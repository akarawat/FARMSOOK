/*
  สำหรับหน้าจอ OLED
  *ถ้าจะแสดงเป็น Scrolling ให้ใช้ การแสดงแบบ Buffer เข้าไปแสดงทีละแถว
  **ถ้าจะแสดงทีละแถวให้เขียนเข้าไปตรงๆได้เลย
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
 SSD1306  display(0x3c, 4, 15);
// OLED ENd

#define SS      18
#define RST     14
#define DI0     26
#define BAND    433E6 //915E6 //433E6

//String txt = "";
const char* txt;
void printBuffer(void) {
  // Initialize the log buffer
  // allocate memory to store 8 lines of text and 30 chars per line.
  display.setLogBuffer(5, 30);

  // Some test data
  const char* test[] = {
      "Hello",
      "World" ,
      txt,
      "Show off",
      "how",
      "the log buffer",
      "is",
      "working.",
      "Even",
      "scrolling is",
      "working"
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

void setup() {
  Serial.begin(115200);
  while (!Serial); //if just the the basic function, must connect to a computer
  delay(1000);
  Serial.println("LoRa Receiver"); 
  
  SPI.begin(5,19,27,18);
  LoRa.setPins(SS,RST,DI0);
  
  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  
  // OLED Start
  pinMode(16,OUTPUT);
  digitalWrite(16, LOW);    // set GPIO16 low to reset OLED
  delay(50); 
  digitalWrite(16, HIGH); // while OLED is running, must set GPIO16 in high
  display.init();
  // display.flipScreenVertically();
  display.setContrast(255);
  txt = "Setup Receiver...";
  printBuffer();
  delay(1000);
  display.clear();
  // OLED End
  
}
//int i = 1;
void loop() { 
  // LoRa Start
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
    Serial.print(strRecv);
    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());

    /*txt = strRecv.c_str();
    printBuffer();
    delay(1000);
    display.clear();*/

    display.clear();
    display.println(strRecv);
    display.drawLogBuffer(0, 0);
    display.display();
    delay(500);

  }
  // LoRa End
  
}
