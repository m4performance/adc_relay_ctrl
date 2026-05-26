#ifndef RCC_H
#define RCC_H


#include "global.h"
#include "mbed.h"
#include <BufferedSerial.h>


#ifdef RCC_DEBUG_ENABLED
  #define RCC_DEBUG(x)                      printf(x)
  #define RCC_DEBUG_VAR(x,y)                printf(x,y)
#else
  #define RCC_DEBUG(x)                      do{} while(0)
  #define RCC_DEBUG_VAR(x,y)                do{} while(0)
#endif


class RCCtrl
{
    public:
    void pllHseInit(void);
};

#endif