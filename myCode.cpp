#include "SPI.h"
#include "Wire.h"
#include "MapleFreeRTOS1000.h"
#include "MapleFreeRTOS1000_pp.h"
#include "tempDaemon.h"
#include "WSNumber.h"
#include "WS2812B.h"

void rainbowCycle(uint8_t wait) ;
void draw(uint32_t bitField, uint32_t finalColor);
void MainTask( void *a );
void myLoop() ;
uint32_t Wheel(byte WheelPos) ;
WSNumber *digits;

#define NUM_LEDS 30

#define DSO_MAIN_TASK_PRIORITY 10

TemperatureDaemon *tempDaemon;
static float temp=20.00;
WS2812B strip(NUM_LEDS);

/**
 * 
 */
void mySetup() 
{
    
    afio_cfg_debug_ports( AFIO_DEBUG_SW_ONLY); // Unlock PB3 & PB4
    digits=new WSNumber();   
    digits->setValue(floor(temp+0.5));
    xTaskCreate( MainTask, "MainTask", 250, NULL, DSO_MAIN_TASK_PRIORITY, NULL );   
    vTaskStartScheduler();      
}
/**
 * 
 * @param a
 */
void MainTask( void *a )
{    
    tempDaemon=new TemperatureDaemon;
    tempDaemon->init(0x5c);
    rainbowCycle(3);

    while(1)
    {
        myLoop();
    }
}
/**
 * 
 */
void myLoop() 
{ 
    temp=tempDaemon->getTemp();    
    digits->setValue(floor(temp+0.5));
    uint32_t full=digits->getBitfield();
    uint32_t finalColor=0x2F33;
    draw(full,finalColor);
    xDelay(2000);

}


void draw(uint32_t bitField, uint32_t finalColor)
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


// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) 
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

uint32_t Wheel(byte WheelPos) 
{
  if(WheelPos < 85) 
  {
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } 
  else 
  {
    if(WheelPos < 170) 
    {
     WheelPos -= 85;
     return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    } 
    else 
    {
     WheelPos -= 170;
     return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
  }
}
// EOF
