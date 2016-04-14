#include <Arduino.h>
#include <SoftwareSerial.h>
#include "WizFi250.h"

#define SSID      "STEST"
#define KEY       "87654321"
#define AUTH       "WPA2"

#define TSN_HOST_IP        "115.239.210.26" 
//#define TSN_HOST_IP      "192.168.1.254"      // broadcast
#define TSN_REMOTE_PORT    80
#define LOCAL_PORT     9000

#define spi_CS  8

SoftwareSerial sprintSerial(4,5);
WizFi250 wizfi250(&Serial);
void setup() {
 
sprintSerial.begin(9600);
Serial.begin(9600);
pinMode(spi_CS,OUTPUT);
digitalWrite(spi_CS,HIGH);
sprintSerial.println("--------- WIZFI250 TEST --------");
// wait for initilization of Wizfi250
  delay(1000);
  sprintSerial.println("Join " SSID );
  delay(10);
  if (!wizfi250.join(SSID, KEY, AUTH)) {
    sprintSerial.println("Failed join " SSID); 
  } else {
    sprintSerial.println("Successfully join  "  SSID);
 
  wizfi250.clear();
  
  wizfi250.connect(TSN_HOST_IP,TSN_REMOTE_PORT,LOCAL_PORT);
  delay(10);
  wizfi250.send("GET / HTTP/1.1\r\n\r\n");
}
   char c; 
   for(int i=0;i<320;i++){
   if (wizfi250.receive((uint8_t *)&c, 1, 100) > 0) {
    sprintSerial.print((char)c);
   }
   } 
}
void loop() {
   while (wizfi250.available()) {
    sprintSerial.write(wizfi250.read());
  }
  while (sprintSerial.available()) {
    wizfi250.write(sprintSerial.read());
}
}