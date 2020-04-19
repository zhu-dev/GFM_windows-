#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "adc.h"

#include "mq_sensor.h"
#include "hc_sr501.h"
#include "dht11.h"
#include "motor.h"
#include "rain_detect.h"
#include "common.h"
#include "usart3.h" 

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
	usart3_init(115200);
	LED_Init();					//��ʼ��LED 
	//Adc_Init();         //��ʼ��ADC
	
//	MQ2_Init();						//��ʼ��MQ2
//	HC_SR_Init();					
//	Rain_Detect_Init();
//	
//	while(DHT11_Init())	//DHT11��ʼ��	

	LED0 = 0;
	LED1 = 0;
	atk_8266_wifista_test();

	while(1)
	{ 
	
	delay_ms(10); 
	atk_8266_at_response(1);
	
//		 if(t%100==0)//ÿ100ms��ȡһ��
//		{		
//				LED0=!LED0;
//				
//				DHT11_Read_Data(&temperature,&humidity);		//��ȡ��ʪ��ֵ					    
//				printf("\r\ntemperature:%d  humidity:%d\r\n",temperature,humidity);
//				
//				ppm = MQ2_GetPPM();
//				printf("\r\nppm:%f \r\n",ppm);							
//		 }
				
		}	
		

}



