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
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);    //初始化延时函数
	uart_init(115200);	//初始化串口波特率为115200
	LED_Init();					//初始化LED 
 	LCD_Init();         //初始化LCD接口
	//Adc_Init();         //初始化ADC
	
	MQ2_Init();						//初始化MQ2
	HC_SR_Init();					
	Rain_Detect_Init();
	
	while(DHT11_Init())	//DHT11初始化	
	
	
	POINT_COLOR=RED; 
	LCD_ShowString(30,50,200,16,16,"Explorer STM32F4");	
	LCD_ShowString(30,70,200,16,16,"ADC TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2014/5/6");	  
	POINT_COLOR=BLUE;//设置字体为蓝色
	LCD_ShowString(30,130,200,16,16,"ADC1_CH5_VAL:");	      
	LCD_ShowString(30,150,200,16,16,"ADC1_CH5_VOL:0.000V");	//先在固定位置显示小数点      
	while(1)
	{ 

	
		
		 if(t%100==0)//每100ms读取一次
		{		
				LED0=!LED0;
				
				DHT11_Read_Data(&temperature,&humidity);		//读取温湿度值					    
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



