// Draw Texts - Demonstrate drawChar and drawString
#include <stdint.h>
#include <TFTv2.h>
#include <SPI.h>

void setup()
{
    TFT_BL_ON;      // turn on the background light
    Tft.TFTinit();  // init TFT library
}

void loop()
{
  
    Tft.drawChar('S',0,0,1,RED);
    Tft.drawChar('E',10,10,2,BLUE);
    Tft.drawChar('E',20,40,3,GREEN);
    Tft.drawChar('E',30,80,4,YELLOW);
    Tft.drawChar('D',40,120,4,YELLOW);
    Tft.drawString("Happy!",0,160,1,CYAN);
    Tft.drawString("Happy!",0,200,2,WHITE);
    Tft.drawString("Happy!",0,240,4,WHITE);
}
