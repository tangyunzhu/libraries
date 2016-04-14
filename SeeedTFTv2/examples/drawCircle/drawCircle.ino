// Draw Circles - Demonstrate drawCircle and fillCircle APIs

#include <stdint.h>
#include <TFTv2.h>
#include <SPI.h>
void setup()
{
    TFT_BL_ON;      // turn on the background light
    Tft.TFTinit();  // init TFT library

    Tft.drawCircle(100, 100, 30,YELLOW);
    Tft.drawCircle(100, 200, 40,CYAN);
    Tft.fillCircle(200, 100, 30,RED);
    Tft.fillCircle(200, 200, 30,BLUE);
}

void loop()
{
  
}
