// Draw Boxes - Demonstrate drawRectangle and fillRectangle

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
    for(int r=0;r<115;r=r+2)
    {
        Tft.drawCircle(119,160,r,random(0xFFFF));
    }
    delay(10);
  
}
