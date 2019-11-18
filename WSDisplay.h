
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
protected:
  static    uint32_t Wheel(int WheelPos );
  static    void     rainbowCycle(uint8_t wait,WS2812B &strip) ;
  static    void     snakeInternal(uint32_t bitField, uint32_t finalColor,WS2812B &strip);
            float    _temp;
            WSNumber *_digits;
            WS2812B  *_strip;
};