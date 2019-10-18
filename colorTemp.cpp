#include <WS2812B.h>
#include "Wire.h"
#include "MapleFreeRTOS1000.h"
#include "MapleFreeRTOS1000_pp.h"

#include "colorTemp.h"


#define NUM_LEDS 12

WS2812B *strip;

#define tempMin 8.
#define tempMax 30.

#define MKCOLOR(r,g,b) ((r<<16)+(g<<8)+b)
const uint32_t tempColors[NUM_LEDS]={
    MKCOLOR(  255,14,240),
    MKCOLOR(  255,6,240),
    MKCOLOR(  255,0,208),
    MKCOLOR(  255,0,80),
    MKCOLOR(  255,30,0),
    MKCOLOR(  255,190,0),
    MKCOLOR(  215,255,0),
    MKCOLOR(  0,255,92),
    MKCOLOR(  0,255,244),
    MKCOLOR(  0,116,255),
    MKCOLOR(  0,50,255),
    MKCOLOR(  0,0,255  ),
};


void ColorTemp::wheel(int ledPosition, bool onoff) 
{/*
    if(onoff) strip->setPixelColor(ledPosition,tempColors[NUM_LEDS-ledPosition]);
    else   strip->setPixelColor(ledPosition,0);        
  * */
    uint32_t col=tempColors[ledPosition];
    for(int i=0;i<ledPosition;i++) strip->setPixelColor(i,col);   
    for(int i=ledPosition;i<NUM_LEDS;i++) strip->setPixelColor(i,0);      

}


ColorTemp::ColorTemp()
{
    
    strip=new WS2812B(NUM_LEDS);
    strip->begin();
    strip->setBrightness(100);
    for(int i=0;i<NUM_LEDS;i++)
        wheel(i,true);
    strip->show();
    xDelay(400);
    for(int i=0;i<NUM_LEDS;i++)
        wheel(i,false);
    strip->show();

}
/**
 * 
 * @param temp
 */
void    ColorTemp::setColor(float temp)
{
    
    if(temp<tempMin) temp=tempMin;
    if(temp>=tempMax) temp=tempMax;
    
    float dex=(float)(NUM_LEDS)*(temp-tempMin)/(tempMax-tempMin);
    wheel(dex,false);
    strip->show();
}
