#include "engine_rpm.h"


#define CAPTURE_BUF_SIZE    1024


volatile uint32_t CaptureBuf[CAPTURE_BUF_SIZE];



void engineRPM::GPIO_TIM5_CH4_Init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    /* PA3 -> AF2 TIM5_CH4 */

    GPIOA->MODER &= ~(3U << (3*2));
    GPIOA->MODER |=  (2U << (3*2));

    GPIOA->OSPEEDR |= (3U << (3*2));

    GPIOA->PUPDR &= ~(3U << (3*2));

    GPIOA->AFR[0] &= ~(0xFU << (3*4));
    GPIOA->AFR[0] |=  (2U << (3*4));
}


void engineRPM::DMA_TIM5_CH4_Init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
    DMA1_Stream1->CR &= ~DMA_SxCR_EN;
    while(DMA1_Stream1->CR & DMA_SxCR_EN);

    DMA1->LIFCR = DMA_LIFCR_CTCIF1 |
                  DMA_LIFCR_CHTIF1 |
                  DMA_LIFCR_CTEIF1 |
                  DMA_LIFCR_CDMEIF1 |
                  DMA_LIFCR_CFEIF1;

    DMA1_Stream1->PAR  = (uint32_t)&TIM5->CCR4;
    DMA1_Stream1->M0AR = (uint32_t)CaptureBuf;
    DMA1_Stream1->NDTR = CAPTURE_BUF_SIZE;
    DMA1_Stream1->CR = 0;

    /* Channel - dobrać wg RM0390 */
    DMA1_Stream1->CR |= (6U << DMA_SxCR_CHSEL_Pos);
    DMA1_Stream1->CR &= ~DMA_SxCR_DIR;                  // Bits 7:6 DIR[1:0]: data transfer direction - peripheral-to-memory
    DMA1_Stream1->CR |= DMA_SxCR_MINC;                  // Bit 10 MINC: memory increment mode - memory address pointer is incremented
    DMA1_Stream1->CR |= DMA_SxCR_CIRC;                  // Bit 8 CIRC: circular mode - circular mode enabled

    DMA1_Stream1->CR |= DMA_SxCR_PSIZE_1;               // Bits 12:11 PSIZE[1:0]: peripheral data size - word (32-bit)
    DMA1_Stream1->CR |= DMA_SxCR_MSIZE_1;               // Bits 14:13 MSIZE[1:0]: memory data size - word (32-bit)
    DMA1_Stream1->CR |= DMA_SxCR_PL_1;                  // Bits 17:16 PL[1:0]: priority level - high
    DMA1_Stream1->FCR = 0;                      
    DMA1_Stream1->CR |= DMA_SxCR_EN;
}


void engineRPM::TIM5_InputCapture_Init(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;

    TIM5->CR1 = 0;
    TIM5->PSC = 89;
    TIM5->ARR = 0xFFFFFFFF;
    TIM5->CCMR2 = 0;
    TIM5->CCMR2 |= TIM_CCMR2_CC4S_0;
    TIM5->CCMR2 &= ~TIM_CCMR2_IC4PSC;                   // IC prescaler = 1
    TIM5->CCMR2 &= ~TIM_CCMR2_IC4F;                     // Filter OFF
    TIM5->CCER &= ~(TIM_CCER_CC4P | TIM_CCER_CC4NP);    // Rising edge   
    TIM5->DIER |= TIM_DIER_CC4DE;                       // Enable DMA request
    TIM5->CCER |= TIM_CCER_CC4E;                        // Enable Capture
    TIM5->CR1 |= TIM_CR1_CEN;                           // Start timer
}


engineRPM::engineRPM(void)
{
    GPIO_TIM5_CH4_Init();
    DMA_TIM5_CH4_Init();
    TIM5_InputCapture_Init();
}


void engineRPM::cyclicHandling(void)
{
    
}