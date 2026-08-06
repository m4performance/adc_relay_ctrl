#ifndef THRESHOLD_H
#define THRESHOLD_H


#include "global.h"
#include <BufferedSerial.h>


class THRESHOLD
{
    private:
    AnalogIn * pressureThresholdIn = NULL;
    uint32_t minVal = 0;
    uint32_t maxVal = 0;
    uint32_t hysteresis = 0;  

    void getEnableThreshold(uint16_t & enableThreshold);
    void getDisableThreshold(uint16_t enableTh, uint16_t & disThreshold);


    public:
    uint16_t thresholdAdcVal = 0;
    uint16_t enable = 0;
    uint16_t disable = 0;

    THRESHOLD(AnalogIn * pressureThresholdIn, uint32_t minVal, uint32_t maxVal, uint32_t hysteresis);
    void cyclicHandling(void);
};


#endif