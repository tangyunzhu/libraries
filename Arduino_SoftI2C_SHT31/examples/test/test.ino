// test software i2c

#include <Streaming.h>
#include <SoftwareI2CSHT.h>

SoftwareI2CSHT sht;

void setup()
{
    Serial.begin(115200);
    
//    cout << "hello world" << endl;
 sht.SoftI2C_selectChannel(2);
    
}

void loop()
{
    
}
