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
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);    //初始化延时函数
	uart_init(115200);	//初始化串口波特率为115200
	usart3_init(115200);
	LED_Init();					//初始化LED 
	//Adc_Init();         //初始化ADC
	
//	MQ2_Init();						//初始化MQ2
//	HC_SR_Init();					
//	Rain_Detect_Init();
//	
//	while(DHT11_Init())	//DHT11初始化	

	LED0 = 0;
	LED1 = 0;
	atk_8266_wifista_test();

	while(1)
	{ 
	
	delay_ms(10); 
	atk_8266_at_response(1);
	
//		 if(t%100==0)//每100ms读取一次
//		{		
//				LED0=!LED0;
//				
//				DHT11_Read_Data(&temperature,&humidity);		//读取温湿度值					    
//				printf("\r\ntemperature:%d  humidity:%d\r\n",temperature,humidity);
//				
//				ppm = MQ2_GetPPM();
//				printf("\r\nppm:%f \r\n",ppm);							
//		 }
				
		}	
		

}



