
#pragma once


/**
 */
class simple7Seg
{
public:    
    
        static simple7Seg   *instantiate(int D1, int D2, int D3, int D4, int aa, int bb, int cc, int dd, int ee, int ff, int gg, int decimalpoint);
        
        virtual bool setNumber(int digit, int value)=0;
        
};