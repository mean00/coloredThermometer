#include "SPI.h"
#include "Wire.h"
#include "simple7seg.h"

#define NUM_LEDS 12

simple7Seg *sevenSeg; 

void mySetup() 
{
    
    sevenSeg=simple7Seg::instantiate(PB12,PB13,PB14,PB15,     PA8,PA10,PB11,PB10, PB6,PA9,PB8,PB7);
    
}

void myLoop() 
{ 
    static char buffer[10];
    static int i=0;
    sevenSeg->setNumber(1,i);
    delay(1.5*500);    
    i++;
    i%=10;
}
