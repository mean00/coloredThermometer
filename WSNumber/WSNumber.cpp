
#include "WSDigit.h"
#include "WSNumber.h"

WSNumber::WSNumber()
{
   _high=new WSDigit(true,0);
   _low=new WSDigit(false,15);
   _bitfield=0;
   _value=0;
}
/**
 */
void WSNumber::setValue(int value)
{
    _value=value;
    int ten=value/10;
    int single=value-ten*10;
    _bitfield=   _high->setDigit(ten)+_low->setDigit(single);
}
int  WSNumber::getBitfield()
{
    return _bitfield;
}
