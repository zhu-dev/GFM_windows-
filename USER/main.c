#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "adc.h"

#include "mq_sensor.h"
#include "hc_sr501.h"
#include "dht11.h"
#include "motor.h"
#include "rain_detect.h"



int main(void)
{ 
 	
	float ppm;
	
	u8 t=0;			    
	u8 temperature;  	    
	u8 humidity;  
	
	u8 rain = 0;
	u8 wind = 0;
	
	u8 window_isOpen = 0;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);    //��ʼ����ʱ����
	uart_init(115200);	//��ʼ�����ڲ�����Ϊ115200
	LED_Init();					//��ʼ��LED 
 	LCD_Init();         //��ʼ��LCD�ӿ�
	//Adc_Init();         //��ʼ��ADC
	
	MQ2_Init();						//��ʼ��MQ2
	HC_SR_Init();					
	Rain_Detect_Init();
	
	while(DHT11_Init())	//DHT11��ʼ��	
	
	
	POINT_COLOR=RED; 
	LCD_ShowString(30,50,200,16,16,"Explorer STM32F4");	
	LCD_ShowString(30,70,200,16,16,"ADC TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2014/5/6");	  
	POINT_COLOR=BLUE;//��������Ϊ��ɫ
	LCD_ShowString(30,130,200,16,16,"ADC1_CH5_VAL:");	      
	LCD_ShowString(30,150,200,16,16,"ADC1_CH5_VOL:0.000V");	//���ڹ̶�λ����ʾС����      
	while(1)
	{ 

	
		
		 if(t%100==0)//ÿ100ms��ȡһ��
		{		
				LED0=!LED0;
				
				DHT11_Read_Data(&temperature,&humidity);		//��ȡ��ʪ��ֵ					    
				printf("\r\ntemperature:%d  humidity:%d\r\n",temperature,humidity);
				
				ppm = MQ2_GetPPM();
				printf("\r\nppm:%f \r\n",ppm);
				
				
				
				if(temperature >20&&temperature <45)
				{
					if(window_isOpen !=1)
					{
							open_sw();
							window_isOpen = 1;
					}

				}
				
				if(ppm>5000)
				{
					if(window_isOpen !=1)
					{
							open_sw();
							window_isOpen = 1;
					}
				
				}
				
				if(temperature<13||rain>100||wind>4)
				{
					
					if(window_isOpen !=0)
					{
							close_sw();
							window_isOpen = 0;
					}

				}
				
		}	
		
	 	delay_ms(10);
		t++;
		
		if(t==200)
		{
			t=0;
		}
		
		
		
		//delay_ms(250);	
	}
}



