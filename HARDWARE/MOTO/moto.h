#ifndef __MOTO_H
#define __MOTO_H

#include "stm32f10x.h"

//配置电机驱动的四个引脚
#define MOTO_CLK  RCC_APB2Periph_GPIOC
#define MOTO_PORT GPIOC
#define MOTO_Pin1  GPIO_Pin_0 
#define MOTO_Pin2  GPIO_Pin_1
#define MOTO_Pin3  GPIO_Pin_2
#define MOTO_Pin4  GPIO_Pin_3 

/* 带参宏，可以像内联函数一样使用 */
#define IN4(a)	if (a)	\
					GPIO_SetBits(MOTO_PORT,MOTO_Pin4);\
					else		\
					GPIO_ResetBits(MOTO_PORT,MOTO_Pin4)
					
#define IN3(a)	if (a)	\
					GPIO_SetBits(MOTO_PORT,MOTO_Pin3);\
					else		\
					GPIO_ResetBits(MOTO_PORT,MOTO_Pin3)
					
#define IN2(a)	if (a)	\
					GPIO_SetBits(MOTO_PORT,MOTO_Pin2);\
					else		\
					GPIO_ResetBits(MOTO_PORT,MOTO_Pin2)
					
#define IN1(a)	if (a)	\
					GPIO_SetBits(MOTO_PORT,MOTO_Pin1);\
					else		\
					GPIO_ResetBits(MOTO_PORT,MOTO_Pin1)

void MOTO_GPIO_Config(void);
void Car_Go(void);
void Car_Back(void);
void Car_Turn_Right(void);
void Car_Turn_Left(void);
					void Car_Stop(void);
#endif
