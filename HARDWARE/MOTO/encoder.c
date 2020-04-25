//������-���벶��ģʽ-ֻ����A�����塣��ʹ��AB��ͬʱ������

#include "encoder.h"


uint32_t CaptureNumber = 0;

//����PA2������
void ENCODER_GPIO_Init(void) 
{

	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ��
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//��Ӧ����PA2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//����ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;//����
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_TIM2);
	
}


void TIM2_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
	
		NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�0
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�1
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
		NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
}

void ENCODER_TIM2_Init(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef        TIM_ICInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//TIM2ʱ��ʹ��    
	
	//��ʱ��TIM2��ʼ��
	TIM_TimeBaseStructure.TIM_Period = 0xffff; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =84-1; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

  /* ��ʼ��TIM2���벶����� */
  /* CC1S=01 	ѡ������� IC1ӳ�䵽TI1�� */
	TIM_ICInitStructure.TIM_Channel     = TIM_Channel_3;
  /* �����ز��� */
	TIM_ICInitStructure.TIM_ICPolarity  = TIM_ICPolarity_Rising;	
  /* ӳ�䵽TI1�� */
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 
  /* ���������Ƶ,����Ƶ  */
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	           
  /* IC1F=0000 ���������˲��� ���˲� */
	TIM_ICInitStructure.TIM_ICFilter    = 0x00;                         
	TIM_ICInit(TIM2, &TIM_ICInitStructure);  
  
  /* ����NVIC */
  TIM2_NVIC_Configuration();
  
  /* ��������ж� ,����CC3IE�����ж�	 */
  TIM_ITConfig(TIM2,TIM_IT_CC3, ENABLE);
	
  /* ʹ�ܶ�ʱ�� */
  TIM_Cmd(TIM2, ENABLE);


}


void ENCODER_Init(void)
{
  ENCODER_GPIO_Init();
  ENCODER_TIM2_Init();	
}

void TIM2_IRQHandler(void)
{
 	if(TIM_GetITStatus(TIM2, TIM_IT_CC3) == SET )              //�����ж�
	{
     /* Clear TIM3 Capture compare interrupt pending bit */
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC3);
    CaptureNumber++;  // ÿ��һ�������ؼ�һ
	}
}




