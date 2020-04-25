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

u8 window_isOpen;
u8 rain;

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
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);    //��ʼ����ʱ����
	uart_init(115200);	//��ʼ�����ڲ�����Ϊ115200
	usart3_init(115200);
	LED_Init();					//��ʼ��LED 
	//Adc_Init();         //��ʼ��ADC
	
//	MQ2_Init();						//��ʼ��MQ2
//	HC_SR_Init();					
//	Rain_Detect_Init();
		ENCODER_Init();
//	while(DHT11_Init())	//DHT11��ʼ��	

	LED0 = 1;
	LED1 = 1;
	
	atk_8266_wifista_init();

	while(1)
	{ 
			atk_8266_at_response(1);
		
			delay_ms(10); 
			
			//1���ȡһ�α���������->����
			if(time %100 == 0)
			{
				windspeed=CaptureNumber*60/13;//ʵ��ÿ���ӵ��ת��
				printf("[sensor]CaptureNumber:%d\r\n",CaptureNumber);
				printf("[sensor]windspeed:%d\r\n",windspeed);
			}
			
			
			
			if(isAlive && time%400==0)//�յ��ͻ����������������˵�����ӻ��ڣ����Է�����.
			{
				
					//��������
					LED0=!LED0;
					
//					DHT11_Read_Data(&temperature,&humidity);		//��ȡ��ʪ��ֵ					    
//					printf("\r\n[sensor]temperature:%d  humidity:%d\r\n",temperature,humidity);
//					
//					ppm = MQ2_GetPPM();//��ȡ��������
//					ppm_1 = ppm;
//						if(ppm_1<500) smokelevel = 0;
//						if(ppm_1>1000) smokelevel = 1;
//						if(ppm_1>2000) smokelevel = 2;
//						if(ppm_1>9999) smokelevel = 3;				
//					printf("[sensor]ppm:%f \r\n",ppm_1);		
					
					//�������״̬
					
					//����
					if(windspeed<100) windlevel = 0;
					if(windspeed<500) windlevel = 1;
					if(windspeed<1000) windlevel = 2;
				  if(windspeed<9999) windlevel = 3;

					//��������״̬,������ʪ�ȣ����������٣�����Ŀ��ϴ���
					
					//�������ݵ��ͻ���
					memset(p,0x00,sizeof(p));
					sprintf(p,"%2d%2d%d%d%d%d",26,50,smokelevel,rain,windlevel,window_isOpen);
					atk_8266_send_cmd("AT+CIPSEND=0,25","OK",20);  //����ָ�����ȵ�����
					delay_ms(10);
					atk_8266_send_data(p,"OK",10);  //����ָ�����ȵ�����
					printf("[debug]senddata:%s\r\n",p);
					
			}
			if(time >800) time = 0;
			time++;
					
		}			

}




