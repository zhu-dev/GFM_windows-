#include "common.h"
#include "stdlib.h"


//ATK-ESP8266 WIFI STA测试
//用于测试TCP/UDP连接
//返回值:0,正常
//    其他,错误代码
u8 netpro=0;	//网络模式
u8 atk_8266_wifista2_test(void)
{
	//u8 netpro=0;	//网络模式
	u8 key;
	u8 timex=0; 
	u8 ipbuf[16]; 	//IP缓存
	u8 *p;
	u16 t=999;		//加速第一次获取链接状态
	u8 res=0;
	u16 rlen=0;
	u8 constate=0;	//连接状态

	atk_8266_send_cmd("AT+CWMODE=1","OK",50);		//设置WIFI STA模式
	atk_8266_send_cmd("AT+RST","OK",20);		//DHCP服务器关闭(仅AP模式有效) 
	delay_ms(1000);         //延时3S等待重启成功
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	//设置连接到的WIFI网络名称/加密方式/密码,这几个参数需要根据您自己的路由器设置进行修改!! 
	sprintf((char*)p,"AT+CWJAP=\"%s\",\"%s\"",wifista_ssid,wifista_password);//设置无线参数:ssid,密码
	while(atk_8266_send_cmd(p,"WIFI GOT IP",300));					//连接目标路由器,并且获得IP


	atk_8266_send_cmd("AT+CIPMUX=1","OK",20);   //0：单连接，1：多连接
	sprintf((char*)p,"AT+CIPSERVER=1,%s",(u8*)portnum);    //开启Server模式(0，关闭；1，打开)，端口号为portnum
	atk_8266_send_cmd(p,"OK",50);    


	//atk_8266_get_wanip(ipbuf);//服务器模式,获取WAN IP
	sprintf((char*)p,"IP地址:%s 端口:%s",ipbuf,(u8*)portnum);
	//将IP地址在OLED上显示出来
	
//	USART3_RX_STA=0;
//			while(1)
//			{
//						atk_8266_send_cmd("AT+CIPSEND=0,25","OK",200);  //发送指定长度的数据
//						delay_ms(200);
//						atk_8266_send_data(p,"OK",100);  //发送指定长度的数据
//						
//				
//		
//				if(USART3_RX_STA&0X8000)		//接收到一次数据了
//				{ 
//					rlen=USART3_RX_STA&0X7FFF;	//得到本次接收到的数据长度
//					USART3_RX_BUF[rlen]=0;		//添加结束符 
//					printf("%s",USART3_RX_BUF);	//发送到串口   
//					sprintf((char*)p,"收到%d字节,内容如下",rlen);//接收到的字节数 

//					USART3_RX_STA=0;
//					if(constate!='+')t=1000;		//状态为还未连接,立即更新连接状态
//					else t=0;                   //状态为已经连接了,10秒后再检查
//				}  
//				if(t==1000)//连续10秒钟没有收到任何数据,检查连接是不是还存在.
//				{
//					constate=atk_8266_consta_check();//得到连接状态
//					if(constate=='+') ;//Show_Str(30+30,80,200,12,"连接成功",12,0);  //连接状态
//					else; //Show_Str(30+30,80,200,12,"连接失败",12,0); 	 
//					t=0;
//				}
//				if((t%20)==0)//LED0=!LED0;
//				atk_8266_at_response(1);
//			}

	return res;		
} 




























