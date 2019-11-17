
#pragma once
class WSDigit
{
public:
        WSDigit(bool down,int index);
  int   setDigit(int digit);


protected:
  int             _startAt;
  const int       *_table;
  
  
  
};