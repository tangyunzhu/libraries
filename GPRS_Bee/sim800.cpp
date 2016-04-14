/*
* gprs.cpp 
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

#include "sim800.h"

void SIM800::preInit(void)
{
	digitalWrite(powerPin,LOW);
	delay(10);
	while (serialSIM800.available()) {
    	char c = serialSIM800.read();
	}
	if(0 != sendCmdAndWaitForResp("AT\r\n","OK",2,CMD)) {
        digitalWrite(powerPin,HIGH);
        delay(2000);
        digitalWrite(powerPin,LOW);
    }
}

char SIM800::readByte(void)
{
    return serialSIM800.read();
}

int SIM800::readable()
{
    return serialSIM800.available();
}

int SIM800::readBuffer(char *buffer,int count, unsigned int timeOut)
{
    int i = 0;
	unsigned long timerStart,timerEnd;
	timerStart = millis();
    timerEnd = 1000*timeOut + timerStart;
    while(1) {
        while (serialSIM800.available()) {
            char c = serialSIM800.read();
            buffer[i++] = c;
            if(i >= count)break;
        }
        if(i >= count)break;
		if(millis() > timerEnd) {
            break;
        }
    }
    return 0;
}

void SIM800::cleanBuffer(char *buffer, int count)
{
    for(int i=0; i < count; i++) {
        buffer[i] = '\0';
    }
}

void SIM800::sendCmd(const char* cmd)
{
    serialSIM800.write(cmd);
}

void SIM800::sendATTest(void)
{
	sendCmdAndWaitForResp("AT\r\n","OK",DEFAULT_TIMEOUT,CMD);
}

bool SIM800::respCmp(const char *resp, unsigned int len, unsigned int timeout)
{
    int sum=0;
	unsigned long timerStart,timerEnd;
	timerStart = millis();
    timerEnd = 1000*timeout + timerStart;
    while(1) {
        if(serialSIM800.available()) {
            char c = serialSIM800.read();
            sum = (c==resp[sum]) ? sum+1 : 0;
            if(sum == len)break;
        }
		if(millis() > timerEnd) {
            return false;
        }
    }

    return true;
}

int SIM800::waitForResp(const char *resp, unsigned int timeout,DataType type)
{
    int len = strlen(resp);
    int sum = 0;
	unsigned long timerStart,timerEnd;
	timerStart = millis();
    timerEnd = 1000*timeout + timerStart;
    while(1) {
        if(serialSIM800.available()) {
            char c = serialSIM800.read();
            sum = (c==resp[sum]) ? sum+1 : 0;
            if(sum == len)break;
        }
        if(millis() > timerEnd) {
            return -1;
        }
    }

    if(type == CMD) {
        while(serialSIM800.available()) {
            char c = serialSIM800.read();
        }
    }

    return 0;
}

void SIM800::sendEndMark(void)
{
	serialSIM800.println((char)26);
}


int SIM800::sendCmdAndWaitForResp(const char* cmd, const char *resp, unsigned timeout,DataType type)
{
    sendCmd(cmd);
    return waitForResp(resp,timeout,type);
}


