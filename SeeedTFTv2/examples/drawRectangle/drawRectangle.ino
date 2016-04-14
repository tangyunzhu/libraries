// Draw Boxes - Demonstrate drawRectangle and fillRectangle

#include <stdint.h>
#include <TFTv2.h>
#include <SPI.h>

void setup()
{
    TFT_BL_ON;      // turn on the background light
    Tft.TFTinit();  // init TFT library

    Tft.drawRectangle(10, 3, 200,60,BLUE);
    Tft.fillRectangle(0, 80, 100,65,YELLOW);
    Tft.drawRectangle(30, 160, 60, 60,RED);
}

void loop()
{
  
}
