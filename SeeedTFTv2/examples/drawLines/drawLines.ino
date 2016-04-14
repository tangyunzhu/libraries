// Draw Lines - Demonstrate drawRectangle and fillRectangle

#include <stdint.h>
#include <TFTv2.h>
#include <SPI.h>
void setup()
{
    TFT_BL_ON;      // turn on the background light
    Tft.TFTinit();  //init TFT library
/*  Demo of 
   drawLine(unsigned int x0,unsigned int y0,unsigned int x1,unsigned int y1,unsigned int color);
   drawVerticalLine(unsigned int poX, unsigned int poY,unsigned int length,unsigned int color);
   drawHorizontalLine(unsigned int poX, unsigned int poY,unsigned int length,unsigned int color);
*/
   Tft.drawLine(0,0,239,319,RED);
   Tft.drawVerticalLine(60,100,100,GREEN);
   Tft.drawHorizontalLine(30,60,150,BLUE);
}

void loop()
{
  
}
