//编码器-输入捕获模式-只测量A相脉冲。不使用AB相同时测量。

#include "encoder.h"


uint32_t CaptureNumber = 0;

//测量PA2的脉冲
void ENCODER_GPIO_Init(void) 
{

	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//对应引脚PA2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;//下拉
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_TIM2);
	
}


void TIM2_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
	
		NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级0
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级1
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
		NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
}

void ENCODER_TIM2_Init(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef        TIM_ICInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//TIM2时钟使能    
	
	//定时器TIM2初始化
	TIM_TimeBaseStructure.TIM_Period = 0xffff; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =84-1; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位

  /* 初始化TIM2输入捕获参数 */
  /* CC1S=01 	选择输入端 IC1映射到TI1上 */
	TIM_ICInitStructure.TIM_Channel     = TIM_Channel_3;
  /* 上升沿捕获 */
	TIM_ICInitStructure.TIM_ICPolarity  = TIM_ICPolarity_Rising;	
  /* 映射到TI1上 */
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 
  /* 配置输入分频,不分频  */
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	           
  /* IC1F=0000 配置输入滤波器 不滤波 */
	TIM_ICInitStructure.TIM_ICFilter    = 0x00;                         
	TIM_ICInit(TIM2, &TIM_ICInitStructure);  
  
  /* 配置NVIC */
  TIM2_NVIC_Configuration();
  
  /* 允许更新中断 ,允许CC3IE捕获中断	 */
  TIM_ITConfig(TIM2,TIM_IT_CC3, ENABLE);
	
  /* 使能定时器 */
  TIM_Cmd(TIM2, ENABLE);


}


void ENCODER_Init(void)
{
  ENCODER_GPIO_Init();
  ENCODER_TIM2_Init();	
}

void TIM2_IRQHandler(void)
{
 	if(TIM_GetITStatus(TIM2, TIM_IT_CC3) == SET )              //捕获中断
	{
     /* Clear TIM3 Capture compare interrupt pending bit */
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC3);
    CaptureNumber++;  // 每来一个上升沿加一
	}
}




