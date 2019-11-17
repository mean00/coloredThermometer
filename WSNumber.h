
#include "stdint.h"
#pragma once
class WSDigit;
class WSNumber
{
public:
                WSNumber();
        void setValue(int value);
        int  getBitfield();

protected:
  int       _value;
  uint32_t  _bitfield;
  WSDigit   *_high, *_low;
};