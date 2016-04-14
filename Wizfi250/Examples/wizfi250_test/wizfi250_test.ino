#include <Arduino.h>
#include <SoftwareSerial.h>
#include "WizFi250.h"

#define SSID      "STEST"
#define KEY       "87654321"
#define AUTH       "WPA2" 

#define spi_CS  8

SoftwareSerial sprintSerial(2,3);
WizFi250 wizfi250(&Serial);

void setup() {
//uart.begin(9600);
sprintSerial.begin(9600);
Serial.begin(9600);
pinMode(spi_CS,OUTPUT);
digitalWrite(spi_CS,HIGH);
sprintSerial.println("--------- WIZFI250 TEST --------");
// wait for initilization of Wizfi250
// wizfi250.reset();
  delay(1000);
  Serial.println("Join " SSID );
  delay(10);
  if (wizfi250.join(SSID, KEY, AUTH)) {
   sprintSerial.println("Successfully join  "  SSID);
  } else {
    sprintSerial.println("Failed join " SSID);
  }
  wizfi250.clear();
  sprintSerial.println("*************send command to get Wizfi250 status******************");
  wizfi250.sendCommand("AT+WSTAT\r");
  //delay(10);
  char c;
  while (wizfi250.receive((uint8_t *)&c, 1, 100) > 0) {
   sprintSerial.print((char)c);
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