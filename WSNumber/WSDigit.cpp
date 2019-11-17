
#include "WSDigit.h"

// Left
#define MKDIGIT(A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14) \
            A14,A11,A8,A5,A2, \
            A1,A4,A7,A10,A13, \
            A12,A9,A6,A3,A0
    
const int DigitTableUp[10][15]={
#include "digitDef.h"
};

#undef MKDIGIT
// Right 
#define MKDIGIT(A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14) \
            A2,A5,A8,A11,A14,\
            A13,A10,A7,A4,A1, \
            A0,A3,A6,A9,A12

const int DigitTableDown[10][15]={
#include "digitDef.h"
};

/**
 */
WSDigit::WSDigit(bool down,int index)
{
    _startAt=index;
    if(down) 
        _table=(const int *)DigitTableDown;
    else
        _table=(const int *)DigitTableUp;
}
/**
 */
int WSDigit::setDigit(int digit)
{
    const int *c=_table+15*digit;
    int value=0;
    int mask=1<<_startAt;
    for(int i=0;i<15;i++)
    {
        if(c[i])
            value+=mask;
        mask<<=1;
    }
    return value;
}
