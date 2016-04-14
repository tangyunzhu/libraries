/*****************************************************************************/
//	Function:    Get the accelemeter of the x/y/z axis. 
//  Hardware:    Grove - 3-Axis Digital Accelerometer(±1.5g)
 

#include <Wire.h>
#include "MMA7660.h"
MMA7660 accelemeter;
#include <SoftwareSerial.h>

SoftwareSerial SoftSerial(7, 6);
int result[6]={0};
int led=1;
void setup()
{
accelemeter.init();  
Serial.begin(9600);
SoftSerial.begin(9600);     
pinMode(8,OUTPUT);
pinMode(9,OUTPUT);
digitalWrite(8,LOW);
digitalWrite(9,LOW);
}
void loop()
{
	int8_t x;
	int8_t y;
	int8_t z;
	 float ax[5],ay[5],az[5];
	//accelemeter.getAcclemeter(&ax,&ay,&az);


 for(int i=0;i<5;i++)
 {
   accelemeter.getAcclemeter(&ax[i],&ay[i],&az[i]);
   SoftSerial.print("az=");
   SoftSerial.println(az[i]);
 
   SoftSerial.print("ax=");
   SoftSerial.println(ax[i]);
  
   SoftSerial.print("ay=");
   SoftSerial.println(ay[i]);
    SoftSerial.println("*************");
	
///////////////////////
if((az[i]<1.2)&&(az[i]>0.8))
{
  result[0]++;
}
else if((az[i]>(-1.2))&&(az[i]<(-0.8)))
{
  result[1]++;
}
//////////////////////////
else if((ax[i]<1.2)&&(ax[i]>0.8))
{
result[2]++;
}
else if((ax[i]>-1.2)&&(ax[i]<-0.8))
{
result[3]++;
}
/////////////////////////
else if((ay[i]<1.2)&&(ay[i]>0.8))
{
result[4]++;
}
else if((ay[i]>-1.2)&&(ay[i]<-0.8))
{
result[5]++;
}
delay(50);

 }
 
 for(int i=0;i<6;i++)
 {
 if(result[i]<5)
 {
 result[i]=0;
 }

 else
 {
 result[i]=5;
 }
 }
 
 if(result[0]+result[1]+result[2]+result[3]+result[4]+result[5]==30)
 {
   while(1)
   {
    SoftSerial.println("ok");
   digitalWrite(9,HIGH);
   digitalWrite(8,LOW);
    delay(100);
 }
 }
 
 SoftSerial.flush();
  
  digitalWrite(8,led);
  led=!led;
}


