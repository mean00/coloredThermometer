
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
            void    breath();
            void    trail();
            void    clear();
            void    setPixelColor(int index, uint32_t color,int alpha=0xff);
            void    setPixelColor(int index, int r, int g, int b, int alpha=0xff);
            void    setPixelColorAlpha16(int index, int r, int g, int b, int alpha16);
            void    setPixelColorAlpha16(int index, uint32_t color, int alpha16);
protected:
  static    uint32_t Wheel(int WheelPos );
            void     rainbowCycle(uint8_t wait,WS2812B &strip) ;
            void     snakeInternal(uint32_t bitField, uint32_t finalColor,WS2812B &strip);
            void     disolveInternal(uint32_t bitField, uint32_t finalColor,WS2812B &strip);
            void     breathInternal(uint32_t bitField, uint32_t finalColor,WS2812B &strip);
            void     trailInternal(uint32_t bitField, uint32_t finalColor,WS2812B &strip);
            float    _temp;
            WSNumber *_digits;
            WS2812B  *_strip;
            uint32_t _color;
};