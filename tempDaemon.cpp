/**
 */
#include "SPI.h"
#include "Wire.h"
#include "MapleFreeRTOS1000.h"
#include "MapleFreeRTOS1000_pp.h"
#include "tempDaemon.h"

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
bool TemperatureDaemon::init()
{
    xTaskCreate( TemperatureDaemon::trampoline, "Temp", 250, this, 12, NULL );   
    return true;
}
/**
 * 
 * @return 
 */
float   TemperatureDaemon::getTemp()
{
#if 1
    return 23;
#else    
    return _temp;
#endif
}

/**
 * 
 */
void   TemperatureDaemon::run()
{
    while(1)
    {
        xDelay(1000);
    }
   
};

// EOF