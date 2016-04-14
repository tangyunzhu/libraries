/*
 * File name  : TH02_dev.h
 * Description: Driver for DIGITAL I2C HUMIDITY AND TEMPERATURE SENSOR
 * Author     : Oliver Wang from Seeed studio
 * Version    : V0.1
 * Create Time: 2014/04
 * Change Log :
*/
#ifndef _TH02_DEV_H
#define _TH02_DEV_H

/****************************************************************************/
/***        Including Files                                               ***/
/****************************************************************************/
#include <SoftwareI2C.h>
#include <Arduino.h>

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define TH02_I2C_DEV_ID      0x40
#define REG_STATUS           0x00 
#define REG_DATA_H           0x01
#define REG_DATA_L           0x02
#define REG_CONFIG           0x03
#define REG_ID               0x11

#define STATUS_RDY_MASK      0x01    //poll RDY,0 indicate the conversion is done

#define CMD_MEASURE_HUMI     0x01    //perform a humility measurement
#define CMD_MEASURE_TEMP     0x11    //perform a temperature measurement

#define TH02_WR_REG_MODE      0xC0
#define TH02_RD_REG_MODE      0x80
/****************************************************************************/
/***        Class Definition                                              ***/
/****************************************************************************/
class TH02_dev : public SoftwareI2C
{
    public:
	//TH02_dev();
	unsigned char isAvailable();
	float ReadTemperature(void);
	float ReadHumidity(void);
	private:
	void TH02_IIC_WriteCmd(unsigned char u8Cmd);
	unsigned char TH02_IIC_ReadReg(unsigned char u8Reg);
	void TH02_IIC_WriteReg(unsigned char u8Reg,unsigned char u8Data);
	unsigned int TH02_IIC_ReadData(void);
	unsigned int TH02_IIC_ReadData2byte(void);
};
extern TH02_dev TH02;

#endif  // _TH02_DEV_H