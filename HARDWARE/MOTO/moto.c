/*
********************************************************************************************************
文件名：moto.c
功  能：直流减速电机控制
备  注：	霍尔测试AB相
淘  宝：https://shop60670850.taobao.com
作  者：lilianhe
日  期: 2017-2-6
*********************************************************************************************************
*/

#include "moto.h"


//***************************配置电机驱动IO口***************************//
//电机驱动与单片机端口配置：IN1-IN2 连接PC3-PC2，
//							          IN3-IN4 连接PC1-PC0，

//电机与驱动连接 右电机上--OUT1，右电机下--OUT2，
//							左电机上--OUT3，左电机下--OUT4
void MOTO_GPIO_Config(void)
{		
		/*定义一个GPIO_InitTypeDef类型的结构体*/
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd( MOTO_CLK, ENABLE);                                	  /*开启GPIO的外设时钟*/																   
		GPIO_InitStructure.GPIO_Pin = MOTO_Pin1 | MOTO_Pin2 | MOTO_Pin3 | MOTO_Pin4;	/*选择要控制的GPIO引脚*/	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                              /*设置引脚模式为通用推挽输出*/   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                             /*设置引脚速率为50MHz */ 
		GPIO_Init(MOTO_PORT, &GPIO_InitStructure);                                    /*调用库函数，初始化GPIO*/	
}

//***************************左右电机向前转动***************************//
//只要配置INx()的状态就可以改变电机转动方向
void Car_Go(void)
{
	IN1(0);// 左电机前进
	IN2(1);
	IN3(1);//	右电机前进
	IN4(0);
	
	
}

//***************************左右电机向后转动***************************//
void Car_Back(void)
{
	IN1(1);// 左电机后退
	IN2(0);
	IN3(0);//	右电机后退
	IN4(1);
}

//***************************小车原地右转***************************//
void Car_Turn_Right(void)
{
	IN1(0);// 左电机前进
	IN2(1);
	IN3(0);//	右电机后退
	IN4(1);
}

//***************************小车原地左转***************************//
void Car_Turn_Left(void)
{
	IN1(1);// 左电机后退
	IN2(0);
	IN3(1);//	右电机前进
	IN4(0);
}

//***************************停车***************************//
void Car_Stop(void)
{
	IN4(0);
	IN3(0);//	
	IN2(0);
	IN1(0);//	
}


