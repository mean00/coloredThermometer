#include "Wire.h"
#include "SPI.h"
#include "USBComposite.h"
extern void mySetup();
extern void myLoop();

void setup() 
{
    mySetup();
}

void loop() 
{ 
    myLoop();
}

