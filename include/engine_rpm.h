#ifndef ENGINE_RPM_H
#define ENGINE_RPM_H


#include "global.h"
#include <BufferedSerial.h>

class engineRPM
{
    private:

    void GPIO_TIM5_CH4_Init(void);
    void DMA_TIM5_CH4_Init(void);
    void TIM5_InputCapture_Init(void);

    public:
    engineRPM();
    void cyclicHandling(void);
};

#endif