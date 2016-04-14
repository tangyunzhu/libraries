/*
SIM800 Serial Debug 

This sketch is used to communicate with SIM800 with AT commands.

create on 2013/12/5, version: 0.1
by lawliet.zou(lawliet.zou@gmail.com)
*/

#include <sim800.h>
#include <SoftwareSerial.h>

SIM800 sim800(8,7,9,9600);//TX,RX,PWR,BaudRate

void setup(){
  Serial.begin(9600);
  sim800.preInit();//Power on SIM800
  Serial.println("SIM800 power on.\r\n Init OK!");
}

void loop(){
  if(sim800.serialSIM800.available()){
    Serial.write(sim800.serialSIM800.read());
  }
  if(Serial.available()){     
    sim800.serialSIM800.write(Serial.read()); 
  }
}



