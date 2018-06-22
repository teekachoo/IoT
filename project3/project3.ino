#include <SoftwareSerial.h>
#include <Servo.h>

#define DEBUG true

SoftwareSerial esp8266(2, 3); //Rx=2, Tx=3
Servo servo;
int servoPin = 9;
int servoAngle = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  esp8266.begin(57600);

  servo.attach(servoPin);
  servo.write(servoAngle);

  //LED 1
  pinMode(10, OUTPUT);
  digitalWrite(10, LOW);

  //LED 2
  pinMode(11, OUTPUT);
  digitalWrite(11, LOW);

  //LED 3
  pinMode(12, OUTPUT);
  digitalWrite(12, LOW);

  //LED 4
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);

  sendData("AT+RST\r\n", 2000, DEBUG); //reset module
  sendData("AT+CWMODE=2\r\n", 1000, DEBUG); //configre ESP8266 as an AP
  sendData("AT+CIFSR\r\n", 1000, DEBUG); //get ip address
  sendData("AT+CIPMUX=1\r\n", 1000, DEBUG); //config esp8266 for mult connection
  sendData("AT+CIPSERVER=1,80\r\n", 1000, DEBUG); //turn on server on port 80

  
}

void loop()
{
    if(esp8266.available())
    {
        if(esp8266.find("+IPD,"))
        {
            delay(1000);
            int connectionId = esp8266.read()-48;

            esp8266.find("pin=");

            int pinNumber = (esp8266.read()-48)*10;
            pinNumber += (esp8266.read()-48);

            switch(pinNumber){
              case 10: sweep(45);
                break;
              case 11: sweep(90);
                break;
              case 12: sweep(135);
                break;
              case 13: sweep(180);
                break;
              default:  Serial.print("Pin Number Error");
            }
              digitalWrite(pinNumber, !digitalRead(pinNumber));
            
              String closeCommand="AT+CIPCLOSE=";
              closeCommand += connectionId;
              closeCommand += "\r\n";

              sendData(closeCommand,1000,DEBUG);
        }
    }
}

String sendData(String command, const int timeout, boolean debug)
{
  String comm = "";
  char x;
  esp8266.print(command);
  long int time = millis();

  while((time+timeout)>millis())
  {
    while(esp8266.available()>0)
    {
      x = esp8266.read();
      comm += x;
    }
  }
  // error check
  if(debug)
  {
    Serial.print(comm);   
  }
  return comm;
}

void sweep (int range)
{
  for(int i = 0; i < range; i++)
  {
    servo.write(i);
    delay(50);
  }
  for(int j = range; j >= 0; j--)
  {
    servo.write(j);
    delay(10);
  }
}
