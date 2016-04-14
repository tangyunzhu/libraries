/*
* sim800.h 
* A library for SeeedStudio seeeduino GPRS shield 
*  
* Copyright (c) 2013 seeed technology inc. 
* Author  		: 	lawliet.zou(lawliet.zou@gmail.com)
* Create Time	: 	Dec 23, 2013 
* Change Log 	: 
*
* The MIT License (MIT)
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*/

#ifndef __SIM800_H__
#define __SIM800_H__

#include "Arduino.h"
#include <SoftwareSerial.h>

#define TRUE 					1
#define FALSE 					0

#define SIM800_TX_PIN           8
#define SIM800_RX_PIN           7
#define SIM800_POWER_PIN        9
#define SIM800_BAUDRATE         9600

#define UART_DEBUG

#ifdef UART_DEBUG
#define ERROR(x)			Serial.println(x)
#define DEBUG(x)			Serial.println(x);
#else
#define ERROR(x)
#define DEBUG(x)
#endif

#define DEFAULT_TIMEOUT   	5

enum DataType {
    CMD		= 0,
    DATA	= 1,
};
/** SIM800 class.
 *  Used for SIM800 communication. attention that SIM800 module communicate with MCU in serial protocol
 */
class SIM800
{

public:
	/**	Create SIM800 Instance 
     *  @param tx	uart transmit pin to communicate with SIM800
     *  @param rx	uart receive pin to communicate with SIM800
     *  @param baudRate	baud rate of uart communication
	 */
	SIM800(int tx, int rx, int power, int baudRate):serialSIM800(tx,rx){
		powerPin = power;
        pinMode(power,OUTPUT);
		serialSIM800.begin(baudRate);
	};
	
	/** Power on SIM800
	 */
	void preInit(void);
	
    /** check serialModem is readable or not
     *	@returns
     *		data on readable
     *		false on not readable
     */
    int readable();

    /** read one byte from serialModem
     *	@returns
     *		one byte read from serialModem
     */
    char readByte(void);
	/** read from SIM800 module and save to buffer array
	 *  @param  buffer	buffer array to save what read from SIM800 module
	 *  @param  count 	the maximal bytes number read from SIM800 module
	 *  @param  timeOut	time to wait for reading from SIM800 module 
	 *  @returns
	 *      0 on success
	 *      -1 on error
	 */
	int readBuffer(char* buffer,int count, unsigned int timeOut);

	
	/** clean Buffer
	 *	@param buffer	buffer to clean
	 *	@param count	number of bytes to clean
	 */
	void cleanBuffer(char* buffer, int count);
	
	/** send AT command to SIM800 module
	 *  @param cmd	command array which will be send to GPRS module
	 */
	void sendCmd(const char* cmd);

	/**send "AT" to SIM800 module
	 */
	void sendATTest(void);
	
	/**send '0x1A' to SIM800 Module
	 */
	void sendEndMark(void);
	
	/**	compare the response from GPRS module with a string
     *	@param resp	buffer to be compared
     *	@param len length that will be compared
     *	@param timeout	waiting seconds till timeout
     */
    bool respCmp(const char *resp, unsigned int len, unsigned int timeout);
	
	
	/** check SIM800 module response before time out
	 *  @param  *resp   correct response which SIM800 module will return
	 *  @param  *timeout    waiting seconds till timeout
	 *  @returns
	 *      0 on success
	 *      -1 on error
	 */ 
	int waitForResp(const char* resp, unsigned int timeout, DataType type);

	/** send AT command to GPRS module and wait for correct response
	 *  @param  *cmd 	AT command which will be send to GPRS module
	 *  @param  *resp   correct response which GPRS module will return
	 *  @param  *timeout 	waiting seconds till timeout
	 *  @returns
	 *      0 on success
	 *      -1 on error
	 */
	int sendCmdAndWaitForResp(const char* data, const char *resp, unsigned timeout, DataType type);
    
    int powerPin;
    SoftwareSerial serialSIM800;

private:
    
};

#endif