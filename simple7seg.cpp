

#include "Wire.h"
#include "simple7seg_priv.h"

simple7Seg *simple7Seg:: instantiate(int D1, int D2, int D3, int D4, int aa, int bb, int cc, int dd, int ee, int ff, int gg, int decimalpoint)
{
    return new simple7SegImpl(D1,D2,D3, D4, aa,bb,cc,dd,ee,ff,gg,decimalpoint);
}
/**
 */
 simple7SegImpl::simple7SegImpl(int D1, int D2, int D3, int D4, int aa, int bb, int cc, int dd, int ee, int ff, int gg, int decimalpoint)
 {
     _digits[0]=D1;
     _digits[1]=D2;
     _digits[2]=D3;
     _digits[3]=D4;
     
     _pins[0]=aa;_pins[1]=bb;_pins[2]=cc;_pins[3]=dd;
     _pins[4]=ee;_pins[5]=ff;_pins[6]=gg,_pins[7]= decimalpoint;
     for(int i=0;i<3;i++)
     {
         pinMode(_digits[i],OUTPUT);
         digitalWrite(_digits[i],1);
     }
      for(int i=0;i<8;i++)
      {
         pinMode(_pins[i],OUTPUT);
         digitalWrite(_pins[i],0);
      }
 }
 
#define MAPPED(a,b,c,d,  e,f,g) (a+(b<<1)+(c<<2)+(d<<3)+(e<<4)+(f<<5)+(g<<6))
 
 uint16_t numberTable[16]=
 {
     //     A   B   C   D     E     F    G
     MAPPED(1,  1,  1,  1,    1  ,  1 ,  0), // 0
     MAPPED(0,  1,  1,  0,    0  ,  0 ,  0), // 1
     
     MAPPED(1,  1,  0,  1,    1  ,  0 ,  1), // 2
     MAPPED(1,  1,  1,  1,    0  ,  0 ,  1), // 3
     MAPPED(0,  1,  1,  0,    0  ,  1 ,  1), // 4
     
     MAPPED(1,  0,  1,  1,    0  ,  1 ,  1), // 5
     MAPPED(0,  0,  1,  1,    1  ,  1 ,  1), // 6
     MAPPED(1,  1,  1,  0,    0  ,  0 ,  0), // 7
     MAPPED(1,  1,  1,  1,    1  ,  1 ,  1), // 8
     MAPPED(1,  1,  1,  1,    0  ,  0 ,  1), // 9
     
     
     MAPPED(0,  1,  1,  0,    0  ,  0 ,  0), // 10 x0A
     MAPPED(0,  1,  1,  0,    0  ,  0 ,  0), // 11 x0B
     MAPPED(0,  1,  1,  0,    0  ,  0 ,  0), // 12 x0C
     MAPPED(0,  1,  1,  0,    0  ,  0 ,  0), // 13 x0D
     MAPPED(0,  1,  1,  0,    0  ,  0 ,  0), // 14 x0E
     MAPPED(0,  1,  1,  0,    0  ,  0 ,  0), // 15 x0F
     
 };
 
/**
 */
 bool simple7SegImpl::setNumber(int digit, int value)
 {
     digitalWrite(_digits[digit],0);
     //
     int x=numberTable[value];
     for(int i=0;i<8;i++)
     {
         digitalWrite(_pins[i],x&1);
         x>>=1;
     }     
     delay(20);
     //digitalWrite(_digits[digit],1);
     return true;
 }
 