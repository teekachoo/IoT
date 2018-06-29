//Project2
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include <DHT.h>

#define DEBUG true
#define DHTTYPE DHT11
#define DHTPIN 7

String transmit(String esp_command, const int timeout, bool debug);

//variables for DHT11 data
float humidity;
float tempCelcius;
float tempFahrenheit;
float heatIndexF;

const int reset = 12, enable = 11, lcdD4 = 5, lcdD5 = 4, lcdD6 = 3, lcdD7 = 2;

//initialize LCD
LiquidCrystal LCDboard(reset, enable, lcdD4, lcdD5, lcdD6, lcdD7);

SoftwareSerial esp(10,9); //Rx is 10, Tx is 9
DHT dht(DHTPIN,DHTTYPE); //initialize DHT sensor


void setup(){
  Serial.begin(115200);
  esp.begin(115200);
  dht.begin();
  LCDboard.begin(16,2); //collumns = 3, rows = 4

  transmit("AT+RST\r\n", 2000, DEBUG); //reset module
  transmit("AT+CWMODE=2\r\n", 1000, DEBUG); //configre ESP8266 as an AP
  transmit("AT+CIFSR\r\n", 1000, DEBUG); //get ip address
  transmit("AT+CIPMUX=1\r\n", 1000, DEBUG); //config esp8266 for mult connection
  transmit("AT+CIPSERVER=1,80\r\n", 1000, DEBUG); //turn on server on port 80
}


void loop() {
  // put your main code here, to run repeatedly:
  delay(2000);
  humidity = dht.readHumidity();
  tempCelcius = dht.readTemperature();
  tempFahrenheit = dht.readTemperature(true);
  heatIndexF = dht.computeHeatIndex(tempFahrenheit,humidity);

  if(esp.available()){
    if(esp.find("+IPD,")){
      delay(1000);
      int id = esp.read()-48;

    //HTML
    String webpage = "<h1 style=""text-align:center;""k0y><mark>EGCP-565 Project-1</mark></h1>";
    String cipsend = "AT+CIPSEND=";
    cipsend += id;
    cipsend += ",";
    cipsend += webpage.length();
    cipsend += "\r\n";

      //send the comm then send webpage
    transmit(cipsend, 1000, DEBUG);
    transmit(webpage, 1000, DEBUG); 

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


      //Display the humidity
      webpage ="<style>table,td{border: 1px solid black;}</style>";
      webpage +="<table><tr><td>Humidity</td><td>";
      webpage+=humidity;
      webpage+="</td></tr>";
      cipsend = "AT+CIPSEND=";
      cipsend += id;
      cipsend += ",";
      cipsend += webpage.length();
      cipsend += "\r\n";
      //send the comm then send webpage
      transmit(cipsend, 1000, DEBUG);
      transmit(webpage, 1000, DEBUG); 


      //Display the temp in Celcius
      webpage = "<tr><td>Temperature in Celcius</td><td>";
      webpage+=tempCelcius;
      webpage+="&#8451;</td></tr>";
      cipsend = "AT+CIPSEND=";
      cipsend += id;
      cipsend += ",";
      cipsend += webpage.length();
      cipsend += "\r\n";
      //send the comm then send webpage
      transmit(cipsend, 1000, DEBUG);
      transmit(webpage, 1000, DEBUG); 

      //Display temp in F
      webpage = "<tr><td>Temperature in F</td><td>";
       webpage+=tempFahrenheit;
      webpage+="&#8457;</td></tr>";
      cipsend = "AT+CIPSEND=";
      cipsend += id;
      cipsend += ",";
      cipsend += webpage.length();
      cipsend += "\r\n";
      //send the comm then send webpage
      transmit(cipsend, 1000, DEBUG);
      transmit(webpage, 1000, DEBUG); 
      
      //Display heat index in F
      webpage = "<tr><td>Heat Index</td><td>";
       webpage+=heatIndexF;
      webpage+="&#8457;</td></tr></table>";
      cipsend = "AT+CIPSEND=";
      cipsend += id;
      cipsend += ",";
      cipsend += webpage.length();
      cipsend += "\r\n";
      //send the comm then send webpage
      transmit(cipsend, 1000, DEBUG);
      transmit(webpage, 1000, DEBUG); 
      
      //refresh every 10 secondns
      webpage="<meta http-equiv=""refresh"" content=""10"">";
      cipsend = "AT+CIPSEND=";
      cipsend += id;
      cipsend += ",";
      cipsend += webpage.length();
      cipsend += "\r\n";
      //send the comm then send webpage
      transmit(cipsend, 1000, DEBUG);
      transmit(webpage, 1000, DEBUG); 

      //Append the connection
      
      String cipclose = "AT+CIPCLOSE=";
      cipclose += id;
      cipclose += "\r\n";
      transmit(webpage, 3000, DEBUG);
      
    }
  }
  //LCD display

//Display humidity onto LCD board
LCDboard.blink();
delay(1000);
LCDboard.noBlink();
LCDboard.print("Humidity");     //display "Humidity" on to LCD screen
delay(1000);
LCDboard.setCursor(0,1);       //move curso to the next line
LCDboard.print("=");      //display humidity - equal sign
LCDboard.print(humidity);                   // value
LCDboard.print("%");                   // percentage
delay(1000);
LCDboard.clear();           //clear the lcd

//Display temperature in celcius onto LCD board
LCDboard.blink();
delay(1000);
LCDboard.noBlink();
LCDboard.print("Temperature: ");     //display "Temperature" on to LCD screen
delay(1000);
LCDboard.setCursor(0,1);     //move curso to the next line
LCDboard.print("=");      //display humidity - equal sign
LCDboard.print(tempCelcius);                   // value
LCDboard.print(char(223));        //degree symbol
LCDboard.print("C") ;                  
delay(1000);
LCDboard.clear();           //clear the lcd

//Display temperature in Fahrenheit onto LCD board
LCDboard.blink();
delay(1000);
LCDboard.noBlink();
LCDboard.print("Temperature: ") ;     //display "Temperature" on to LCD screen
delay(1000);
LCDboard.setCursor(0,1);       //move curso to the next line
LCDboard.print("=") ;      //display humidity - equal sign
LCDboard.print(tempFahrenheit) ;                   // value
LCDboard.print(char(223));      // degree symbol
LCDboard.print("F") ;                   
delay(1000);
LCDboard.clear();           //clear the lcd

//Display temperature in Fahrenheit onto LCD board
LCDboard.blink();
delay(1000);
LCDboard.noBlink();
LCDboard.print("Feels like: ") ;     //display "Feels like" on to LCD screen
delay(1000);
LCDboard.setCursor(0,1);       //move curso to the next line
LCDboard.print("=");      //display humidity - equal sign
LCDboard.print(heatIndexF) ;                   // value
LCDboard.print(char(223)) ;                   // degree symbol
LCDboard.print("F");
delay(1000);
LCDboard.clear();           //clear the lcd



}


  //voila!
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


"# 565_IoT_Project_2" 
