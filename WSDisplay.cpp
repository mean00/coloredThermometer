#include "Wire.h"
#include "MapleFreeRTOS1000.h"
#include "MapleFreeRTOS1000_pp.h"
#include "WSDisplay.h"
/**
 */
WSDisplay::WSDisplay()
{
    _digits=new WSNumber();   
    _strip=new WS2812B(NUM_LEDS);
    _strip->begin();
    _digits->setValue(20);
    

}
/**
 */
 void    WSDisplay::setTemp(float temp)
 {
     _temp=temp;
}
 /**
  */
void    WSDisplay::snake(void)
{
    _digits->setValue(floor(_temp+0.5));
    uint32_t full=_digits->getBitfield();
    uint32_t finalColor=0x2F33;
    snakeInternal(full,finalColor,*_strip);

}
/**
 * 
 */
void WSDisplay::banner()
{
    rainbowCycle(3,*_strip);
}

/**
 * 
 * @param bitField
 * @param finalColor
 * @param strip
 */
void WSDisplay::snakeInternal(uint32_t bitField, uint32_t finalColor,WS2812B &strip)
{
  int wait=20;

  int whiteLength = 10;
  int whiteSpeed=10;
  int head = whiteLength - 1;
  int tail = 0;

  static unsigned long lastTime = 0;
  uint32_t    bits[32];
  int nbPixel=strip.numPixels();
  
  
  for(int i=0;i<nbPixel;i++)
      if(bitField & (1<<i)) 
          bits[i]=finalColor;
      else                  
          bits[i]=0;

    int  j=0;
    while(1)
    {
        j=(j+1)&255;
        if(head<whiteLength)
        {
            for(int i=0;i<head;i++)
                 strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
            for(int i=head+1;i<nbPixel;i++)
                 strip.setPixelColor(i, strip.Color(0,0,0, 255 ) );
            head++;
            strip.show();
            xDelay(wait);        
            continue;
        }
        // Draw from 0 to tail with bits
        for(int i=0;i<tail;i++)
        {
            strip.setPixelColor(i, bits[i]);
        }
        // Draw from tail to head with color
        for(int i=tail;i<head;i++)
              strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
        
        // draw from head to end with black
        for(int i=head;i<nbPixel;i++)
              strip.setPixelColor(i, strip.Color(0,0,0, 255 ) );
        
        
        if(head!=nbPixel) 
            head++;            
        tail++;
        strip.show();
        xDelay(wait);        
        if(tail==nbPixel+1)
            return;

    }
}

/**
 */
// Slightly different, this makes the rainbow equally distributed throughout
void WSDisplay::rainbowCycle(uint8_t wait,WS2812B &strip) 
{
  uint16_t i, j;

  for(j=0; j<256*3; j++) 
  { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) 
    {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    xDelay(wait);
  }
}
/**
 */
uint32_t WSDisplay::Wheel(int WheelPos )
{
  if(WheelPos < 85) 
  {
    return WS2812B::Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } 
  else 
  {
    if(WheelPos < 170) 
    {
     WheelPos -= 85;
     return WS2812B::Color(255 - WheelPos * 3, 0, WheelPos * 3);
    } 
    else 
    {
     WheelPos -= 170;
     return WS2812B::Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
  }
}