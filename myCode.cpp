#include "SPI.h"
#include "Wire.h"
#include "MapleFreeRTOS1000.h"
#include "MapleFreeRTOS1000_pp.h"
#include "tempDaemon.h"
#include "WSDisplay.h"
WSDisplay *_wsDisplay;



void MainTask( void *a );
void myLoop() ;
void startUSBHID();
extern void hidSetTemp(float v);
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
    tempDaemon->init(-3230); // our beta
    _wsDisplay=new WSDisplay;      
    xTaskCreate( MainTask, "MainTask", 250, NULL, DSO_MAIN_TASK_PRIORITY, NULL );   
    startUSBHID();
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

#define DOIT(x) \        
        temp=tempDaemon->getTemp();         \
        _wsDisplay->setTemp(temp); \
        hidSetTemp(temp);   \
        _wsDisplay->x(); \
        xDelay(1000); \
        _wsDisplay->clear(); \
        xDelay (5000);

/**
 * 
 */
void myLoop() 
{ 
    DOIT(snake)
    DOIT(disolve)
    DOIT(breath)
    
    
}

// EOF
