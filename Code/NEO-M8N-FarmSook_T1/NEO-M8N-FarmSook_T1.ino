#include "TinyGPS++.h"
#include "SoftwareSerial.h"

SoftwareSerial SerGPS(10, 11); //RX=pin 10, TX=pin 11
TinyGPSPlus gps;

//// SIM  Start///
#include <String.h>
SoftwareSerial mySerial(2, 3);
//// SIM  End///

void setup()
{
  // GPS
  mySerial.begin(9600);
  Serial.begin(9600);
  SerGPS.begin(9600);
  Serial.println("GPS Start");

  ///SIM START
  mySerial.println("AT+CSQ");
  delay(100);
  ShowSerialData();

  mySerial.println("AT+CGATT?");
  delay(100);
  ShowSerialData();

  mySerial.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");//setting the SAPBR, the connection type is using gprs
  delay(1000);
  ShowSerialData();

  mySerial.println("AT+SAPBR=3,1,\"APN\",\"CMNET\"");//setting the APN, the second need you fill in your local apn server
  delay(4000);
  ShowSerialData();

  mySerial.println("AT+SAPBR=1,1");//setting the SAPBR, for detail you can refer to the AT command mamual
  delay(2000);
  ShowSerialData();

  mySerial.println("AT+HTTPINIT"); //init the HTTP request
  delay(2000);
  ShowSerialData();

  mySerial.println("AT+HTTPPARA=\"URL\",\"gps.scsthai.com\"");
  //mySerial.println("AT+HTTPPARA=\"URL\",\"gps.scsthai.com/?m=0959530130,100.1234,144.2345\"");
  delay(1000);
  ShowSerialData();
  mySerial.println("AT+HTTPACTION=0");//submit the request
  delay(10000);
  ShowSerialData();
  delay(100);
  ///SIM END
}


void loop()
{
  //Serial.println("Start Loop");
  //http://gps.scsthai.com/?m=0959530130,100.1234,144.2345,10.56,320
  //mySerial.println("AT+HTTPPARA=\"URL\",\"gps.scsthai.com/?m=0959530130,14.2066862,100.6027656\"");
  mySerial.println("AT+HTTPPARA=\"URL\",\"gps.scsthai.com/?m=0959530130,100.1234,144.2345,10.56,399\"");
  delay(1000);
  ShowSerialData();
  mySerial.println("AT+HTTPACTION=0");//submit the request
  delay(10000);
  ShowSerialData();
  delay(5000);

}

void ShowSerialData()
{
  //while(mySerial.available()!=0)
  //Serial.write(mySerial.read());
}
