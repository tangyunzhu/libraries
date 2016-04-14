/*
 * File name  : HP20x_dev.cpp
 * Description: Driver for I2C PRECISION BAROMETER AND ALTIMETER [HP206C]
 * Author     : Oliver Wang from Seeed studio
 * Version    : V0.1
 * Create Time: 2014/04
 * Change Log :
*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "HP20x_dev.h"
//#include <wire.h>
#include <SoftwareI2C.h>
#include <Arduino.h>
#include "KalmanFilter.h"
/****************************************************************************/
/***       Local Variable                                                 ***/
/****************************************************************************/
 HP20x_dev HP20x;


/****************************************************************************/
/***       Class member Functions                                         ***/
/****************************************************************************/
/*
 **@ Function name: HP20x_dev
 **@ Description: Constructor
 **@ Input: none
 **@ OutPut: none
 **@ Retval: none
*/
HP20x_dev::HP20x_dev()
{
    OSR_CFG = HP20X_CONVERT_OSR1024;
	//OSR_CFG	= HP20X_CONVERT_OSR256;
	//OSR_CFG	= HP20X_CONVERT_OSR128;
    OSR_ConvertTime = 25; 
}

/*
 **@ Function name: begin
 **@ Description: Initialize HP20x_dev
 **@ Input: none
 **@ OutPut: none
 **@ Retval: none
*/
void HP20x_dev::begin()
{
  //wire.begin();
  //wire.SoftI2C_selectChannel();
  /* Reset HP20x_dev */
  HP20x.HP20X_IIC_WriteCmd(HP20X_SOFT_RST);
}

/*
 **@ Function name: isAvailable
 **@ Description: Indicate whether it's available
 **@ Input: none
 **@ OutPut: none
 **@ Retval: unsigned char 
*/
unsigned char HP20x_dev::isAvailable()
{
  unsigned char ret = HP20x.HP20X_IIC_ReadReg(REG_PARA);
  return ret;
}
/*
 **@ Function name: ReadTemperature
 **@ Description: Read Temperature from HP20x_dev
 **@ Input:
 **@ OutPut:
 **@ Retval:
*/
unsigned long HP20x_dev::ReadTemperature(void)
{
    unsigned char Temp;
    unsigned char Temp0;
     
	HP20X_IIC_WriteCmd(HP20X_WR_CONVERT_CMD|OSR_CFG);	//ADC convert
 
    delay(OSR_ConvertTime);			                    //difference OSR_CFG will be difference OSR_ConvertTime
    HP20X_IIC_WriteCmd(HP20X_READ_T);      
    unsigned long Temperature = HP20X_IIC_ReadData();
    return Temperature;		
}

/*
 **@ Function name: ReadPressure
 **@ Description: Read Pressure value
 **@ Input:
 **@ OutPut: 
 **@ Retval: value
*/
 
unsigned long HP20x_dev::ReadPressure(void)
{
    HP20X_IIC_WriteCmd(HP20X_WR_CONVERT_CMD|OSR_CFG);
    delay(OSR_ConvertTime);
    HP20X_IIC_WriteCmd(HP20X_READ_P);
    unsigned long Pressure = HP20X_IIC_ReadData();             
    return Pressure;
} 

/*
 **@ Function name: ReadAltitude
 **@ Description: Read Pressure value
 **@ Input:
 **@ OutPut: 
 **@ Retval: value
*/
unsigned long HP20x_dev::ReadAltitude(void)
{
    HP20X_IIC_WriteCmd(HP20X_READ_A);
    unsigned long Altitude = HP20X_IIC_ReadData();   
    return Altitude;		
} 
 
/*
void ReadPressureAndTemperature(void)
{
        HP20X_IIC_WriteCmd(HP20X_WR_CONVERT_CMD|OSR_CFG);
        Timer_Delayxms(OSR_ConvertTime*2);
        HP20X_IIC_WriteCmd(HP20X_READ_PT);
        
        Temperature=HP20X_IIC_ReadData();
       
        Pressure=HP20X_IIC_ReadData3byte();       
}

void IIC_ReadAltitudeAndTemperature(void)
{

       HP20X_IIC_WriteCmd(HP20X_WR_CONVERT_CMD|OSR_CFG);
       Timer_Delayxms(OSR_ConvertTime*2);
       HP20X_IIC_WriteCmd(HP20X_READ_AT);
        
        Temperature=HP20X_IIC_ReadData();
        IIC_ACK();
        Altitude=HP20X_IIC_ReadData3byte();
        IIC_NoAck();      
        IIC_Stop();  
                   
}*/
/****************************************************************************/
/***       Local Functions                                                ***/
/****************************************************************************/

/*
 **@ Function name: HP20X_IIC_WriteCmd
 **@ Description:
 **@ Input:
 **@ OutPut:
 **@ Retval:
*/
void HP20x_dev::HP20X_IIC_WriteCmd(unsigned char uCmd)
{		
	/* Port to arduino */
	wire.beginTransmission(HP20X_I2C_DEV_ID);
	wire.write(uCmd);
	wire.endTransmission();
}

/*
 **@ Function name: HP20X_IIC_ReadReg
 **@ Description:
 **@ Input:
 **@ OutPut:
 **@ Retval:  
*/
unsigned char HP20x_dev::HP20X_IIC_ReadReg(unsigned char bReg)
{
    /* Port to arduino */
    unsigned char Temp = 0;
	
	/* Send a register reading command */
    HP20X_IIC_WriteCmd(bReg|HP20X_RD_REG_MODE);	
	 
	wire.requestFrom(HP20X_I2C_DEV_ID, 1);	 
//	while(wire.available())
//	{
	    Temp = wire.read();
//	}
	 
	return Temp;
} 
/*
 **@ Function name: HP20X_IIC_WriteReg
 **@ Description:
 **@ Input:
 **@ OutPut:
 **@ Retval:
*/
void HP20x_dev::HP20X_IIC_WriteReg(unsigned char bReg,unsigned char bData)
{       
	wire.beginTransmission(HP20X_I2C_DEV_ID);
	wire.write(bReg|HP20X_WR_REG_MODE);
	wire.write(bData);
	wire.endTransmission();
}


/*
 **@ Function name: HP20X_IIC_ReadData
 **@ Description:
 **@ Input:
 **@ OutPut:
 **@ Retval:
*/
unsigned long HP20x_dev::HP20X_IIC_ReadData(void)
{                        
	/* Port to arduino */	 
	unsigned long Temp = HP20X_IIC_ReadData3byte(); 
	return Temp;
}

/*
 **@ Function name: HP20X_IIC_ReadData3byte
 **@ Description:
 **@ Input:
 **@ OutPut:
 **@ Retval:
*/
unsigned long HP20x_dev::HP20X_IIC_ReadData3byte(void)
{	
	unsigned long TempData = 0;
	unsigned long tmpArray[3]={0};
	int cnt = 0;
	
	/* Require three bytes from slave */
	wire.requestFrom(HP20X_I2C_DEV_ID, 3);      
/*
    while(wire.available())     // slave may send less than requested
    { 
      unsigned char c = wire.read();    // receive a byte as character	  	  
	  tmpArray[cnt] = (unsigned long)c;
	  cnt++;
	}
*/
	int i = 0;
	unsigned char c = 0;
	for(i=0;i<3;i++)
	{
      	c = wire.read();
		tmpArray[i] = (unsigned long)c;		
	}
	/* MSB */
	TempData = tmpArray[0]<<16 | tmpArray[1]<<8 | tmpArray[2];

	
    if(TempData&0x800000)
    {
	    TempData|=0xff000000;
	}

 /* 	// 24 bit to 32 bit 
	if(TempData&0x800000)
	{
	  // 1:minus 
	  TempData |= 0x80000000;
	  TempData &= 0xff7fffff;
	}
	else
	{
	  // 0:plus 
	  //do noting
	}  */
	return TempData;
} 


/**************************************END OF FILE**************************************/