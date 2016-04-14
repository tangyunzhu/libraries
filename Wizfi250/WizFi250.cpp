#include <string.h>
#include "WizFi250.h"
#include "Debug.h"

WizFi250 *WizFi250::instance;

WizFi250::WizFi250(Stream *serial)
{
    instance = this;
    this->serial = serial;
    setTimeout(DEFAULT_WAIT_RESPONSE_TIME);
    associated = false;
    error_count = 0;
}

WizFi250::WizFi250(Stream &serial)
{
    instance = this;
    this->serial = &serial;
    setTimeout(DEFAULT_WAIT_RESPONSE_TIME);
    associated = false;
}

int WizFi250::available()
{
    return serial->available();
}

int WizFi250::read()
{
    return serial->read();
}

int WizFi250::peek()
{
    return serial->peek();
}

void WizFi250::flush()
{
    return serial->flush();
}

size_t WizFi250::write(uint8_t c)
{
    return serial->write(c);
}

size_t WizFi250::write(const uint8_t *buffer, size_t size)
{
    return serial->write(buffer, size);
}

void WizFi250::reset()
{
    sendCommand("AT+MRESET\r", "[OK]");
    delay(1000);
}
void WizFi250::factoryReset() {
	sendCommand("AT+MFDEF=FR\r", "[OK]");
    delay(1000);
}
boolean WizFi250::save()
{
    if (sendCommand("AT+MPROF=S\r", "OK")) {
        return true;
    }
    return false;
}
boolean WizFi250::leave()
{
    if (sendCommand("AT+WLEAVE\r", "OK")) {
        associated = false;
        return true;
    }
    return false;
}

 boolean WizFi250::staticIP(const char *ip, const char *mask, const char *gateway)
{
    boolean result = false;
    char cmd[MAX_CMD_LEN];
    reset(); 
    sendCommand("AT+WNET=0,");
    snprintf(cmd, MAX_CMD_LEN, "%s, %s, %s\r", ip, mask, gateway);
    sendCommand(cmd, "[OK]");
    result = sendCommand("AT+WJOIN\r","[OK]");
    return result;
}
 
boolean WizFi250::connect(const char *host,uint16_t port,uint16_t localport)
{
  char cmd[MAX_CMD_LEN];
  snprintf(cmd, sizeof(cmd), "AT+SCON=SO,TCN,%s,%d,%d,1\r",host,port,localport);
   if (sendCommand(cmd, "CONNECT")) {
    clear();
    return false;
  }
  return true;
}
boolean WizFi250::connect(uint16_t localport)
{
  char cmd[MAX_CMD_LEN];
  snprintf(cmd, sizeof(cmd), "AT+SCON=O,TSN, , ,%d,1\r",localport);
   if (sendCommand(cmd, "CONNECT")) {
    clear();
    return false;
  }

  return true;
}
boolean WizFi250::join(const char *ssid, const char *phrase, const char *auth)
{
    char cmd[MAX_CMD_LEN];
    boolean result = false;
    sendCommand("AT\r", "OK"); 
    delay(10);
    // ssid
    snprintf(cmd, MAX_CMD_LEN, "AT+WSET=0,%s\r", ssid);
    sendCommand(cmd,"OK");
 
    //key&auth
     snprintf(cmd, MAX_CMD_LEN, "AT+WSEC=0,%s,%s\r", auth, phrase);
     sendCommand(cmd,"OK");
     sendCommand("AT+WNET=1\r","OK");
     result = sendCommand("AT+WJOIN\r","OK");
    delay(100);
    if(!result){
    return false;}
    associated = true;
    clear();
    delay(50);
    return true;
    }
 
 
int WizFi250::send(const uint8_t *data, int len, int timeout)
{
    int write_bytes = 0;
    boolean write_error = false;
    unsigned long start_millis;

    if (data == NULL) {
        return 0;
    }
    while (write_bytes < len) {
        if (write(data[write_bytes]) == 1) {
            write_bytes++;
            write_error = false;
        } else {         // failed to write, set timeout
            if (write_error) {
                if ((millis() - start_millis) > timeout) {
                    DBG("Send data. Timeout!\r\n");
                    break;
                }
            } else {
                write_error = true;
                start_millis = millis();
            }
        }
    }

    return write_bytes;
}

int WizFi250::send(const char *data, int timeout)
{
    send((uint8_t *)data, strlen(data), timeout);
}

boolean WizFi250::ask(const char *q, const char *a, int timeout)
{
    unsigned long start;
    unsigned long end;
    int q_len = strlen(q);
    send((uint8_t *)q, q_len, timeout);

    if (a != NULL) {
        setTimeout(timeout);
        start = millis();
        boolean found = find((char *)a);
        if (!found) {
            end = millis();
            if ((end - start) < timeout) {
                DBG("\r\n");
                DBG(q);
                DBG("\r\nTry to find: ");
                DBG(a);
                DBG("\r\nTimeout: ");
                DBG(timeout);
                DBG("\r\nStart time: ");
                DBG(start);
                DBG("\r\nEnd time: ");
                DBG(end);
                DBG("\r\n***** Probably ot enough memory *****\r\n");
            } else {
                DBG("Timeout! ");
            }

            return false;
        }
    }

    return true;
}

boolean WizFi250::sendCommand(const char *cmd, const char *ack, int timeout)
{
    DBG("CMD: ");
    DBG(cmd);
    DBG("\r\n");
    clear();
 
    if (!ask(cmd, ack, timeout)) {
        DBG("Failed to run: ");
        DBG(cmd);
        DBG("\r\n");
        error_count++;
        return false;
    }
    error_count = 0;
    return true;
}

void WizFi250::clear()
{
    char r;
    while (receive((uint8_t *)&r, 1, 10) == 1) {
    }
}

 
int WizFi250::receive(uint8_t *buf, int len, int timeout)
{
    int read_bytes = 0;
    int ret;
    unsigned long end_millis;

    while (read_bytes < len) {
        end_millis = millis() + timeout;
        do {
            ret = read();
            if (ret >= 0) {
                break;
            }
        } while (millis() < end_millis);

        if (ret < 0) {
            return read_bytes;
        }
        buf[read_bytes] = (char)ret;
        read_bytes++;
    }

    return read_bytes;
}
