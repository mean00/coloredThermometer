#include "SPI.h"
#include "Wire.h"
#include "MapleFreeRTOS1000.h"
#include "MapleFreeRTOS1000_pp.h"
#include "tempDaemon.h"
#include "WSDisplay.h"
WSDisplay *_wsDisplay;

void rainbowCycle(uint8_t wait,WS2812B &strip) ;
void draw(uint32_t bitField, uint32_t finalColor,WS2812B &strip);
void MainTask( void *a );
void myLoop() ;
uint32_t Wheel(byte WheelPos) ;

#define DSO_MAIN_TASK_PRIORITY 10

TemperatureDaemon *tempDaemon;
static float temp=20.00;


/**
 * 
 */
void mySetup() 
{    
    afio_cfg_debug_ports( AFIO_DEBUG_SW_ONLY); // Unlock PB3 & PB4
    interrupts();    
    tempDaemon=new TemperatureDaemon;  
    tempDaemon->init();
    _wsDisplay=new WSDisplay;      
    xTaskCreate( MainTask, "MainTask", 250, NULL, DSO_MAIN_TASK_PRIORITY, NULL );   
    vTaskStartScheduler();      
}
/**
 * 
 * @param a
 */
void MainTask( void *a )
{    
    //_wsDisplay->banner();
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
    _wsDisplay->setTemp(temp);
    //_wsDisplay->snake();
    _wsDisplay->breath();
    //_wsDisplay->disolve();
//    xDelay(2000);
}

// EOF
