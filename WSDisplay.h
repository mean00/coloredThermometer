
#pragma once

#include "WSNumber.h"
#include "WS2812B.h"


#define NUM_LEDS 30
class WSDisplay
{
public:
                    WSDisplay(); 
            void    setTemp(float temp);
            void    snake(void);
            void    banner();
            void    disolve();
            void    rainbow();
            void    setPixelColor(int index, uint32_t color,int alpha=0xff);
            void    setPixelColor(int index, int r, int g, int b, int alpha=0xff);
protected:
  static    uint32_t Wheel(int WheelPos );
            void     rainbowCycle(uint8_t wait,WS2812B &strip) ;
            void     snakeInternal(uint32_t bitField, uint32_t finalColor,WS2812B &strip);
            void     disolveInternal(uint32_t bitField, uint32_t finalColor,WS2812B &strip);
            void     rainbowInternal(uint32_t bitField, uint32_t finalColor,WS2812B &strip);
            float    _temp;
            WSNumber *_digits;
            WS2812B  *_strip;
};