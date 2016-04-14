/*
 * File name  : HP20x_dev.h
 * Description: Driver for I2C PRECISION BAROMETER AND ALTIMETER [HP206C]
 * Author     : Oliver Wang from Seeed studio
 * Version    : V0.1
 * Create Time: 2014/04
 * Change Log :
*/
#ifndef _HP20X_DEV_H
#define _HP20X_DEV_H
/****************************************************************************/
/***        Including Files                                               ***/
/****************************************************************************/
//#include <Wire.h>
#include <SoftwareI2C.h>
#include <Arduino.h>
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
//typedef unsigned int    unsigned int;
//typedef unsigned char   unsigned char;
//typedef unsigned long   unsigned long;

#define HP20X_I2C_DEV_ID       (0xEC)>>1    //CSB PIN is VDD level(address is 0x76)
#define HP20X_I2C_DEV_ID2      (0XEE)>>1    //CSB PIN is GND level(address is 0x77)
#define HP20X_SOFT_RST         0x06
#define HP20X_WR_CONVERT_CMD   0x40
#define HP20X_CONVERT_OSR4096  0<<2
#define HP20X_CONVERT_OSR2048  1<<2
#define HP20X_CONVERT_OSR1024  2<<2
#define HP20X_CONVERT_OSR512   3<<2
#define HP20X_CONVERT_OSR256   4<<2
#define HP20X_CONVERT_OSR128   5<<2

#define HP20X_READ_P           0x30   //read_p command
#define HP20X_READ_A           0x31   //read_a command
#define HP20X_READ_T           0x32   //read_t command
#define HP20X_READ_PT          0x10   //read_pt command
#define HP20X_READ_AT          0x11   //read_at command
#define HP20X_READ_CAL		   0X28	  //RE-CAL ANALOG

#define HP20X_WR_REG_MODE      0xC0
#define HP20X_RD_REG_MODE      0x80

#define ERR_WR_DEVID_NACK       0x01    
#define ERR_RD_DEVID_NACK       0x02    
#define ERR_WR_REGADD_NACK      0x04   
#define ERR_WR_REGCMD_NACK      0x08   
#define ERR_WR_DATA_NACK        0x10     
#define ERR_RD_DATA_MISMATCH    0x20 

#define I2C_DID_WR_MASK         0xFE
#define I2C_DID_RD_MASK         0x01

#define T_WIN_EN                0X01
#define PA_WIN_EN               0X02
#define T_TRAV_EN               0X04
#define PA_TRAV_EN              0X08
#define PA_RDY_EN               0X20
#define T_RDY_EN                0X10

#define T_WIN_CFG               0X01
#define PA_WIN_CFG              0X02
#define PA_MODE_P               0X00
#define PA_MODE_A               0X40

#define T_TRAV_CFG              0X04

#define OK_HP20X_DEV            0X80		//HP20x_dev successfully initialized
#define REG_PARA                0X0F        //Status register

/****************************************************************************/
/***        Class Definitions                                             ***/
/****************************************************************************/
class HP20x_dev : public SoftwareI2C
{
  /* Public variables and functions */
  public:
    unsigned char OSR_CFG;
	unsigned int  OSR_ConvertTime;
    /* Constructor */
    HP20x_dev();	
	void begin();
	unsigned char isAvailable();
	
	/* Read sensor data */
	unsigned long ReadTemperature(void);
	unsigned long ReadPressure(void);
	unsigned long ReadAltitude(void);
	
  /* Private variables and functions */
  private:
    /* Write a command to HP20x */
	void HP20X_IIC_WriteCmd(unsigned char uCmd);	
	/* Read register value */
	unsigned char HP20X_IIC_ReadReg(unsigned char bReg);	
	void HP20X_IIC_WriteReg(unsigned char bReg,unsigned char bData);	 	
	unsigned long HP20X_IIC_ReadData(void);
	unsigned long HP20X_IIC_ReadData3byte(void);
};
extern HP20x_dev HP20x;
#endif