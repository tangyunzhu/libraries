
#ifndef __WIZFI250_H__
#define __WIZFI250_H__

#include <Arduino.h>
#include <Stream.h>

#define DEFAULT_WAIT_RESPONSE_TIME      15000         
#define DEFAULT_BAUDRATE                9600
#define MAX_CMD_LEN                     60
#define MAX_TRY_JOIN                    3

class WizFi250 : public Stream
{
public:
    WizFi250(Stream *);
    WizFi250(Stream &);

    size_t write(uint8_t);
    virtual size_t write(const uint8_t *, size_t);
    virtual int available();
    virtual int read();
    virtual int peek();
    virtual void flush();

    static WizFi250 *getInstance() {
        return instance;
    }

   
    void reset();
    void factoryReset();
   
    boolean leave();
    boolean save();
    
    boolean join(const char *ssid, const char *phrase, const char *auth);
    boolean staticIP(const char *ip, const char *mask, const char *gateway);
    //boolean connect(const char *sockettype,const char *host,uint16_t port,uint16_t localport);
    boolean connect(const char *host,uint16_t port,uint16_t localport); 
    boolean connect(uint16_t localport);
 
    int send(const char *data, int timeout = DEFAULT_WAIT_RESPONSE_TIME);
    int send(const uint8_t *data, int len, int timeout = DEFAULT_WAIT_RESPONSE_TIME);
    int receive(uint8_t *buf, int len, int timeout = DEFAULT_WAIT_RESPONSE_TIME);

    boolean ask(const char *q, const char *a, int timeout = DEFAULT_WAIT_RESPONSE_TIME);
    boolean sendCommand(const char *cmd, const char *ack = NULL, int timeout = DEFAULT_WAIT_RESPONSE_TIME);
 
    void clear();

private:
    static WizFi250  *instance;

    Stream *serial;

    boolean command_mode;
    boolean associated;
    uint8_t dhcp;
    uint8_t error_count;

};

#endif // __WIZFI250_H__

