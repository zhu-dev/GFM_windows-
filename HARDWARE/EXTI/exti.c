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

//�ⲿ�ж�0�������
void EXTI0_IRQHandler(void)
{
	delay_ms(10);	//����
		
		printf("\r\n�������͵翪�ؼ�⵽����\r\n");
		enter_home = 1;
		//LED1 = !LED1;
		EXTI_ClearITPendingBit(EXTI_Line0); //���LINE0�ϵ��жϱ�־λ 
}	

//�ⲿ�ж�2�������
void EXTI2_IRQHandler(void)
{
	//���ģ���ǵ͵�ƽ��Ч
	//ע����ֵ��ʱ֮��ģ�����һֱ���ж�
	 delay_ms(10);	//����
	
	if(raint_count>1)
	{
		rain = 1;
		raint_count = 0;
		//LED0 = !LED0;
		printf("\r\n��⵽�����\r\n");
	}
	 raint_count++;
	 EXTI_ClearITPendingBit(EXTI_Line2);//���LINE2�ϵ��жϱ�־λ 
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
			//������
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
			//��������
			window_isOpen = 0;
			//close_window_flag = 0;
		}
		
		printf("mh2->%d mh1->%d\r\n",mh1,mh2);
		printf("window_isOpen  %d\r\n",window_isOpen);
		EXTI_ClearITPendingBit(EXTI_Line7);
	}
}
	   
//�ⲿ�жϳ�ʼ������
//��ʼ��PA0Ϊ�ж�����.
void HC_EXTI_Init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	//HC_SR_Init();
	
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//ʹ��SYSCFGʱ��
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);//PA0 ���ӵ��ж���0
	
  /* ����EXTI_Line0 */
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;//LINE0
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //�����ش��� 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ��LINE0
  EXTI_Init(&EXTI_InitStructure);//����
	

	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//�ⲿ�ж�0
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//��ռ���ȼ�0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//�����ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);//����
	
	   
}

void Rain_EXIT_Init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//ʹ��SYSCFGʱ��
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2);//PE2 ���ӵ��ж���2
	
	/* ����EXTI_Line2 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line2 ;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½��ش���
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//�ж���ʹ��
  EXTI_Init(&EXTI_InitStructure);//����
	

	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;//�ⲿ�ж�2
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//��ռ���ȼ�3
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//�����ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);//����

}


void MH_EXIT_Init(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//ʹ��SYSCFGʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//ʹ��GPIOEʱ��
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource7);//PE2 ���ӵ��ж���2
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource5);//PE2 ���ӵ��ж���2
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_7;//��Ӧ����PE2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;//����
  GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��GPIOE
	
	/* ����EXTI_Line2 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line5|EXTI_Line7 ;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½��ش���
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//�ж���ʹ��
  EXTI_Init(&EXTI_InitStructure);//����
	

	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;//�ⲿ�ж�2 EXTI9_5_IRQHandler
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;//��ռ���ȼ�3
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//�����ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);//����

}


void ReadWindowStatus(void)
{
	if(MH1&&MH2)
	{
		 //�����ϵ�ʱ�Ǵ򿪵�
		window_isOpen = 1;
		mh1 = 1;
		mh2 = 1;
	}
	if(!MH1&&!MH2)
	{
		 //�����ϵ�ʱ�ǹرյ�
		window_isOpen = 0;
		mh1 = 0;
		mh2 = 0;
	}
	
			printf("window_isOpen init  %d\r\n",window_isOpen);
}






