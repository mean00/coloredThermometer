#include "NTC.h"
#include "MapleFreeRTOS1000.h"
#include "MapleFreeRTOS1000_pp.h"
#define K 273.15
/**
 */
NTC::NTC(int pin, float beta, float resistance, float otherResistance)
{
    _pin=pin;
    _beta=beta;
    _resistance=resistance;
    _otherResistance=otherResistance;
    
    _adc=new DSOADC(_pin);
    _adc->setupADCs();
    _adc->prepareDMASampling(ADC_SMPR_239_5,ADC_PRE_PCLK2_DIV_8);    
}
/**
 */
bool    NTC::getTemperature(float &temp)
{
    uint16_t *samples;
    int nbSamples;

        _adc->startDMASampling(16);
        xAssert(true==_adc->getSamples(&samples,nbSamples));
        xAssert(nbSamples);
        _adc->stopDmaCapture();
        // avg 
        float r=0;
        for(int i=0;i<nbSamples;i++)
            r+=samples[i];
        r/=nbSamples;
        
        // 1 - Compute resistance
        float ntcResistor;
        if(r>=4090) 
            ntcResistor=480; 
        else
        {
            ntcResistor=_otherResistance*(r)/(4095-r);
        }
        // now compute temp
        if(fabs(_resistance-ntcResistor)<1.0)
        {
            temp=25;
        }else
        {
            float alpha=_beta/log(_resistance/ntcResistor);
            temp=alpha*(K+25)/(K+25+alpha)-K;
        }
        return true;
}

// EOF