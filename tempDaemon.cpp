/**
 */
#include "SPI.h"
#include "Wire.h"
#include "MapleFreeRTOS1000.h"
#include "MapleFreeRTOS1000_pp.h"
#include "DHT12.h"
#include "tempDaemon.h"

DHT12  *dht=NULL;
/**
 * 
 */
TemperatureDaemon::TemperatureDaemon()
{
    for(int i=0;i<TEMP_SAMPLING_COUNT;i++)
        _measure[i]=0;
    _index=0;
    _first=true;
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
bool TemperatureDaemon::init(int dhtAdr)
{
    dht=new DHT12();
    dht->begin();
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
        float f=dht->readTemperature(false,true);
        if(_first)
        {
            _first=false;
            for(int i=1;i<TEMP_SAMPLING_COUNT;i++)
                _measure[i]=f;
        }
        _measure[_index]=f;
        _index=(_index+1)%TEMP_SAMPLING_COUNT;
        float avg=0;
        for(int i=0;i<TEMP_SAMPLING_COUNT;i++)
            avg+=_measure[i];
        avg=avg/TEMP_SAMPLING_COUNT;
        
        // only keep XX.y format, more accurate is pointless
        int round=(avg*10+5);
        _temp=(float)round/10.-1.;
        xDelay(500);
    }
};

// EOF