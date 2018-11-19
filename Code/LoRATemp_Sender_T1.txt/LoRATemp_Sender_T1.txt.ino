// Farm sook Tommy Test by TTGO_ESP32_LORA SX1278
// Upload by ESP32_Dev_Module

#include <SPI.h>
#include <LoRa.h>
#include<Arduino.h>

#include <LiquidCrystal_I2C.h>
// LDC Start //
//LiquidCrystal_I2C lcd(0x27, 20, 4); // YELLOW
LiquidCrystal_I2C lcd(0x3F, 20, 4); // BLUE
// LDC End //

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
#define BAND    433E6  //915E6 -- 这里的模式选择中，检查一下是否可在中国实用915这个频段

int counter = 0;
String stuId = "FFX";
void setup() {
  
  Serial.begin(115200);
  while (!Serial); //If just the the basic function, must connect to a computer
  
  SPI.begin(5,19,27,18);
  LoRa.setPins(SS,RST,DI0);
//  Serial.println("LoRa Sender");

  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("LoRa Initial OK!");

  // LDC Start //
  // initialize the LCD
  lcd.begin();
  lcd.backlight();
  /// LDC LOGO START///
  lcd.setCursor(0, 0);
  lcd.print("....................");
  lcd.setCursor(0, 1);
  lcd.print("   LXS BEETHOVEN    ");
  lcd.setCursor(0, 2);
  lcd.print("  RFID TRANSITION   ");
  lcd.setCursor(0, 3);
  lcd.print("....................");
  /// LCD LOGO END ///
  // LDC End //
  
}

void loop() {
  Serial.print("Sending packet: ");
  Serial.println(counter);

  // send packet
  LoRa.beginPacket();
  LoRa.print(stuId);
  LoRa.print("|");
  LoRa.print(counter);
  LoRa.endPacket();

  counter++;                      // wait for a second
  
  delay(3000);
}
