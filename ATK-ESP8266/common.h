#ifndef __COMMON_H__
#define __COMMON_H__	 
#include "sys.h"
#include "usart.h"		
#include "delay.h"		 
#include "string.h"    	
#include "usart3.h" 


void atk_8266_init(void);

u8 atk_8266_mode_cofig(u8 netpro);
void atk_8266_at_response(u8 mode);
char* atk_8266_check_cmd(char *str);

u8 atk_8266_send_data(u8 *data,u8 *ack,u16 waittime);
u8 atk_8266_send_cmd(u8 *cmd,u8 *ack,u16 waittime);
u8 atk_8266_consta_check(void);
u8 atk_8266_quit_trans(void);

void atk_8266_get_ip(char* ipbuf);

void atk_8266_test(void);

u8 atk_8266_wifista_test(void);

//用户配置参数
extern const u8* portnum;			//连接端口
 
extern const char* wifista_ssid;		//WIFI STA SSID
extern const char* wifista_password; 	//WIFI STA 密码


#endif





