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
bool TemperatureDaemon::init(int beta, int resistance)
{
    ntc=new  NTC(PB1, beta,resistance,220); // supposedly -3950
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
float   TemperatureDaemon::getResistance()
{
    return _resistance;
}
/**
 * 
 */
void   TemperatureDaemon::run()
{
    while(1)
    {        
        ntc->getTemperature(_temp); // this should be fairly atomic
        ntc->getResistance(_resistance);
        xDelay(1000);
    }
   
};

// EOF