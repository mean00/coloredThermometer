#include "Wire.h"
#include "MapleFreeRTOS1000.h"
#include "MapleFreeRTOS1000_pp.h"
#include "WSDisplay.h"
#include "breath_table.h"
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
void    WSDisplay::disolve(void)
{
    _digits->setValue(floor(_temp+0.5));
    uint32_t full=_digits->getBitfield();
    uint32_t finalColor=0x2F33;
    disolveInternal(full,finalColor,*_strip);

}

void    WSDisplay::breath(void)
{
    _digits->setValue(floor(_temp+0.5));
    uint32_t full=_digits->getBitfield();
    uint32_t finalColor=0x2F33;
    breathInternal(full,finalColor,*_strip);

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
 * @param index
 * @param color
 * @param alpha
 */
void WSDisplay::setPixelColor(int index, uint32_t color,int alpha)
{    
    setPixelColor(index, (color>>16)&0xff,(color>>8)&0xff,color&0xff,alpha);
}
void WSDisplay::setPixelColorAlpha16(int index, uint32_t color,int alpha)
{    
    setPixelColorAlpha16(index, (color>>16)&0xff,(color>>8)&0xff,color&0xff,alpha);
}
void WSDisplay::setPixelColorAlpha16(int index, int r, int g, int b, int alpha16)
{
    if(alpha16!=0xffff)
    {
        int mul=alpha16;
        r=(r*mul+mul/2)>>16;
        g=(g*mul+mul/2)>>16;
        b=(b*mul+mul/2)>>16;    
    }
    _strip->setPixelColor(index, r,g,b );
}
void WSDisplay::setPixelColor(int index, int r, int g, int b, int alpha)
{
    if(alpha!=0xff)
    {
        int mul=alpha*256+alpha;
        r=(r*mul)>>16;
        g=(g*mul)>>16;
        b=(b*mul)>>16;
    
    }
    _strip->setPixelColor(index, r,g,b );
    
       
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
                 setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
            for(int i=head+1;i<nbPixel;i++)
                 setPixelColor(i, strip.Color(0,0,0, 255 ) );
            head++;
            strip.show();
            xDelay(wait);        
            continue;
        }
        // Draw from 0 to tail with bits
        for(int i=0;i<tail;i++)
        {
             setPixelColor(i, bits[i]);
        }
        // Draw from tail to head with color
        for(int i=tail;i<head;i++)
              setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
        
        // draw from head to end with black
        for(int i=head;i<nbPixel;i++)
              setPixelColor(i, strip.Color(0,0,0, 255 ) );
        
        
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
 * 
 * @param bitField
 * @param finalColor
 * @param strip
 */
void WSDisplay::disolveInternal(uint32_t bitField, uint32_t finalColor,WS2812B &strip)
{
  int step=32;
  int shift;
  int wait=40;
  uint32_t    bits[32];
  int nbPixel=strip.numPixels();
  
  
  for(int i=0;i<nbPixel;i++)
      if(bitField & (1<<i)) 
          bits[i]=finalColor;
      else                  
          bits[i]=0;

  int j;
  uint32_t mask=0xff;
  for(int luma=0;luma<255;luma+=step)
  {
      shift=luma/8;
      for(int i=0;i<nbPixel;i++)
      {
          if(luma>128 || !bits[i])
          {
              uint32_t col= Wheel(((i * 256 / strip.numPixels()) + j) & 255);
              setPixelColor(i,col>>16,col>>8,col);
          }
          else
          {
               setPixelColor(i, bits[i]);
          }   
          
      }
      j=(j+1)&255;
      strip.show();
      xDelay(wait);
      
  }
}

/**
 * 
 * @param bitField
 * @param finalColor
 * @param strip
 */
void WSDisplay::breathInternal(uint32_t bitField, uint32_t finalColor,WS2812B &strip)
{
  int wait=30;
  int nbPixel=strip.numPixels();
  
  int j;
  for(int j=0;j<256*3;j++) // X cycles
  {
      int alpha=breath_k[j&0xff];      
      for(int i=0;i<nbPixel;i++)
      {
          if(bitField & (1<<i)) 
          {              
              setPixelColorAlpha16(i,finalColor,alpha);
          }
          else
          {
               setPixelColor(i, 0);
          }   
          
      }
      strip.show();
      xDelay(wait);
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
      setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
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