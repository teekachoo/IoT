//Project 1
#include <SoftwareSerial.h>
//#include <ESP8266WiFi.h>
//#include <ESP8266WiFiMulti.h>

#define DEBUG true

SoftwareSerial esp(10,9); //RX=10, TX=9

String transmit(String esp_command, const int timeout, bool debug){
  //declare a string and char variable that will be used to receive each byte of data
 String comm;
 char x;
 esp.print(esp_command);
 long int time = millis();

 while((time+timeout) > millis()){
  while (esp.available() > 0){
      //Serial.print("errordsadsa");
    x = esp.read(); //read next char
    comm += x;
  }
 }

 //Error check
 if(debug){
  Serial.print(comm);
 }

 return comm;
 
}


void setup(){
  Serial.begin(115200);
  esp.begin(115200);

  transmit("AT+RST\r\n", 2000, DEBUG); //reset module
  transmit("AT+CWMODE=2\r\n", 1000, DEBUG); //configre ESP8266 as an AP
  transmit("AT+CIFSR\r\n", 1000, DEBUG); //get ip address
  transmit("AT+CIPMUX=1\r\n", 1000, DEBUG); //config esp8266 for mult connection
  transmit("AT+CIPSERVER=1,80\r\n", 1000, DEBUG); //turn on server on port 80
}


void loop(){
  if(esp.available()){
    if(esp.find("+IPD,")){
      delay(1000);
      int id = esp.read()-48;

      String webpage = "<h1 style=""text-align:center;""k0y><mark>EGCP-565 Project-1</mark></h1>";
      String cipsend = "AT+CIPSEND=";
      cipsend += id;
      cipsend += ",";
      cipsend += webpage.length();
      cipsend += "\r\n";

      //send the comm then send webpage
      transmit(cipsend, 1000, DEBUG);
      transmit(webpage, 1000, DEBUG); 
      
    /*  webpage = "<html>";*/
/*
      cipsend = "AT + CIPSEND = ";
      cipsend += id;
      cipsend += ",";
      cipsend += String(webpage.length());
      cipsend += "\r\n";

      //send the comm then send webpage
      transmit(cipsend, 1000, DEBUG);
      transmit(webpage, 1000, DEBUG); 
      */
     /* webpage = "<head><h1 style=""text-align:center;""k0y><mark>EGCP-565 Project-1</mark></h1></head>";
      
      cipsend = "AT + CIPSEND = ";
      cipsend += id;
      cipsend += ",";
      cipsend +=webpage.length();
      cipsend += "\r\n";

      //send the comm then send webpage
      transmit(cipsend, 1000, DEBUG);
      transmit(webpage, 1000, DEBUG); */
      
      webpage = "<body><h3>Team Members:</h3></body>";
      cipsend = "AT+CIPSEND=";
      cipsend += id;
      cipsend += ",";
      cipsend +=webpage.length();
      cipsend += "\r\n";

      //send the comm then send webpage
      transmit(cipsend, 1000, DEBUG);
      transmit(webpage, 1000, DEBUG); 
       
      
      webpage = "<ul><li>Hy Do</li><li>Vu Truong</li><li>Riken Parekh</li></ul>";
      cipsend = "AT+CIPSEND=";
      cipsend += id;
      cipsend += ",";
      cipsend += webpage.length();
      cipsend += "\r\n";

      //send the comm then send webpage
      transmit(cipsend, 1000, DEBUG);
      transmit(webpage, 1000, DEBUG); 



      String cipclose = "AT+CIPCLOSE=";
      cipclose += id;
      cipclose += "\r\n";

      transmit(webpage, 3000, DEBUG);
    }
  }
}







