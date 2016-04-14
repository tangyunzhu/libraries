/*
* gprs.cpp 
* A library for SeeedStudio seeeduino GPRS shield 
*  
* Copyright (c) 2013 seeed technology inc. 
* Author      	: 	lawliet.zou(lawliet.zou@gmail.com)
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

#include <stdio.h>
#include "gprs.h"

GPRS* GPRS::inst;

GPRS::GPRS(int tx, int rx, int power, int baudRate, const char* apn, const char* userName, const char* passWord) : SIM800(tx,rx,power,baudRate)
{
    inst = this;
    _apn = apn;
    _userName = userName;
    _passWord = passWord;
}

int GPRS::init(void)
{	
	preInit();
	for(int i = 0; i < 2; i++){
		sendCmdAndWaitForResp("AT\r\n", "OK", 2,CMD);
		delay(100);
	}
	if(0 != sendCmdAndWaitForResp("AT+CFUN=1\r\n", "OK" ,2, CMD)){
		return -1;
	} 
    if(0 != checkSIMStatus()) {
        return -1;
    }
    return 0;
}

int GPRS::checkSIMStatus(void)
{
    char gprsBuffer[32];
    int count = 0;
    cleanBuffer(gprsBuffer,32);
    while(count < 3) {
        sendCmd("AT+CPIN?\r\n");
        readBuffer(gprsBuffer,32,DEFAULT_TIMEOUT);
        if((NULL != strstr(gprsBuffer,"+CPIN: READY"))) {
            break;
        }
        count++;
        delay(300);
    }
    if(count == 3) {
        return -1;
    }
    return 0;
}



int GPRS::sendSMS(char *number, char *data)
{
    char cmd[32];
    if(0 != sendCmdAndWaitForResp("AT+CMGF=1\r\n", "OK", DEFAULT_TIMEOUT,CMD)) { // Set message mode to ASCII
        return -1;
    }
    delay(500);
    snprintf(cmd, sizeof(cmd),"AT+CMGS=\"%s\"\r\n", number);
    if(0 != sendCmdAndWaitForResp(cmd,">",DEFAULT_TIMEOUT,CMD)) {
        return -1;
    }
    delay(1000);
    serialSIM800.write(data);
    delay(500);
	sendEndMark();
	return 0;
}

int GPRS::readSMS(int messageIndex, char *message,int length)
{
	int i = 0;
    char gprsBuffer[100];
	char cmd[16];
    char *p,*s;
	
	sendCmdAndWaitForResp("AT+CMGF=1\r\n","OK",DEFAULT_TIMEOUT,CMD);
	delay(1000);
	sprintf(cmd,"AT+CMGR=%d\r\n",messageIndex);
	serialSIM800.write(cmd);
    cleanBuffer(gprsBuffer,100);
    readBuffer(gprsBuffer,100,DEFAULT_TIMEOUT);
    if(NULL != ( s = strstr(gprsBuffer,"+CMGR"))){
        if(NULL != ( s = strstr(gprsBuffer,"+32"))){
            p = s + 6;
            while((*p != '\r')&&(i < length-1)) {
                message[i++] = *(p++);
            }
            message[i] = '\0';
        }
    }
    return 0;
}

int GPRS::deleteSMS(int index)
{
    char cmd[16];
    snprintf(cmd,sizeof(cmd),"AT+CMGD=%d\r\n",index);
    sendCmd(cmd);
    return 0;
}

int GPRS::callUp(char *number)
{
	char cmd[24];
    if(0 != sendCmdAndWaitForResp("AT+COLP=1\r\n","OK",DEFAULT_TIMEOUT,CMD)) {
        return -1;
    }
    delay(1000);
	sprintf(cmd,"ATD%s;\r\n", number);
	serialSIM800.write(cmd);
    return 0;
}

int GPRS::answer(void)
{
    sendCmd("ATA\r\n");
    return 0;
}


bool GPRS::join()
{
    char cmd[64];
    char ipAddr[32];
    //Select multiple connection
    //sendCmdAndWaitForResp("AT+CIPMUX=1\r\n","OK",DEFAULT_TIMEOUT,CMD);

    //set APN
    snprintf(cmd,sizeof(cmd),"AT+CSTT=\"%s\",\"%s\",\"%s\"\r\n",_apn,_userName,_passWord);
    sendCmdAndWaitForResp(cmd, "OK", DEFAULT_TIMEOUT,CMD);

    //Brings up wireless connection
    sendCmdAndWaitForResp("AT+CIICR\r\n","OK",DEFAULT_TIMEOUT,CMD);

    //Get local IP address
    sendCmd("AT+CIFSR\r\n");
    readBuffer(ipAddr,32,2);

    if(NULL != strstr(ipAddr,"AT+CIFSR")) {
        _ip = str_to_ip(ipAddr+12);
        if(_ip != 0) {
            return true;
        }
    }
    return false;
}

bool GPRS::connect(Protocol ptl,const char * host, int port, int timeout)
{
    char cmd[64];
    char resp[96];

    if(ptl == TCP) {
        sprintf(cmd, "AT+CIPSTART=\"TCP\",\"%s\",%d\r\n",host, port);
    } else if(ptl == UDP) {
        sprintf(cmd, "AT+CIPSTART=\"UDP\",\"%s\",%d\r\n",host, port);
    } else {
        return false;
    }
    sendCmd(cmd);
    readBuffer(resp,96,2*DEFAULT_TIMEOUT);
    if(NULL != strstr(resp,"CONNECT")) { //ALREADY CONNECT or CONNECT OK
        return true;
    }
    return false;//ERROR
}

bool GPRS::gethostbyname(const char* host, uint32_t* ip)
{
    uint32_t addr = str_to_ip(host);
    char buf[17];
    snprintf(buf, sizeof(buf), "%d.%d.%d.%d", (addr>>24)&0xff, (addr>>16)&0xff, (addr>>8)&0xff, addr&0xff);
    if (strcmp(buf, host) == 0) {
        *ip = addr;
        return true;
    }
    return false;
}

bool GPRS::disconnect()
{
    sendCmd("AT+CIPSHUT\r\n");
    return true;
}

bool GPRS::is_connected(void)
{
    char resp[96];
    sendCmd("AT+CIPSTATUS\r\n");
    readBuffer(resp,sizeof(resp),DEFAULT_TIMEOUT);
    if(NULL != strstr(resp,"CONNECTED")) {
        //+CIPSTATUS: 1,0,"TCP","216.52.233.120","80","CONNECTED"
        return true;
    } else {
        //+CIPSTATUS: 1,0,"TCP","216.52.233.120","80","CLOSED"
        //+CIPSTATUS: 0,,"","","","INITIAL"
        return false;
    }
}

bool GPRS::close()
{
    // if not connected, return
    if (is_connected() == false) {
        return true;
    }
    if(0 != sendCmdAndWaitForResp("AT+CIPCLOSE\r\n", "CLOSE OK", DEFAULT_TIMEOUT,CMD)) {
        return false;
    }
    return true;
}

int GPRS::readable(void)
{
    return readable();
}

int GPRS::wait_readable(int wait_time)
{
	unsigned long timerStart,timerEnd;
	int dataLen = 0;
	timerStart = millis();
    timerEnd = 1000*wait_time + timerStart;
	while(millis() < timerEnd) {
		delay(500);
		dataLen = readable();
		if(dataLen > 0){
			break;
		}
    }
    return dataLen;
}

int GPRS::wait_writeable(int req_size)
{
    return req_size+1;
}

int GPRS::send(const char * str, int len)
{
    char cmd[32];
    char resp[16];
    delay(1000);
    if(len > 0){
        snprintf(cmd,sizeof(cmd),"AT+CIPSEND=%d\r\n",len);
        if(0 != sendCmdAndWaitForResp(cmd,">",DEFAULT_TIMEOUT,CMD)) {
            return false;
        }
        if(0 != sendCmdAndWaitForResp(str,"SEND OK",DEFAULT_TIMEOUT+2,DATA)) {
            return false;
        }
    }
    return len;
}

int GPRS::recv(char* buf, int len)
{
    cleanBuffer(buf,len);
    readBuffer(buf,len,DEFAULT_TIMEOUT);
    return strlen(buf);
}

uint32_t GPRS::str_to_ip(const char* str)
{
    uint32_t ip = 0;
    char* p = (char*)str;
    for(int i = 0; i < 4; i++) {
        ip |= atoi(p);
        p = strchr(p, '.');
        if (p == NULL) {
            break;
        }
        ip <<= 8;
        p++;
    }
    return ip;
}


