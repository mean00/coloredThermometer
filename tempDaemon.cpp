/**
 */
#include "SPI.h"
#include "Wire.h"
#include "MapleFreeRTOS1000.h"
#include "MapleFreeRTOS1000_pp.h"
#include "tempDaemon.h"
#include "NTC.h"
NTC *ntc;
/**
 * 
 */
TemperatureDaemon::TemperatureDaemon()
{
}
/**
 * 
 * @param a
 */
void TemperatureDaemon::trampoline(void *a)
{
    TemperatureDaemon *instance=(TemperatureDaemon *)a;
    instance->run();
}
/**
 * 
 * @param dhtAdr
 * @return 
 */
bool TemperatureDaemon::init(int beta)
{
    ntc=new  NTC(PB1, beta,100,220); // supposedly -3950
    xTaskCreate( TemperatureDaemon::trampoline, "Temp", 250, this, 12, NULL );   
    return true;
}
/**
 * 
 * @return 
 */
float   TemperatureDaemon::getTemp()
{
    return _temp;
}

/**
 * 
 */
void   TemperatureDaemon::run()
{
    while(1)
    {
        xDelay(1000);
        ntc->getTemperature(_temp); // this should be fairly atomic
    }
   
};

// EOF