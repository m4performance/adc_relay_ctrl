#include "rcc.h"


#define RCC_PLLCFGR_PLLM_VALUE            8


//---------------------------------------------------------------------------
// Function change clock source: PLL sourced from HSI to PLL sourced from HSE
//---------------------------------------------------------------------------
void RCCtrl::pllHseInit(void)
{
  RCC->CR |= RCC_CR_HSEON;                                // enable HSE
  while((RCC->CR & RCC_CR_HSERDY) == 0)                   // wait for HSE ready
  {
  }
  
  RCC->CFGR &= ~(RCC_CFGR_SW);                            // change clock from PLL to HSI
  while((RCC->CFGR & (RCC_CFGR_SWS)) != RCC_CFGR_SWS_HSI) // wait until HSI will by clock source
  {
  }
  RCC->CR &= ~(RCC_CR_PLLON);                             // disable PLL
  while(RCC->CR & RCC_CR_PLLRDY)                          // wait to unlock PLL
  {
  }
  RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC;
  RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLM);
  RCC->PLLCFGR |= (RCC_PLLCFGR_PLLM_VALUE << RCC_PLLCFGR_PLLM_Pos);

  RCC->CR |= RCC_CR_PLLON;                                // enable PLL
  while((RCC->CR & RCC_CR_PLLRDY) == 0)                   // wait to lock PLL
  {
  }
  RCC->CFGR |= RCC_CFGR_SW_PLL;
  while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL)
  {
  }
}