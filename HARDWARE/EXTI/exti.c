#include "exti.h"
#include "delay.h" 
#include "led.h"
#include "usart.h"

extern u8 rain;
extern u8 enter_home;
extern u8 window_isOpen;
extern u8 open_window_flag;
extern u8 close_window_flag;


u8 raint_count = 0;

u8 mh1;
u8 mh2;

//外部中断0服务程序
void EXTI0_IRQHandler(void)
{
	delay_ms(10);	//消抖
		
		printf("\r\n红外热释电开关监测到有人\r\n");
		enter_home = 1;
		//LED1 = !LED1;
		EXTI_ClearITPendingBit(EXTI_Line0); //清除LINE0上的中断标志位 
}	

//外部中断2服务程序
void EXTI2_IRQHandler(void)
{
	//雨滴模块是低电平有效
	//注意阈值延时之类的，避免一直进中断
	 delay_ms(10);	//消抖
	
	if(raint_count>1)
	{
		rain = 1;
		raint_count = 0;
		//LED0 = !LED0;
		printf("\r\n监测到有雨滴\r\n");
	}
	 raint_count++;
	 EXTI_ClearITPendingBit(EXTI_Line2);//清除LINE2上的中断标志位 
}


void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line5)!=RESET )
	{
		delay_ms(20);
		LED1 = !LED1;
		mh1 = 1;
		if(mh2 == 1)
		{
			mh2 = 0; 
			//窗打开了
			window_isOpen = 1;
			//open_window_flag = 0;
		}
		printf("mh1->%d mh2->%d\r\n",mh1,mh2);
		printf("window_isOpen  %d\r\n",window_isOpen);
		EXTI_ClearITPendingBit(EXTI_Line5);
	}
	
		if(EXTI_GetITStatus(EXTI_Line7)!=RESET )
	{
		delay_ms(20);
		LED0 = !LED0;
		mh2 = 1;
		if(mh1 == 1)
		{
			 mh1 = 0;
			//窗关上了
			window_isOpen = 0;
			//close_window_flag = 0;
		}
		
		printf("mh2->%d mh1->%d\r\n",mh1,mh2);
		printf("window_isOpen  %d\r\n",window_isOpen);
		EXTI_ClearITPendingBit(EXTI_Line7);
	}
}
	   
//外部中断初始化程序
//初始化PA0为中断输入.
void HC_EXTI_Init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	//HC_SR_Init();
	
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);//PA0 连接到中断线0
	
  /* 配置EXTI_Line0 */
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;//LINE0
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //上升沿触发 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能LINE0
  EXTI_Init(&EXTI_InitStructure);//配置
	

	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//外部中断0
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//抢占优先级0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);//配置
	
	   
}

void Rain_EXIT_Init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2);//PE2 连接到中断线2
	
	/* 配置EXTI_Line2 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line2 ;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//中断线使能
  EXTI_Init(&EXTI_InitStructure);//配置
	

	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;//外部中断2
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//抢占优先级3
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);//配置

}


void MH_EXIT_Init(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//使能GPIOE时钟
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource7);//PE2 连接到中断线2
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource5);//PE2 连接到中断线2
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_7;//对应引脚PE2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;//上拉
  GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化GPIOE
	
	/* 配置EXTI_Line2 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line5|EXTI_Line7 ;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//中断线使能
  EXTI_Init(&EXTI_InitStructure);//配置
	

	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;//外部中断2 EXTI9_5_IRQHandler
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;//抢占优先级3
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);//配置

}


void ReadWindowStatus(void)
{
	if(MH1&&MH2)
	{
		 //窗户上电时是打开的
		window_isOpen = 1;
		mh1 = 1;
		mh2 = 1;
	}
	if(!MH1&&!MH2)
	{
		 //窗户上电时是关闭的
		window_isOpen = 0;
		mh1 = 0;
		mh2 = 0;
	}
	
			printf("window_isOpen init  %d\r\n",window_isOpen);
}






