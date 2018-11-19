// Farm sook Tommy Test by TTGO_ESP32_LORA SX1278
// Upload by ESP32_Dev_Module
// SDA = LoRASX1278 ESP32 PIN21
// SCL = LoRASX1278 ESP32 PIN22


#include <SPI.h>
#include <LoRa.h>

#include <LiquidCrystal_I2C.h>
// LDC Start //
//LiquidCrystal_I2C lcd(0x27, 20, 4); // YELLOW
LiquidCrystal_I2C lcd(0x3F, 20, 4); // BLUE
// LDC End //

#include "BluetoothSerial.h"
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
BluetoothSerial SerialBT;

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

  SerialBT.begin("Student_MasterNoA001"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");

  // LDC Start //
  // initialize the LCD
  lcd.begin();
  lcd.backlight();
  /// LDC LOGO START ///
  lcd.setCursor(0, 0);
  lcd.print("....................");
  lcd.setCursor(0, 1);
  lcd.print("   FARM SOOK    ");
  lcd.setCursor(0, 2);
  lcd.print("  HISTORICAL DATA   ");
  lcd.setCursor(0, 3);
  lcd.print("....................");
  /// LCD LOGO END ///
  // LDC End //

}

void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet '");

    // read packet
    //char inData = 'F';
    String incoming = "";                 // payload of packet

    while (LoRa.available()) {            // can't use readString() in callback, so
      incoming += (char)LoRa.read();      // add bytes one by one
    }

    Serial.print("[");
    Serial.print(incoming);
    Serial.print("]");

    // print RSSI of packet
    Serial.println("' with RSSI ");
    Serial.println(LoRa.packetRssi());
    String rssi = String(LoRa.packetRssi());

    String txtBt = incoming + ";" + rssi;
    int len = txtBt.length();
    SerialBT.write((uint8_t *)txtBt.c_str(), len + 1);
    //LCD Start
    // initialize the LCD
    lcd.begin();
    lcd.clear();
    lcd.backlight();
    /// LDC LOGO START ///
    lcd.setCursor(0, 0);
    lcd.print(incoming);
    delay(1500);
    //lcd.clear();
    lcd.noBacklight();
    //LCD End

  }
}

