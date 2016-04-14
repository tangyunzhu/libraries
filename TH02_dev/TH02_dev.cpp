/*
 * File name  : TH02_dev.cpp
 * Description: Driver for DIGITAL I2C HUMIDITY AND TEMPERATURE SENSOR
 * Author     : Oliver Wang from Seeed studio
 * Version    : V0.1
 * Create Time: 2014/04
 * Change Log :
*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "TH02_dev.h"
#include <Arduino.h>
#include <SoftwareI2C.h>
/* Use Serial IIC */
//#ifdef SERIAL_IIC
//#endif


TH02_dev TH02;
//TH02_dev::TH02_dev()
//{

//}
float TH02_dev::ReadTemperature(void)
{    
    /* Start a new temperature conversion */
	TH02_IIC_WriteReg(REG_CONFIG, CMD_MEASURE_TEMP);	 	 
    delay(100);
	/* Wait until conversion is done */
	//while(!isAvailable());
	uint16_t value = TH02_IIC_ReadData();
	
	value = value >> 2;
	/* 
	  Formula:
      Temperature(C) = (Value/32) - 50	  
	*/	
	float temper = (value/32.0)-50.0;
	
	return temper;
}
 
float TH02_dev::ReadHumidity(void)
{
 /* Start a new humility conversion */
	TH02_IIC_WriteReg(REG_CONFIG, CMD_MEASURE_HUMI);
	
	/* Wait until conversion is done */
	delay(100);
	//while(!isAvailable());
	uint16_t value = TH02_IIC_ReadData();
	
	value = value >> 4;
 
	/* 
	  Formula:
      Humidity(%) = (Value/16) - 24	  
	*/	

	float humility = (value/16.0)-24.0;
	
	return humility;
}
/****************************************************************************/
/***       Local Functions                                                ***/
/****************************************************************************/
uint8_t TH02_dev::isAvailable()
{
    uint8_t status =  TH02_IIC_ReadReg(REG_STATUS);
	if(status & STATUS_RDY_MASK)
	{
	    return 0;    //ready
	}
	else
	{
	    return 1;    //not ready yet
	}
}

void TH02_dev::TH02_IIC_WriteCmd(uint8_t u8Cmd)
{		
	/* Port to arduino */
	wire.beginTransmission(TH02_I2C_DEV_ID);
	wire.write(u8Cmd);
	wire.endTransmission();
}
uint8_t TH02_dev::TH02_IIC_ReadReg(uint8_t u8Reg)
{
    /* Port to arduino */
    uint8_t Temp = 0;
	
	/* Send a register reading command */
    TH02_IIC_WriteCmd(u8Reg);		 
	wire.requestFrom(TH02_I2C_DEV_ID, 1);
    delay(1);	
	Temp = wire.read();		
	return Temp;
} 
/*
 **@ Function name: TH02_IIC_WriteReg
 **@ Description:
 **@ Input:
 **@ OutPut:
 **@ Retval:
*/
void TH02_dev::TH02_IIC_WriteReg(uint8_t u8Reg,uint8_t u8Data)
{           
	wire.beginTransmission(TH02_I2C_DEV_ID);	 
	wire.write(u8Reg);	 
	wire.write(u8Data);	 
	wire.endTransmission();	 
}

uint16_t TH02_dev::TH02_IIC_ReadData(void)
{                        
	/* Port to arduino */	 
	uint16_t Temp = TH02_IIC_ReadData2byte(); 
	return Temp;
}

uint16_t TH02_dev::TH02_IIC_ReadData2byte()
{
    uint16_t TempData = 0;
	uint16_t tmpArray[3]={0};

	//int cnt = 0;
	TH02_IIC_WriteCmd(REG_DATA_H);	
	
	wire.requestFrom(TH02_I2C_DEV_ID, 3);	 
	for(int cnt=0;cnt<3;cnt++){
	tmpArray[cnt] = (uint16_t)wire.read();        	        	
	//cnt++;
	}
	/* MSB */
	TempData = (tmpArray[1]<<8)|(tmpArray[2]); 
	return TempData;
}