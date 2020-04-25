#include "common.h"
#include "stdio.h"
#include "led.h"


//用户配置区

//连接端口号:8086,可自行修改为其他端口.
const u8* portnum="8787";		 

//WIFI STA模式,设置要去连接的路由器无线参数,请根据你自己的路由器设置,自行修改.
const char* wifista_ssid="TP-LINK_301";			//路由器SSID号
const char* wifista_password="19881020"; 	//连接密码

extern u8 window_isOpen;

u8 isAlive=0;	//连接状态


void check_app_cmd(void)
{
	if(strstr((const char*)USART3_RX_BUF,"app:hbp!")== NULL)
	{
		 if(strstr((const char*)USART3_RX_BUF,"open"))
		 {
			 if(window_isOpen == 0)
			 {
				 	LED0 = !LED0;
				 //打开窗户
				 //window_isOpen = 1;
			 }

		 }
		 else if(strstr((const char*)USART3_RX_BUF,"close"))
		 {
			 
			 	if(window_isOpen == 1)
			 {
				  LED1= !LED1;
				  //关闭窗户
					//window_isOpen = 0;
			 }

		 }
	}
}

//usmart支持部分
//将收到的AT指令应答数据返回给电脑串口
//mode:0,不清零USART3_RX_STA;
//     1,清零USART3_RX_STA;
void atk_8266_at_response(u8 mode)
{
	char *buf;
	if(USART3_RX_STA&0X8000)		//接收到一次数据了
	{ 
		USART3_RX_BUF[USART3_RX_STA&0X7FFF]=0;//添加结束符
		printf("%s\r\n",USART3_RX_BUF);	//发送到串口
		
			if(strstr((const char*)USART3_RX_BUF,"app:hbp!")!= NULL) isAlive = 1;
			else if(strstr((const char*)USART3_RX_BUF,"open")) isAlive = 1;
			else if(strstr((const char*)USART3_RX_BUF,"close")) isAlive = 1;
			else isAlive = 0;
		
			check_app_cmd();//判断下发的数据
		
		if(mode)USART3_RX_STA=0;
	} 
}
//ATK-ESP8266发送命令后,检测接收到的应答
//str:期待的应答结果
//返回值:0,没有得到期待的应答结果
//    其他,期待应答结果的位置(str的位置)
char* atk_8266_check_cmd(char *str)
{
	
	char *strx=0;
	if(USART3_RX_STA&0X8000)		//接收到一次数据了
	{ 
		USART3_RX_BUF[USART3_RX_STA&0X7FFF]=0;//添加结束符
		strx=strstr((const char*)USART3_RX_BUF,(const char*)str);
	} 
	return strx;
}
//向ATK-ESP8266发送命令
//cmd:发送的命令字符串
//ack:期待的应答结果,如果为空,则表示不需要等待应答
//waittime:等待时间(单位:10ms)
//返回值:0,发送成功(得到了期待的应答结果)
//       1,发送失败
u8 atk_8266_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART3_RX_STA=0;
	u3_printf("%s\r\n",cmd);	//发送命令
	printf("[cmd]%s\r\n",cmd);
	if(ack&&waittime)		//需要等待应答
	{
		while(--waittime)	//等待倒计时
		{
			delay_ms(100);
			if(USART3_RX_STA&0X8000)//接收到期待的应答结果
			{
				if(atk_8266_check_cmd(ack))
				{
					printf("[ack]:%s\r\n",(u8*)ack);
					break;//得到有效数据 
				}
					USART3_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
} 
//向ATK-ESP8266发送指定数据
//data:发送的数据(不需要添加回车了)
//ack:期待的应答结果,如果为空,则表示不需要等待应答
//waittime:等待时间(单位:10ms)
//返回值:0,发送成功(得到了期待的应答结果)luojian
u8 atk_8266_send_data(u8 *data,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART3_RX_STA=0;
	u3_printf("%s",data);	//发送命令
	if(ack&&waittime)		//需要等待应答
	{
		while(--waittime)	//等待倒计时
		{
			delay_ms(10);
			if(USART3_RX_STA&0X8000)//接收到期待的应答结果
			{
				if(atk_8266_check_cmd(ack))break;//得到有效数据 
				USART3_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
}



//ATK-ESP8266退出透传模式
//返回值:0,退出成功;
//       1,退出失败
u8 atk_8266_quit_trans(void)
{
	while((USART3->SR&0X40)==0);	//等待发送空
	USART3->DR='+';      
	delay_ms(15);					//大于串口组帧时间(10ms)
	while((USART3->SR&0X40)==0);	//等待发送空
	USART3->DR='+';      
	delay_ms(15);					//大于串口组帧时间(10ms)
	while((USART3->SR&0X40)==0);	//等待发送空
	USART3->DR='+';      
	delay_ms(500);					//等待500ms
	return atk_8266_send_cmd("AT","OK",20);//退出透传判断.
}

//获取ATK-ESP8266模块的连接状态
//返回值:0,未连接;1,连接成功.
u8 atk_8266_consta_check(void)
{
	u8 *p;
	u8 res;
	if(atk_8266_quit_trans())return 0;			//退出透传 
	atk_8266_send_cmd("AT+CIPSTATUS",":",50);	//发送AT+CIPSTATUS指令,查询连接状态
	p=atk_8266_check_cmd("+CIPSTATUS:"); 
	res=*p;									//得到连接状态	
	return res;
}


//获取AP+STA ip地址并在指定位置显示
//ipbuf:ip地址输出缓存区
char* atk_8266_get_ip(void)
{
		char *p;
		char *p1;
		const char s[2] = "\"";
		char *ip;
		atk_8266_send_cmd("AT+CIFSR","OK",10);//获取WAN IP地址失败	
		p = atk_8266_check_cmd("STAIP,\"");//返回第一次出现匹配字符串位置(地址)
		p1 = strstr(p,"\"");
		ip =	strtok(p1+1,s);
		//printf("ip->:%s\r\n",ip);
		return ip;
	
}




//STA模式下的AP的TCP、UDP工作模式配置
u8 atk_8266_mode_cofig(u8 netpro)
{
	//u8 netpro;
	u8 ipbuf[16]; 	//IP缓存
	u8 *p;
	
	sprintf((char*)p,"AT+CIPSTART=0,\"TCP\",\"%s\",%s",ipbuf,(u8*)portnum); 
	
	return netpro;
}

//ATK-ESP8266模块测试主函数
void atk_8266_test(void)
{

	while(atk_8266_send_cmd("AT","OK",20))//检查WIFI模块是否在线
	{
		atk_8266_quit_trans();//退出透传
		atk_8266_send_cmd("AT+CIPMODE=0","OK",200);  //关闭透传模式	
	} 
	
	//while(atk_8266_send_cmd("ATE0","OK",20));//关闭回显
	
	while(1)
	{
		delay_ms(10); 
		atk_8266_at_response(1);//检查ATK-ESP8266模块发送过来的数据,及时上传给电脑

		//atk_8266_wifista_test();//WIFI STA测试
	}
			
		
} 


u8 atk_8266_wifista_init(void)
{
	//u8 netpro=0;	//网络模式
	u8 key;
	u8 timex=0; 
	char *ip; 	//IP
	char p[30];
	u16 t=999;		//加速第一次获取链接状态
	u8 res=0;
	u16 rlen=0;
	u8 constate=0;	//连接状态

	//设置模组工作方式
	while(atk_8266_send_cmd("AT+CWMODE=1","OK",10))	//设置WIFI STA模式
	{
		printf("[ERROR]SET MODE FAILD..\r\n");
	}
	
	//重启模组
	while(atk_8266_send_cmd("AT+RST","WIFI GOT IP",80))		
	{
			printf("[ERROR]RST MODEL FAILD..\r\n");
	}

	//设置连接到的WIFI网络名称/加密方式/密码
	sprintf(p,"AT+CWJAP=\"%s\",\"%s\"",wifista_ssid,wifista_password);//设置无线参数:ssid,密码
	while(atk_8266_send_cmd(p,"OK",80))				//连接目标路由器,并且获得IP
	{
		printf("[ERROR]SET SSID&PASSWORD FAILD...\r\n");
	}
	
	//设置多连接模式
	atk_8266_send_cmd("AT+CIPMUX=1","OK",10);   //0：单连接，1：多连接
	
	//开启服务器模式
	sprintf(p,"AT+CIPSERVER=1,%s",(u8*)portnum);    //开启Server模式(0，关闭；1，打开)，端口号为portnum
	atk_8266_send_cmd(p,"OK",10);
	
	//获取服务器IP
	ip =	atk_8266_get_ip();//服务器模式,获取WAN IP
	
	//将IP地址在OLED上显示出来
	printf("[INFO]localIp:%s\r\n",ip);

	return res;		
} 
 
	 




