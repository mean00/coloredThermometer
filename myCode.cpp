#include "SPI.h"
#include "Wire.h"
#include "MapleFreeRTOS1000.h"
#include "MapleFreeRTOS1000_pp.h"
#include "simple7seg.h"
#include "tempDaemon.h"
#include "colorTemp.h"

void MainTask( void *a );
void myLoop() ;

#define NUM_LEDS 12

#define DSO_MAIN_TASK_PRIORITY 10

simple7Seg *sevenSeg; 
TemperatureDaemon *tempDaemon;
ColorTemp *colors;
static float temp=20.00;
/**
 * 
 */
void mySetup() 
{
    
    afio_cfg_debug_ports( AFIO_DEBUG_SW_ONLY); // Unlock PB3 & PB4

    sevenSeg=simple7Seg::instantiate(PB12,PB13,PB14,PB15,     PA8,PA10,PB11,PB10, PA4,PA9,PB8,PA3);    
    sevenSeg->setSignificantDigits(2,1); // xy.z
    xTaskCreate( MainTask, "MainTask", 250, NULL, DSO_MAIN_TASK_PRIORITY, NULL );   
    vTaskStartScheduler();      
}
/**
 * 
 * @param a
 */
void MainTask( void *a )
{    
    tempDaemon=new TemperatureDaemon;
    tempDaemon->init(0x5c);
    colors=new ColorTemp();
    
    
    while(1)
    {
        myLoop();
    }
}
/**
 * 
 */
void myLoop() 
{ 
    temp=tempDaemon->getTemp();    
    sevenSeg->printAsFloat((float)temp);
    colors->setColor(temp);
    xDelay(500);    
}
// EOF
