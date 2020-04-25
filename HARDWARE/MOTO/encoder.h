#ifndef __ENCODER_H
#define __ENCODER_H

#include "stm32f4xx.h" 


extern uint32_t CaptureNumber;

void ENCODER_GPIO_Init(void);
void ENCODER_TIM2_Init(void);
void ENCODER_Init(void);

#endif

