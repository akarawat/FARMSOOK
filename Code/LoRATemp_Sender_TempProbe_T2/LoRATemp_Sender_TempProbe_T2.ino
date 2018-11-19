// Farm sook Tommy Test by TTGO_ESP32_LORA SX1278
// Upload by ESP32_Dev_Module
// Use Temp Probe >> Pin 4

#include <SPI.h>
#include <LoRa.h>

// Probe Start
#include <OneWire.h>
#include <DallasTemperature.h>
// Probe Stop

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

// Probe Start
float tprobe = 0;
#define ONE_WIRE_BUS 4
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
// Probe Stop

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

  // Probe Start
  sensors.begin();
  // Probe Stop
}

String txtTProbe = "";
void loop() {
  // Probe Start
  sensors.requestTemperatures();
  tprobe = sensors.getTempCByIndex(0);
  txtTProbe = String(tprobe);
  // Probe Stop
  Serial.print("Sending packet: ");
  Serial.println(txtTProbe);

  // send packet
  LoRa.beginPacket();
  LoRa.print(stuId);
  LoRa.print("|");
  LoRa.print(txtTProbe);
  LoRa.endPacket();

  //tprobe++;                      // wait for a second
  
  delay(5000);
  
}
