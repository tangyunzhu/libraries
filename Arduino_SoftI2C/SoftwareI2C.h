/*
  SoftwareI2C.h
  2012 Copyright (c) Seeed Technology Inc.  All right reserved.

  Author:Loovee
  2013-11-1

  This is a Software I2C Library, can act as I2c master mode.
  
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef _SOFTWAREI2C_H_
#define _SOFTWAREI2C_H_
////////////////////
#define	G1_0_SCL				22
#define	G1_1_SDA				23
#define	G2_0_SCL				24
#define	G2_1_SDA				25f
#define	G3_0_SCL				26
#define	G3_1_SDA				27
#define	G4_0_SCL				28
#define	G4_1_SDA				29							

 
#define	G5_0_SCL				30
#define	G5_1_SDA				31
#define	G6_0_SCL				32
#define	G6_1_SDA				33
#define	G7_0_SCL				34
#define	G7_1_SDA				35
#define	G8_0_SCL				36
#define	G8_1_SDA				37				

 
#define	G9_0_SCL				38
#define	G9_1_SDA				39
#define	G10_0_SCL				40
#define	G10_1_SDA				41
////////////////////////

#define DELAY i2c_delay

#define __Debug        1                      // debug mode


#define  GETACK        1                      // get ack                        
#define  GETNAK        0                      // get nak   
   
#ifndef  HIGH        
#define  HIGH          1
#endif
#ifndef  LOW
#define  LOW           0
#endif

#define uchar unsigned char

class SoftwareI2C
{
private:
    
    int pinSda;
    int pinScl;
    
    int recv_len;
    
private:
    
    void sdaSet(uchar ucDta); 
    void sclSet(uchar ucDta);                             
    uchar sdaGet(void);                                        
    void i2c_delay(void);          

    void sendStart(void);
    void sendStop(void);
    uchar getAck(void);
    void sendAck(void);
    void sendNak(void);
    
    void sendBit(uchar bit);
    uchar revBit();
    
    void sendByte(uchar ucDta);
    
    uchar sendByteAck(uchar ucDta);                                 // send byte and get ack
    
    uchar revByte();                                                // receive a byte
    
public:

    void SoftI2C_selectChannel(unsigned char i2c_channel);
    void begin(int Sda, int Scl); 
    uchar beginTransmission(uchar addr);
    void endTransmission();
    
    uchar write(uchar dta);
    uchar write(uchar len, uchar *dta);
    uchar requestFrom(uchar addr, uchar len);
    uchar read();

};

extern SoftwareI2C wire;
#endif
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
