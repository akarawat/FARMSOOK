
#include <SoftwareSerial.h>
#include <String.h>

SoftwareSerial mySerial(2, 3);


int ledPin = 6;
int inputPin = 7;


void setup()
{
  mySerial.begin(19200);               // the GPRS baud rate
  Serial.begin(19200);    // the GPRS baud rate
  delay(500);

  //PIR
  pinMode(ledPin, OUTPUT);
  pinMode(inputPin, INPUT);
}

void loop()
{
  //PIR
  int value = digitalRead(inputPin);
  if (value == HIGH)
  {
    digitalWrite(ledPin, HIGH);
    SubmitHttpRequest();
    if (mySerial.available())
      Serial.write(mySerial.read());
  }

  else
  {
    digitalWrite(ledPin, LOW);
  }


}

void SubmitHttpRequest()
{
  mySerial.println("AT+CSQ");
  delay(100);

  ShowSerialData();// this code is to show the data from gprs shield, in order to easily see the process of how the gprs shield submit a http request, and the following is for this purpose too.

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

  //http://gps.scsthai.com/?m=0959530130,100.1234,144.2345,10.56,300
  //http://gps.scsthai.com/pir.php?m=0959530130
  mySerial.println("AT+HTTPPARA=\"URL\",\"gps.scsthai.com/pir.php?m=0959530130\"");// setting the httppara, the second parameter is the website you want to access
  delay(1000);

  ShowSerialData();

  mySerial.println("AT+HTTPACTION=0");//submit the request
  delay(10000);//the delay is very important, the delay time is base on the return from the website, if the return datas are very large, the time required longer.
  //while(!mySerial.available());

  ShowSerialData();

  mySerial.println("AT+HTTPREAD");// read the data from the website you access
  delay(300);

  ShowSerialData();

  mySerial.println("");
  delay(100);
}

void ShowSerialData()
{
  while (mySerial.available() != 0)
    Serial.write(mySerial.read());
}
