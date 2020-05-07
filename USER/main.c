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
#include "exti.h"



u8 window_isOpen;
u8 rain;
u8 enter_home;
u8 open_window_flag ;
u8 close_window_flag;
u8 update_window_status;


u8 temp_threshold = 50;
u8 humi_threshold = 80;
u8 wind_threshold = 1000;
u8 somok_threshold = 2000;


char *ip; 	//IP

void check_app_cmd(void);
void openWindow(void);
void closeWindow(void);

int main(void)
{ 
 			
	u16 time = 0;
	char p[30];

	float ppm ;
	u16 ppm_1 = 0;
	u8 smokelevel = 0;
	
	u8 temperature;  	    
	u8 humidity;
	char temp_str[2];
	char humi_str[2];
	
	uint32_t windspeed = 0;
	u8 windlevel = 0;
	
	open_window_flag = 0;
	close_window_flag = 0;
	

	update_window_status = 1; //上电首次连接客户端，就马上上报目前窗户状态。
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
	
	OLED_P8x16Str(0,0,"IP:",0);
	OLED_P8x16Str(16,0,":",0);
	
	LED0 = 1;
	LED1 = 1;
	
	printf("[debug]1\r\n");
	
	atk_8266_wifista_init();
	OLED_P8x16Str(24,0,ip,0);
	LED0 = 0;
	
	sim800a_pdu_init();
	LED1 = 0;
	
	printf("[debug]:init sensor....\r\n");
	MQ2_Init();						//初始化MQ2
	
	HC_SR_Init();			
	
	Rain_Detect_Init();

	ENCODER_Init();
	
	while(DHT11_Init())	//DHT11初始化
	printf("[debug]:init sensor ok\r\n");
	
	TIM3_PWM_Init(1000-1,84-1); //84M/84 = 1MHZ 1MHZ/1000 = 1KHZ

	
	OLED_CLS();

	MH_EXIT_Init();
	
	ReadWindowStatus();//读取窗户上电时的状态
	
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
			
		atk_8266_at_response(1);//非柱塞接收
		
		if(time%20 ==0)  //将窗户数据发送给客户端
		{
				if(isAlive&&update_window_status)
				{
					update_window_status = 0;
					USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);//开启相关中断
					//发送数据到客户端
					memset(p,0x00,sizeof(p));
					sprintf(p,"%c%d",'w',window_isOpen);
					atk_8266_send_cmd("AT+CIPSEND=0,2","OK",10);  //发送指定长度的数据
					delay_ms(10);
					atk_8266_send_data(p,"OK",10);  //发送指定长度的数据
					printf("[debug]senddata:%s\r\n",p);
					USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启相关中断
				}
		}		

						
			//轮询是否有人进入房间
			if(enter_home)
			{
				sim800a_send_warning();
				enter_home = 0;
			}
			
			
			//1秒读取一次编码器数据->风速
			if(time %10 == 0)
			{
				windspeed=CaptureNumber*60/13;//实际每分钟电机转速
				//printf("[sensor]CaptureNumber:%d\r\n",CaptureNumber);
				
			}
			
			if(time%40==0||time ==0)//定时读取传感器数据
			{
				
					//发送数据
					LED0=!LED0;
					
					DHT11_Read_Data(&temperature,&humidity);		//读取温湿度值		
				
					if(temperature>temp_threshold)  close_window_flag = 1;
						
					if(humidity>humi_threshold) close_window_flag = 1;
						
					printf("\r\n[sensor]temperature:%d  humidity:%d\r\n",temperature,humidity);
					
					ppm = MQ2_GetPPM();//读取烟雾数据
					ppm_1 = ppm;
				
					if(ppm_1<500) smokelevel = 0;
					if(ppm_1>1000) smokelevel = 1;
					if(ppm_1>2000) smokelevel = 2;
				
					if(ppm_1>somok_threshold) open_window_flag = 1;	
				
					printf("[sensor]ppm:%d \r\n",ppm_1);		
					
					//监测下雨状态
					
				
					//风速
					if(windspeed<100) windlevel = 0;
					if(windspeed<1000&&windspeed>100) windlevel = 1;
					if(windspeed<5000&&windspeed>1000) windlevel = 2;
					
				  if(windspeed>wind_threshold)  close_window_flag = 1;
					
					printf("[sensor]windspeed:%d\r\n",windspeed);
					CaptureNumber = 0;
		
					sprintf(temp_str,"%d",temperature);
					sprintf(humi_str,"%d",humidity);
					
					
					/* 显示动态数据 */
					OLED_P8x16Str(104,0,humi_str,0);//湿度 
					OLED_P8x16Str(68,0,temp_str,0);//温度
					OLED_Show16x16(100,2,13+windlevel,0); //风速           低13  中14  高15
					OLED_Show16x16(100,4,17-rain,0);//雨量检测        晴17  雨16
					OLED_Show16x16(100,6,13+smokelevel,0); //空气烟雾浓度   低13  中14  高15
					
					//窗户开合状态,根据温湿度，雨量，风速，合理的开合窗户

					
//					          String temp = data_ascii_str.substring(0, 2);
//                    String humidity = data_ascii_str.substring(2, 4);
//                    char smokelevel = data_ascii_str.charAt(4);
//                    char rainlevel = data_ascii_str.charAt(5);
//                    char windlevel = data_ascii_str.charAt(6);
		
					if(isAlive) //当连接存在时发送数据到客户端
					{
						
							USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);//开启相关中断
							//发送数据到客户端
							memset(p,0x00,sizeof(p));
							sprintf(p,"%c%2d%2d%d%d%d",'d',temperature,humidity,smokelevel,rain,windlevel);
							atk_8266_send_cmd("AT+CIPSEND=0,8","OK",10);  //发送指定长度的数据
							delay_ms(10);
							atk_8266_send_data(p,"OK",10);  //发送指定长度的数据
							printf("[debug]senddata:%s\r\n",p);
						  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启相关中断
					}
					
					rain =0;
					
			}
			
			
			if(open_window_flag)
		{
				printf("openWindow1---open_window_flag：%d\r\n",open_window_flag);
				openWindow();
				
				open_window_flag = 0;
				printf("openWindow2---open_window_flag：%d\r\n",open_window_flag);
		}
		
		
		if(close_window_flag)
		{
			printf("closeWindow---close_window_flag：%d\r\n",close_window_flag);
			closeWindow();
			
			 close_window_flag = 0;
			 printf("closeWindow---close_window_flag：%d\r\n",close_window_flag);
		}
		
			delay_ms(100);
			if(time >100) time = 0;
			time++;					
		}			

}


void openWindow(void)
{
		while(open_window_flag&&!window_isOpen)
		{
			TIM_SetCompare4(TIM3,400);	//修改比较值，修改占空比  电机PWM控制
			TIM_SetCompare3(TIM3,0);	//修改比较值，修改占空比
		}
		update_window_status = 1;
		
		TIM_SetCompare4(TIM3,0);	//修改比较值，修改占空比  电机PWM控制
		TIM_SetCompare3(TIM3,0);	//修改比较值，修改占空比
	
}

void closeWindow(void)
{
		while(close_window_flag&&window_isOpen)
		{
				TIM_SetCompare4(TIM3,0);	//修改比较值，修改占空比  电机PWM控制
				TIM_SetCompare3(TIM3,400);	//修改比较值，修改占空比
		}
		update_window_status = 1;
		
		TIM_SetCompare4(TIM3,0);	//修改比较值，修改占空比  电机PWM控制
		TIM_SetCompare3(TIM3,0);	//修改比较值，修改占空比
		

}

