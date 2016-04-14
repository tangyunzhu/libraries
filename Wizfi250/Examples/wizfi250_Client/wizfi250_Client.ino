#include <Arduino.h>
#include "WizFi250.h"

#define SSID      "STEST"
#define KEY       "87654321"
#define AUTH       "WPA2" 

 
#define  HOST_IP       "192.168.168.102"
#define  REMOTE_PORT    9090
#define LOCAL_PORT      1234
 
#define spi_CS  8
 
WizFi250 wizfi250(&Serial1);
boolean returnValue=false;
void setup() {
 
Serial1.begin(19200);
Serial.begin(19200);
pinMode(spi_CS,OUTPUT);
Serial.println("--------- WIZFI250 TEST --------");
// wait for initilization of Wizfi250
  delay(1000);
  Serial.println("Join " SSID );
  wizfi250.reset();
  delay(1000);
  wizfi250.sendCommand("AT+WLEAVE\r");
   delay(1000);
   if (!wizfi250.join(SSID, KEY, AUTH)) {
    Serial.println("Failed join " SSID);
     Serial.println("Please Restart");
  } else {
    
  Serial.println("Successfully join " SSID);
  wizfi250.sendCommand("AT+WSTAT\r");
  delay(5);
  char c;
  while(wizfi250.receive((uint8_t *)&c, 1, 100) > 0) {
    Serial.print((char)c);
  }
  delay(2000);  
  returnValue=wizfi250.connect(HOST_IP,REMOTE_PORT,LOCAL_PORT);
    Serial.print(returnValue);
}
}
//unsigned int time_point = 0;

void loop() {
  if(wizfi250.available()) {
    Serial.print((char)wizfi250.read());
  }
   if(returnValue){
   delay(500);
    wizfi250.send("OK"); 
   } 
  
}
