#pragma once
#include "dso_adc.h"

class NTC
{
public:
                        NTC(int pin, float beta, float resistance, float otherResistance);
                bool    getTemperature(float &temp);
                bool    getResistance(float &res) {res=_currentValue;return true;};
            
protected:
            DSOADC *_adc;
            int _pin;
            float _beta,_resistance, _otherResistance;
            float _currentValue;
};

