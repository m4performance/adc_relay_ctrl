#include "threshold.h"


void THRESHOLD::getEnableThreshold(uint16_t & enableThreshold)
{
    enableThreshold = (thresholdAdcVal * (maxVal - minVal)) / UINT16_MAX;
}


void THRESHOLD::getDisableThreshold(uint16_t enableTh, uint16_t & disThreshold)
{
  disThreshold = 0;
  if(enableTh > hysteresis)
  {
    disThreshold = enableTh - hysteresis;
  }
  else
  {
    disThreshold = 0;
  }
}


THRESHOLD::THRESHOLD(AnalogIn * pressureThresholdIn, uint32_t minVal, uint32_t maxVal, uint32_t hysteresis)
{
    this->pressureThresholdIn = pressureThresholdIn;
    this->minVal = minVal;
    this->maxVal = maxVal;
    this->hysteresis = hysteresis;
}


void THRESHOLD::cyclicHandling(void)
{
    if(this->pressureThresholdIn)
    {
        thresholdAdcVal = pressureThresholdIn->read_u16();
        getEnableThreshold(enable);
        getDisableThreshold(enable, disable);
        // printf("Threshold ADC: %5u, ON: %umV, OFF: %umV\r\n", thresholdAdcVal, enable, disable);
    }
}