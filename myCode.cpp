#include "SPI.h"
#include "Wire.h"
#include "MapleFreeRTOS1000.h"
#include "MapleFreeRTOS1000_pp.h"

#include "simple7seg.h"

void MainTask( void *a );
void myLoop() ;

#define NUM_LEDS 12

#define DSO_MAIN_TASK_PRIORITY 10

simple7Seg *sevenSeg; 

void mySetup() 
{
    afio_cfg_debug_ports( AFIO_DEBUG_SW_ONLY); // Unlock PB3 & PB4

    sevenSeg=simple7Seg::instantiate(PB12,PB13,PB14,PB15,     PA8,PA10,PB11,PB10, PB6,PA9,PB8,PB7);    
    xTaskCreate( MainTask, "MainTask", 250, NULL, DSO_MAIN_TASK_PRIORITY, NULL );   
    vTaskStartScheduler();      

}
void MainTask( void *a )
{    
    while(1)
    {
        myLoop();
    }
}
void myLoop() 
{ 
    static char buffer[10];
    static int i=0;
    for(int j=0;j<4;j++)
    {
        int k=(i+j)%10;
        sevenSeg->setNumber(j,k);
    }
    xDelay(1000);    
    i++;
    i%=10;
}
