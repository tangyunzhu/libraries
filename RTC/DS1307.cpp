/****************************************************************************/	
//	Function: Cpp file for DS1307
//	Hardware: Grove - RTC
//	Arduino IDE: Arduino-1.0
//	Author:	 FrankieChu		
//	Date: 	 Jan 19,2013
//	Version: v1.0
//	by www.seeedstudio.com
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
/****************************************************************************/

//#include <wire.h>
#include <SoftwareI2C.h>
#include "DS1307.h"

uint8_t DS1307::decToBcd(uint8_t val)
{
	return ( (val/10*16) + (val%10) );
}

//Convert binary coded decimal to normal decimal numbers
uint8_t DS1307::bcdToDec(uint8_t val)
{
	return ( (val/16*10) + (val%16) );
}

// void DS1307::begin()
// {
	// wire.begin();
// }
/*Function: The clock timing will start */
void DS1307::startClock(void)        // set the ClockHalt bit low to start the rtc
{
  wire.beginTransmission(DS1307_I2C_ADDRESS);
  wire.write((uint8_t)0x00);                      // Register 0x00 holds the oscillator start/stop bit
  wire.endTransmission();
  wire.requestFrom(DS1307_I2C_ADDRESS, 1);
  second = wire.read() & 0x7f;       // save actual seconds and AND sec with bit 7 (sart/stop bit) = clock started
  wire.beginTransmission(DS1307_I2C_ADDRESS);
  wire.write((uint8_t)0x00);
  wire.write((uint8_t)second);                    // write seconds back and start the clock
  wire.endTransmission();
}
/*Function: The clock timing will stop */
void DS1307::stopClock(void)         // set the ClockHalt bit high to stop the rtc
{
  wire.beginTransmission(DS1307_I2C_ADDRESS);
  wire.write((uint8_t)0x00);                      // Register 0x00 holds the oscillator start/stop bit
  wire.endTransmission();
  wire.requestFrom(DS1307_I2C_ADDRESS, 1);
  second = wire.read() | 0x80;       // save actual seconds and OR sec with bit 7 (sart/stop bit) = clock stopped
  wire.beginTransmission(DS1307_I2C_ADDRESS);
  wire.write((uint8_t)0x00);
  wire.write((uint8_t)second);                    // write seconds back and stop the clock
  wire.endTransmission();
}
/****************************************************************/
/*Function: Read time and date from RTC	*/
void DS1307::getTime()
{
    // Reset the register pointer
	wire.beginTransmission(DS1307_I2C_ADDRESS);
	wire.write((uint8_t)0x00);
	wire.endTransmission();  
	wire.requestFrom(DS1307_I2C_ADDRESS, 7);
	// A few of these need masks because certain bits are control bits
	second	   = bcdToDec(wire.read() & 0x7f);
	minute	   = bcdToDec(wire.read());
	hour	   = bcdToDec(wire.read() & 0x3f);// Need to change this if 12 hour am/pm
	dayOfWeek  = bcdToDec(wire.read());
	dayOfMonth = bcdToDec(wire.read());
	month      = bcdToDec(wire.read());
	year	   = bcdToDec(wire.read());
}
/*******************************************************************/
/*Frunction: Write the time that includes the date to the RTC chip */
void DS1307::setTime()
{
	wire.beginTransmission(DS1307_I2C_ADDRESS);
	wire.write((uint8_t)0x00);
	wire.write(decToBcd(second));// 0 to bit 7 starts the clock
	wire.write(decToBcd(minute));
	wire.write(decToBcd(hour));  // If you want 12 hour am/pm you need to set bit 6 
	wire.write(decToBcd(dayOfWeek));
	wire.write(decToBcd(dayOfMonth));
	wire.write(decToBcd(month));
	wire.write(decToBcd(year));
	wire.endTransmission();
}
void DS1307::fillByHMS(uint8_t _hour, uint8_t _minute, uint8_t _second)
{
	// assign variables
	hour = _hour;
	minute = _minute;
	second = _second;
}
void DS1307::fillByYMD(uint16_t _year, uint8_t _month, uint8_t _day)
{
	year = _year-2000;
	month = _month;
	dayOfMonth = _day;
}
void DS1307::fillDayOfWeek(uint8_t _dow)
{
	dayOfWeek = _dow;
}

