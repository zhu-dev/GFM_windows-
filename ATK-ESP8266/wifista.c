#include "common.h"
#include "stdlib.h"


//ATK-ESP8266 WIFI STA����
//���ڲ���TCP/UDP����
//����ֵ:0,����
//    ����,�������
u8 netpro=0;	//����ģʽ
u8 atk_8266_wifista2_test(void)
{
	//u8 netpro=0;	//����ģʽ
	u8 key;
	u8 timex=0; 
	u8 ipbuf[16]; 	//IP����
	u8 *p;
	u16 t=999;		//���ٵ�һ�λ�ȡ����״̬
	u8 res=0;
	u16 rlen=0;
	u8 constate=0;	//����״̬

	atk_8266_send_cmd("AT+CWMODE=1","OK",50);		//����WIFI STAģʽ
	atk_8266_send_cmd("AT+RST","OK",20);		//DHCP�������ر�(��APģʽ��Ч) 
	delay_ms(1000);         //��ʱ3S�ȴ������ɹ�
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	//�������ӵ���WIFI��������/���ܷ�ʽ/����,�⼸��������Ҫ�������Լ���·�������ý����޸�!! 
	sprintf((char*)p,"AT+CWJAP=\"%s\",\"%s\"",wifista_ssid,wifista_password);//�������߲���:ssid,����
	while(atk_8266_send_cmd(p,"WIFI GOT IP",300));					//����Ŀ��·����,���һ��IP


	atk_8266_send_cmd("AT+CIPMUX=1","OK",20);   //0�������ӣ�1��������
	sprintf((char*)p,"AT+CIPSERVER=1,%s",(u8*)portnum);    //����Serverģʽ(0���رգ�1����)���˿ں�Ϊportnum
	atk_8266_send_cmd(p,"OK",50);    


	//atk_8266_get_wanip(ipbuf);//������ģʽ,��ȡWAN IP
	sprintf((char*)p,"IP��ַ:%s �˿�:%s",ipbuf,(u8*)portnum);
	//��IP��ַ��OLED����ʾ����
	
//	USART3_RX_STA=0;
//			while(1)
//			{
//						atk_8266_send_cmd("AT+CIPSEND=0,25","OK",200);  //����ָ�����ȵ�����
//						delay_ms(200);
//						atk_8266_send_data(p,"OK",100);  //����ָ�����ȵ�����
//						
//				
//		
//				if(USART3_RX_STA&0X8000)		//���յ�һ��������
//				{ 
//					rlen=USART3_RX_STA&0X7FFF;	//�õ����ν��յ������ݳ���
//					USART3_RX_BUF[rlen]=0;		//��ӽ����� 
//					printf("%s",USART3_RX_BUF);	//���͵�����   
//					sprintf((char*)p,"�յ�%d�ֽ�,��������",rlen);//���յ����ֽ��� 

//					USART3_RX_STA=0;
//					if(constate!='+')t=1000;		//״̬Ϊ��δ����,������������״̬
//					else t=0;                   //״̬Ϊ�Ѿ�������,10����ټ��
//				}  
//				if(t==1000)//����10����û���յ��κ�����,��������ǲ��ǻ�����.
//				{
//					constate=atk_8266_consta_check();//�õ�����״̬
//					if(constate=='+') ;//Show_Str(30+30,80,200,12,"���ӳɹ�",12,0);  //����״̬
//					else; //Show_Str(30+30,80,200,12,"����ʧ��",12,0); 	 
//					t=0;
//				}
//				if((t%20)==0)//LED0=!LED0;
//				atk_8266_at_response(1);
//			}

	return res;		
} 




























