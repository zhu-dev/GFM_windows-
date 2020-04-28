#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "adc.h"

#include "mq_sensor.h"
#include "hc_sr501.h"
#include "dht11.h"
#include "rain_detect.h"
#include "common.h"
#include "usart3.h" 
#include "encoder.h"
#include "oled.h"
#include "sim800a.h"
#include "usart6.h" 

u8 window_isOpen;
u8 rain;
u8 enter_home;

void check_app_cmd(void);


int main(void)
{ 
 	
		
	u16 time;
	char p[30];

	float ppm ;
	u16 ppm_1 = 0;
	u8 smokelevel = 0;
	
	u8 temperature;  	    
	u8 humidity;  
	
	uint32_t windspeed = 0;
	u8 windlevel = 0;
	
	window_isOpen = 0;
	rain = 0;
	isAlive=0;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);    //初始化延时函数
	usart_init(115200);	//初始化串口波特率为115200
	usart3_init(115200);
	usart6_init(115200);
	LED_Init();					//初始化LED 
	OLED_Init();
//	TIM3_PWM_Init(1000-1,84-1); //84M/84 = 1MHZ 1MHZ/1000 = 1KHZ
	//Adc_Init();         //初始化ADC
	
//	MQ2_Init();						//初始化MQ2
//	HC_SR_Init();					
//	Rain_Detect_Init();
//		ENCODER_Init();
//	while(DHT11_Init())	//DHT11初始化	

	LED0 = 1;
	LED1 = 1;
	printf("[debug]1\r\n");
	//atk_8266_wifista_init();
	sim800a_pdu_init();
	//sim800a_send_warning();
	
	while(1)
	{ 

	
			/* 静态信息 */
			OLED_Show16x16(0,0,1,0);//温湿度
			OLED_Show16x16(16,0,2,0);
			OLED_Show16x16(32,0,3,0);
			OLED_P8x16Str(84,0,"C",0);
			OLED_P8x16Str(120,0,"%",0);
		
		
			OLED_Show16x16(0,2,4,0);//风速
			OLED_Show16x16(16,2,5,0);
				
			OLED_Show16x16(0,4,6,0);//雨量检测
		  OLED_Show16x16(16,4,7,0);
		
			OLED_Show16x16(0,6,8,0);//空气烟雾浓度
			OLED_Show16x16(16,6,9,0);
			OLED_Show16x16(32,6,10,0);
			OLED_Show16x16(48,6,11,0);
			
			/* 动态数据 */
			OLED_P8x16Str(104,0,"80",0);//湿度 
			OLED_P8x16Str(68,0,"26",0);//温度
			OLED_Show16x16(100,2,13,0); //风速           低13  中14  高15
			OLED_Show16x16(100,4,17,0);//雨量检测        晴17  雨16
			OLED_Show16x16(100,6,13,0); //空气烟雾浓度   低13  中14  高15
		
		
			//atk_8266_at_response(1);
			//TIM_SetCompare4(TIM3,500);	//修改比较值，修改占空比
			//TIM_SetCompare3(TIM3,500);	//修改比较值，修改占空比
			delay_ms(10); 
			
			
			//轮询是否有人进入房间
			if(enter_home)
			{
				sim800a_send_warning();
				enter_home = 0;
			}
			
			
			//1秒读取一次编码器数据->风速
//			if(time %100 == 0)
//			{
//				windspeed=CaptureNumber*60/13;//实际每分钟电机转速
//				printf("[sensor]CaptureNumber:%d\r\n",CaptureNumber);
//				printf("[sensor]windspeed:%d\r\n",windspeed);
//			}
			
			
			
			
			if(isAlive && time%400==0)//收到客户端心跳包或者命令，说明连接还在，可以发数据.
			{
				
					//发送数据
					LED0=!LED0;
					
//					DHT11_Read_Data(&temperature,&humidity);		//读取温湿度值					    
//					printf("\r\n[sensor]temperature:%d  humidity:%d\r\n",temperature,humidity);
//					
//					ppm = MQ2_GetPPM();//读取烟雾数据
//					ppm_1 = ppm;
//						if(ppm_1<500) smokelevel = 0;
//						if(ppm_1>1000) smokelevel = 1;
//						if(ppm_1>2000) smokelevel = 2;
//						if(ppm_1>9999) smokelevel = 3;				
//					printf("[sensor]ppm:%f \r\n",ppm_1);		
					
					//监测下雨状态
					
				
					//风速
					if(windspeed<100) windlevel = 0;
					if(windspeed<500) windlevel = 1;
					if(windspeed<1000) windlevel = 2;
				  if(windspeed<9999) windlevel = 3;

		
					/* 显示动态数据 */
					OLED_P8x16Str(104,0,"80",0);//湿度 
					OLED_P8x16Str(68,0,"26",0);//温度
					OLED_Show16x16(100,2,13,0); //风速           低13  中14  高15
					OLED_Show16x16(100,4,17,0);//雨量检测        晴17  雨16
					OLED_Show16x16(100,6,13,0); //空气烟雾浓度   低13  中14  高15
					//窗户开合状态,根据温湿度，雨量，风速，合理的开合窗户
					//TIM_SetCompare4(TIM3,500);	//修改比较值，修改占空比
					
					
					//发送数据到客户端
					memset(p,0x00,sizeof(p));
					sprintf(p,"%2d%2d%d%d%d%d",26,50,smokelevel,rain,windlevel,window_isOpen);
					atk_8266_send_cmd("AT+CIPSEND=0,25","OK",20);  //发送指定长度的数据
					delay_ms(10);
					atk_8266_send_data(p,"OK",10);  //发送指定长度的数据
					printf("[debug]senddata:%s\r\n",p);
					
			}
			if(time >800) time = 0;
			time++;
					
		}			

}




