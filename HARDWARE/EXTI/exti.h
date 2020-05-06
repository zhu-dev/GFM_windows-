#ifndef __EXTI_H
#define __EXIT_H	 
#include "sys.h"  	

#define		DISABLE_EXIT_0		(EXTI->IMR &= ~(EXTI_Line0))
#define		ENABLE_EXIT_0			(EXTI->IMR != EXTI_Line0)

#define		DISABLE_EXIT_2		(EXTI->IMR &= ~(EXTI_Line2))
#define		ENABLE_EXIT_2			(EXTI->IMR != EXTI_Line2)


void HC_EXTI_Init(void);		
void Rain_EXIT_Init(void);
#endif

























