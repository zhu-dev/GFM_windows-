#include "common.h"
#include "stdio.h"
#include "led.h"


//�û�������

//���Ӷ˿ں�:8086,�������޸�Ϊ�����˿�.
const u8* portnum="8787";		 

//WIFI STAģʽ,����Ҫȥ���ӵ�·�������߲���,��������Լ���·��������,�����޸�.
const char* wifista_ssid="TP-LINK_301";			//·����SSID��
const char* wifista_password="19881020"; 	//��������

extern u8 window_isOpen;

u8 isAlive=0;	//����״̬


void check_app_cmd(void)
{
	if(strstr((const char*)USART3_RX_BUF,"app:hbp!")== NULL)
	{
		 if(strstr((const char*)USART3_RX_BUF,"open"))
		 {
			 if(window_isOpen == 0)
			 {
				 	LED0 = !LED0;
				 //�򿪴���
				 //window_isOpen = 1;
			 }

		 }
		 else if(strstr((const char*)USART3_RX_BUF,"close"))
		 {
			 
			 	if(window_isOpen == 1)
			 {
				  LED1= !LED1;
				  //�رմ���
					//window_isOpen = 0;
			 }

		 }
	}
}

//usmart֧�ֲ���
//���յ���ATָ��Ӧ�����ݷ��ظ����Դ���
//mode:0,������USART3_RX_STA;
//     1,����USART3_RX_STA;
void atk_8266_at_response(u8 mode)
{
	char *buf;
	if(USART3_RX_STA&0X8000)		//���յ�һ��������
	{ 
		USART3_RX_BUF[USART3_RX_STA&0X7FFF]=0;//��ӽ�����
		printf("%s\r\n",USART3_RX_BUF);	//���͵�����
		
			if(strstr((const char*)USART3_RX_BUF,"app:hbp!")!= NULL) isAlive = 1;
			else if(strstr((const char*)USART3_RX_BUF,"open")) isAlive = 1;
			else if(strstr((const char*)USART3_RX_BUF,"close")) isAlive = 1;
			else isAlive = 0;
		
			check_app_cmd();//�ж��·�������
		
		if(mode)USART3_RX_STA=0;
	} 
}
//ATK-ESP8266���������,�����յ���Ӧ��
//str:�ڴ���Ӧ����
//����ֵ:0,û�еõ��ڴ���Ӧ����
//    ����,�ڴ�Ӧ������λ��(str��λ��)
char* atk_8266_check_cmd(char *str)
{
	
	char *strx=0;
	if(USART3_RX_STA&0X8000)		//���յ�һ��������
	{ 
		USART3_RX_BUF[USART3_RX_STA&0X7FFF]=0;//��ӽ�����
		strx=strstr((const char*)USART3_RX_BUF,(const char*)str);
	} 
	return strx;
}
//��ATK-ESP8266��������
//cmd:���͵������ַ���
//ack:�ڴ���Ӧ����,���Ϊ��,���ʾ����Ҫ�ȴ�Ӧ��
//waittime:�ȴ�ʱ��(��λ:10ms)
//����ֵ:0,���ͳɹ�(�õ����ڴ���Ӧ����)
//       1,����ʧ��
u8 atk_8266_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART3_RX_STA=0;
	u3_printf("%s\r\n",cmd);	//��������
	printf("[cmd]%s\r\n",cmd);
	if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			delay_ms(100);
			if(USART3_RX_STA&0X8000)//���յ��ڴ���Ӧ����
			{
				if(atk_8266_check_cmd(ack))
				{
					printf("[ack]:%s\r\n",(u8*)ack);
					break;//�õ���Ч���� 
				}
					USART3_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
} 
//��ATK-ESP8266����ָ������
//data:���͵�����(����Ҫ��ӻس���)
//ack:�ڴ���Ӧ����,���Ϊ��,���ʾ����Ҫ�ȴ�Ӧ��
//waittime:�ȴ�ʱ��(��λ:10ms)
//����ֵ:0,���ͳɹ�(�õ����ڴ���Ӧ����)luojian
u8 atk_8266_send_data(u8 *data,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART3_RX_STA=0;
	u3_printf("%s",data);	//��������
	if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			delay_ms(10);
			if(USART3_RX_STA&0X8000)//���յ��ڴ���Ӧ����
			{
				if(atk_8266_check_cmd(ack))break;//�õ���Ч���� 
				USART3_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
}



//ATK-ESP8266�˳�͸��ģʽ
//����ֵ:0,�˳��ɹ�;
//       1,�˳�ʧ��
u8 atk_8266_quit_trans(void)
{
	while((USART3->SR&0X40)==0);	//�ȴ����Ϳ�
	USART3->DR='+';      
	delay_ms(15);					//���ڴ�����֡ʱ��(10ms)
	while((USART3->SR&0X40)==0);	//�ȴ����Ϳ�
	USART3->DR='+';      
	delay_ms(15);					//���ڴ�����֡ʱ��(10ms)
	while((USART3->SR&0X40)==0);	//�ȴ����Ϳ�
	USART3->DR='+';      
	delay_ms(500);					//�ȴ�500ms
	return atk_8266_send_cmd("AT","OK",20);//�˳�͸���ж�.
}

//��ȡATK-ESP8266ģ�������״̬
//����ֵ:0,δ����;1,���ӳɹ�.
u8 atk_8266_consta_check(void)
{
	u8 *p;
	u8 res;
	if(atk_8266_quit_trans())return 0;			//�˳�͸�� 
	atk_8266_send_cmd("AT+CIPSTATUS",":",50);	//����AT+CIPSTATUSָ��,��ѯ����״̬
	p=atk_8266_check_cmd("+CIPSTATUS:"); 
	res=*p;									//�õ�����״̬	
	return res;
}


//��ȡAP+STA ip��ַ����ָ��λ����ʾ
//ipbuf:ip��ַ���������
char* atk_8266_get_ip(void)
{
		char *p;
		char *p1;
		const char s[2] = "\"";
		char *ip;
		atk_8266_send_cmd("AT+CIFSR","OK",10);//��ȡWAN IP��ַʧ��	
		p = atk_8266_check_cmd("STAIP,\"");//���ص�һ�γ���ƥ���ַ���λ��(��ַ)
		p1 = strstr(p,"\"");
		ip =	strtok(p1+1,s);
		//printf("ip->:%s\r\n",ip);
		return ip;
	
}




//STAģʽ�µ�AP��TCP��UDP����ģʽ����
u8 atk_8266_mode_cofig(u8 netpro)
{
	//u8 netpro;
	u8 ipbuf[16]; 	//IP����
	u8 *p;
	
	sprintf((char*)p,"AT+CIPSTART=0,\"TCP\",\"%s\",%s",ipbuf,(u8*)portnum); 
	
	return netpro;
}

//ATK-ESP8266ģ�����������
void atk_8266_test(void)
{

	while(atk_8266_send_cmd("AT","OK",20))//���WIFIģ���Ƿ�����
	{
		atk_8266_quit_trans();//�˳�͸��
		atk_8266_send_cmd("AT+CIPMODE=0","OK",200);  //�ر�͸��ģʽ	
	} 
	
	//while(atk_8266_send_cmd("ATE0","OK",20));//�رջ���
	
	while(1)
	{
		delay_ms(10); 
		atk_8266_at_response(1);//���ATK-ESP8266ģ�鷢�͹���������,��ʱ�ϴ�������

		//atk_8266_wifista_test();//WIFI STA����
	}
			
		
} 


u8 atk_8266_wifista_init(void)
{
	//u8 netpro=0;	//����ģʽ
	u8 key;
	u8 timex=0; 
	char *ip; 	//IP
	char p[30];
	u16 t=999;		//���ٵ�һ�λ�ȡ����״̬
	u8 res=0;
	u16 rlen=0;
	u8 constate=0;	//����״̬

	//����ģ�鹤����ʽ
	while(atk_8266_send_cmd("AT+CWMODE=1","OK",10))	//����WIFI STAģʽ
	{
		printf("[ERROR]SET MODE FAILD..\r\n");
	}
	
	//����ģ��
	while(atk_8266_send_cmd("AT+RST","WIFI GOT IP",80))		
	{
			printf("[ERROR]RST MODEL FAILD..\r\n");
	}

	//�������ӵ���WIFI��������/���ܷ�ʽ/����
	sprintf(p,"AT+CWJAP=\"%s\",\"%s\"",wifista_ssid,wifista_password);//�������߲���:ssid,����
	while(atk_8266_send_cmd(p,"OK",80))				//����Ŀ��·����,���һ��IP
	{
		printf("[ERROR]SET SSID&PASSWORD FAILD...\r\n");
	}
	
	//���ö�����ģʽ
	atk_8266_send_cmd("AT+CIPMUX=1","OK",10);   //0�������ӣ�1��������
	
	//����������ģʽ
	sprintf(p,"AT+CIPSERVER=1,%s",(u8*)portnum);    //����Serverģʽ(0���رգ�1����)���˿ں�Ϊportnum
	atk_8266_send_cmd(p,"OK",10);
	
	//��ȡ������IP
	ip =	atk_8266_get_ip();//������ģʽ,��ȡWAN IP
	
	//��IP��ַ��OLED����ʾ����
	printf("[INFO]localIp:%s\r\n",ip);

	return res;		
} 
 
	 




